#include "Game.h"

sf::Text FontControl::newTextElement(sf::Text textElement, sf::Font font)
{
	textElement.setFont(font);
	textElement.setCharacterSize(20);
	textElement.setColor(sf::Color(255, 255, 255));
	return textElement;
}