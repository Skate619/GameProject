#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <sstream>
using namespace std;
const int MAX_BULLETS = 3;

class Game
{
public:
	Game(sf::RenderWindow* app);
	sf::RenderWindow* GameWindow;
	void Init();
	void Draw();
	void UserInput();
	void Update();
	void Play();
	void SetName(sf::RenderWindow* app, string myString);
	string GetText(sf::Event event, string myString);
	string InputName(sf::RenderWindow* app);
	void Reverse(string myString);
	void FizzBuzz();
protected:
	sf::Clock clock;

private:
	sf::Event event;
	sf::Text help;
	sf::Text playerName;
	sf::Font arial;

	bool Enter = false;
	bool name = true;

	float helpWidth = 0;
	float inputWidth = 0;

	string myString;

};
#endif