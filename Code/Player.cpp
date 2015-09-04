/*
Authors: Randy Brunecz, Joshua Gray
Player.h
9/3/2015

This class contains information about the player character such as position, graphic used and
characteristics.
*/


#include <SFML\Graphics.hpp>
#include "Player.h"

/*
constructor

Currently, this is a temporary constructor for the player. Obviously, the character will use a sprite.
*/
Player::Player()
{
	x = 960;
	y = 540;

	tmp.setRadius(20);
	tmp.setPosition(x, y);
	tmp.setFillColor(sf::Color::White);
}

/*
draw
Parameters:
	window: The game window

This method draws the character model on to the games window
*/
void Player::draw(sf::RenderWindow* window)
{
	window->draw(tmp);
}

/*
updatePosition
Parameters:
	window: The game window (NOT SURE WE NEED THIS)
	timeSinceLastFrame: This is the time elapsed between updates.

This method takes player input (if there is any) and moves the player character
in a direction that the player chooses.
*/
void Player::updatePosition(sf::RenderWindow* window, float timeSinceLastFrame)
{
	//Move up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		y = y + (-3200 * timeSinceLastFrame);
		tmp.setPosition(x, y);
	}
	//Move down
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		y = y + (3200 * timeSinceLastFrame);
		tmp.setPosition(x, y);
	}
	//Move right
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		x = x + (3200 * timeSinceLastFrame);
		tmp.setPosition(x, y);
	}
	//Move left
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		x = x + (-3200 * timeSinceLastFrame);
		tmp.setPosition(x, y);
	}
}