#include "../HeaderFiles/Game.h"

int main()
{
	unsigned int width = 1280;
	unsigned int height = 720;

	sf::RenderWindow App(sf::VideoMode(width, height), "RPG Game");

	App.setFramerateLimit(120);

	Game g(&App);
	g.Init();

	while (App.isOpen())
	{
		g.Play();
	}
	return EXIT_SUCCESS;
}