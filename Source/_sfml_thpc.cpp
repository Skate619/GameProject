#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Network.hpp>

#include <gl\GL.h>
#include <gl\GLU.h>

#include "thpc_game.h"


// Globally-accessible references to SFML objects
// This is currently the best way I have to implement platform-game separation
// Not sure how possible it'll be to separate without using "global" references yet
struct SFMLInterface
{
	sf::RenderWindow *win; // Reference to currently active (target) window
	thpc::Game *game; // Reference to the game object
	sf::Vector2i lastMousePos; // Needed because apparently SFML doesn't track mouse pos change
	unsigned int winW,winH;
} sfi;


void sfProcessInput(sf::RenderWindow *win, thpc::Game *game)
{
	thpc::InputRef inputRef = {};

	sf::Vector2i mousePos = sf::Mouse::getPosition(*win);
	thpc::floatXY mouseDelta =
	{
		(float)(mousePos.x - sfi.lastMousePos.x),
		(float)(mousePos.y - sfi.lastMousePos.y)
	};

	game->inputManager.preEvents();

	sf::Event ev;
	while(win->pollEvent(ev))
	{
		switch(ev.type)
		{
			case sf::Event::Closed:
			{
				game->end();
			}
			break;

			case sf::Event::KeyPressed:
				game->inputManager.processEvent(thpc::InputRef(thpc::IPT_Keyboard,ev.key.code), thpc::IPS_Press);
				break;
			case sf::Event::KeyReleased:
				game->inputManager.processEvent(thpc::InputRef(thpc::IPT_Keyboard,ev.key.code), thpc::IPS_Release);
				break;

			case sf::Event::MouseButtonPressed:
				game->inputManager.processEvent(thpc::InputRef(thpc::IPT_MouseButton,ev.mouseButton.button), thpc::IPS_Press);
				break;
			case sf::Event::MouseButtonReleased:
				game->inputManager.processEvent(thpc::InputRef(thpc::IPT_MouseButton,ev.mouseButton.button), thpc::IPS_Release);
				break;

			case sf::Event::MouseMoved:
				game->inputManager.processEvent(thpc::InputRef(thpc::IPT_MouseAxis, thpc::IPC_Axis_MouseX), thpc::IPS_Static, mouseDelta.x);
				game->inputManager.processEvent(thpc::InputRef(thpc::IPT_MouseAxis, thpc::IPC_Axis_MouseY), thpc::IPS_Static, mouseDelta.y);
				break;

			case sf::Event::MouseWheelMoved:
				game->inputManager.processEvent(thpc::InputRef(thpc::IPT_MouseAxis, thpc::IPC_Axis_MouseWheelV), thpc::IPS_Static, (float)ev.mouseWheel.delta);
			{
				inputRef.type = thpc::IPT_MouseAxis;
				inputRef.code = thpc::IPC_Axis_MouseWheelV;
				game->inputManager.processEvent(inputRef, thpc::IPS_Static, (float)ev.mouseWheel.delta);
			} 
			break;

			default: 
				break;
		}
	}

	sfi.lastMousePos = mousePos;
}


sf::Color sfColFromRGBA(thpc::RGBA rgba)
{
	sf::Color result;
	result.r = rgba.r;
	result.g = rgba.g;
	result.b = rgba.b;
	result.a = rgba.a;
	return result;
}

void thpc::Draw::clear(unsigned char r, unsigned char g, unsigned char b)
{
	sfi.win->clear(sf::Color(r,g,b));
}

// This implementation of Draw::line does not have a width
// It is either visible (and "1px" wide) or invisible (when lineWidth is 0)
void thpc::Draw::line(floatXY a, floatXY b)
{
	if(this->lineWidth == 0.f) return;

	sf::Vertex verts[] =
	{
		sf::Vertex(sf::Vector2f(a.x,a.y),sfColFromRGBA(this->lineColor)),
		sf::Vertex(sf::Vector2f(b.x,b.y),sfColFromRGBA(this->lineColor))
	};
	
	sfi.win->draw(verts, 2, sf::Lines);
}

void thpc::Draw::rect(thpc::floatXY pos, thpc::floatXY size)
{
	sf::RectangleShape shape;
	sf::Vector2f sfSize;
	sfSize.x = size.x; sfSize.y = size.y;
	
	shape.setPosition(pos.x,pos.y);
	shape.setSize(sfSize);

	shape.setFillColor(sfColFromRGBA(this->fillColor));
	if(this->lineWidth!=0.f)
	{
		shape.setOutlineColor(sfColFromRGBA(this->lineColor));
		shape.setOutlineThickness(this->lineWidth);
	}
	
	sfi.win->draw(shape);
}

void thpc::Draw::circle(thpc::floatXY center, float radius)
{
	sf::CircleShape shape;
	
	shape.setPosition(center.x-radius,center.y-radius);
	shape.setRadius(radius);
	
	shape.setFillColor(sfColFromRGBA(this->fillColor));
	if(this->lineWidth!=0.f)
	{
		shape.setOutlineColor(sfColFromRGBA(this->lineColor));
		shape.setOutlineThickness(this->lineWidth);
	}
	
	sfi.win->draw(shape);
}

void thpc::Draw::tri(floatXY a, floatXY b, floatXY c)
{
	sf::ConvexShape shape;

	shape.setPointCount(3);
	shape.setPoint(0, sf::Vector2f(a.x,a.y));
	shape.setPoint(1, sf::Vector2f(b.x,b.y));
	shape.setPoint(2, sf::Vector2f(c.x,c.y));

	shape.setFillColor(sfColFromRGBA(this->fillColor));
	if(this->lineWidth!=0.f)
	{
		shape.setOutlineColor(sfColFromRGBA(this->lineColor));
		shape.setOutlineThickness(this->lineWidth);
	}

	sfi.win->draw(shape);
}

void thpc::Draw::poly(floatXY *points, size_t count)
{
	sf::ConvexShape shape;

	shape.setPointCount(count);
	for(size_t i=0; i<count; ++i)
	{
		shape.setPoint(i, sf::Vector2f(points[i].x,points[i].y));
	}

	shape.setFillColor(sfColFromRGBA(this->fillColor));
	if(this->lineWidth!=0.f)
	{
		shape.setOutlineColor(sfColFromRGBA(this->lineColor));
		shape.setOutlineThickness(this->lineWidth);
	}

	sfi.win->draw(shape);
}



void thpc::Platform::kill()
{
	sfi.win->close();
}


int main(int argc, char* argv)
{
	FreeConsole();
	sfi = {};
	sfi.winW = 1280;
	sfi.winH = 720;

	thpc::Platform p;
	p.draw.winSize.x = (float)sfi.winW;
	p.draw.winSize.y = (float)sfi.winH;
	
	sf::String title = "Tank Helicopter Planecar!";
	sf::ContextSettings context;
	context.antialiasingLevel = 8;

	sf::RenderWindow App(sf::VideoMode(sfi.winW,sfi.winH), title, sf::Style::Close, context);
	sfi.win = &App;

	thpc::Game g(&p);
	g.init();

	sf::Clock time;
	time.restart();

	while (App.isOpen())
	{
		float elapse = time.getElapsedTime().asSeconds();
		time.restart();

		sfProcessInput(&App, &g);

		g.update(elapse);
		g.draw();

		sfi.win->display();
	}

	return EXIT_SUCCESS;
}