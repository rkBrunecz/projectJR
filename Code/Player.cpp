#include <SFML\Graphics.hpp>
#include "Player.h"

Player::Player()
{
	x = 960;
	y = 540;

	tmp.setRadius(20);
	tmp.setPosition(x, y);
	tmp.setFillColor(sf::Color::White);
}

void Player::draw(sf::RenderWindow* window)
{
	window->draw(tmp);
}

void Player::updatePosition(sf::RenderWindow* window, float seconds)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		y = y + (-3200 * seconds);
		tmp.setPosition(x, y);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		y = y + (3200 * seconds);
		tmp.setPosition(x, y);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		x = x + (3200 * seconds);
		tmp.setPosition(x, y);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		x = x + (-3200 * seconds);
		tmp.setPosition(x, y);
	}
}