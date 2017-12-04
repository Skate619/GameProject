#include "thpc_game.h"
using namespace thpc;


Game::Game(Platform *platform)
	: platform(platform)
{}

void Game::init()
{
	this->time.reset();

	this->inputManager.bind(0, ACT_DebugKill, InputRef(IPT_Keyboard,36));
	this->inputManager.bind(0, ACT_CursorAbsX, InputRef(IPT_MouseAxis,IPC_Axis_MouseX));
	this->inputManager.bind(0, ACT_CursorAbsY, InputRef(IPT_MouseAxis,IPC_Axis_MouseY));
}

void Game::update(float elapse)
{
	// TODO/Jen: poll actions within inputManager and do stuff to game objects
	 if (this->inputManager.getActionPressed(0,ACT_DebugKill))
		this->platform->kill();
	
	this->cursor.x = this->inputManager.getActionValue(0, ACT_CursorAbsX);
	this->cursor.y = this->inputManager.getActionValue(0, ACT_CursorAbsY);
}

void Game::update(Time elapse)
{
	float passElapse = elapse.getSecondsReal(false);
    this->update(passElapse);
}

void Game::draw()
{
	Draw *d = &(this->platform->draw);

	float border = 100.f;
	float hbord = border*.5f;
	float dbord = border*2.f;
	float ww = d->winSize.x, wh = d->winSize.y;
	
	d->reset();
	d->clear();

	d->setFillColor(255,0,0);
	d->rect({0,hbord},{border,wh-border});
	d->setFillColor(255,255,0);
	d->rect({hbord,0},{ww-border,border});
	d->setFillColor(0,255,0);
	d->rect({ww-border,hbord},{border,wh-border});
	d->setFillColor(0,255,255);
	d->rect({hbord,wh-border},{ww-border,border});
	
	d->setLineWidth(16.f);
	d->setLineColor(0,128,255);
	d->line({0,0},{ww,wh});
	
	d->setLineWidth(0.f);
	d->setFillColor(0,0,255,192);
	d->circle({border,border},hbord);
	
	d->setLineWidth(4.f);
	d->setLineColor(255,128,0);
	d->setFillColor(255,0,255,64);
	d->circle({ww-border,wh*.5f},dbord);
	
	d->setLineWidth(2.f);
	d->setLineColor(255,255,255,128);
	d->setFillColor(128,128,128,128);
	floatXY testVerts[] =
	{
		{ww*.3f,  wh*.3f},
		{ww*.7f,  wh*.25f},
		{ww*.65f, wh*.6f},
		{ww*.5f,  wh*.8f},
		{ww*.35f, wh*.65f}
	};
	d->poly(testVerts,5);

	d->setLineWidth(4.f);
	d->setLineColor(255,0,255);
	d->setFillColor(0,0,0,0);
	d->tri({ww*.5f,wh*.39f},{ww*.57f,wh*.62f},{ww*.43f,wh*.58f});

    d->setLineWidth(2.f);
    d->setLineColor(255,255,255);
    d->setFillColor(255,255,255,128);
    d->circle({cursor.x,cursor.y},20.f);
}

int Game::end()
{
	// Ends the game - return code can be used to chuck out error codes if necessary
	this->platform->kill();
	return 0;
}