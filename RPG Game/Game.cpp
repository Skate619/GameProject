#include "Game.h"

Game::Game(sf::RenderWindow* GameWindow) : GameWindow(GameWindow)
{

}

void Game::Init()
{
	arial.loadFromFile("arial.ttf");
	help.setFont(arial);
	help.setCharacterSize(20);
	help.setColor(sf::Color(255, 255, 255));
	playerName.setFont(arial);
	playerName.setCharacterSize(20);
	playerName.setColor(sf::Color(255, 255, 255));
}

void Game::Draw()
{
	GameWindow->clear(sf::Color::Black);
	GameWindow->setView(GameWindow->getDefaultView());
}

void Game::UserInput()
{
	while (GameWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			GameWindow->close();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && Enter != true)
		{
			Enter = true;
			string tempString = InputName(GameWindow);
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && Enter == true)
			Enter = false;
	}
}

void Game::Update()
{

}

void Game::Play()
{
	Draw();
	UserInput();
	Update();
	GameWindow->display();
}

string Game::GetText(sf::Event event, string myString)
{
	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode < 128 && event.text.unicode != 13)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
			{
				if (myString.size() > 0)
					myString.erase(myString.size() - 1, 2);
			}
			else
			{
				if (event.text.unicode == 32)
					myString = myString + static_cast<char>(' ');
				myString = myString + static_cast<char>(event.text.unicode);
			}
		}
	}
	return myString;
}

string Game::InputName(sf::RenderWindow* app)
{
	GameWindow = app;
	name = false;
	myString.clear();
	while (!name)
	{
		while (GameWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				GameWindow->close();
			}

			myString = GetText(event, myString);

			playerName.setString(myString);
			inputWidth = playerName.getGlobalBounds().width;
			playerName.setPosition((GameWindow->getSize().x - inputWidth) / 2, GameWindow->getSize().y / 2);

			help.setString("Enter Name");
			helpWidth = help.getGlobalBounds().width;
			help.setPosition((GameWindow->getSize().x - helpWidth) / 2, GameWindow->getSize().y / 4);

			GameWindow->clear(sf::Color::Black);
			GameWindow->setView(GameWindow->getDefaultView());
			GameWindow->draw(playerName);
			GameWindow->draw(help);
			GameWindow->display();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && Enter != true)
		{
			name = true;
			Enter = true;
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && Enter == true)
			Enter = false;
	}
	playerName.setString(myString);
	return myString;
}

void Game::Reverse(string myString)
{
	for (int i = 0; i <= ((myString.size() - 1) / 2); ++i)
	{
		char a = myString[i];
		char b = myString[(myString.size() - 1) - i];
		myString[i] = b;
		myString[(myString.size() - 1) - i] = a;
	}
	cout << "\n";
}

void Game::FizzBuzz()
{
	for (int i = 1; i < 101; ++i)
	{
		if ((i % 3 == 0) && (i % 5 == 0))
		{
			cout << "\nFizzBuzz" << i;
		}
		else if (i % 3 == 0)
			cout << "\nFizz: " << i;
		else if (i % 5 == 0)
			cout << "\nBuzz: " << i;
		else
			cout << "\n" << i;
	}
	system("pause");
}