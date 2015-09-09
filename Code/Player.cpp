/*
Authors: Randy Brunecz, Joshua Gray
Player.h
9/3/2015

This class contains information about the player character such as position, graphic used and
characteristics.
*/

#include <SFML\Graphics.hpp>
#include "Player.h"
#include "Animation.h"
/*
constructor
Parameters:
	window: The game window. Used to get the window size.

Creates the main characters sprite and sets other important information.
*/
Player::Player(sf::RenderWindow* window)
{
	//Set the start position for the character sprite
	x = window->getSize().x / 2;
	y = window->getSize().y / 2;

	//Load sprite map
	if (!spriteMap.loadFromFile("bin/Graphics/test.png"))
		exit(EXIT_FAILURE);

	//Set up the sprites properties
	character.sprite.setTexture(spriteMap);
	character.sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
	character.sprite.setOrigin(16, 16);

	//Set up the sprites shadow properties
	character.shadow.setRadius(8);
	character.shadow.setFillColor(sf::Color(0, 0, 0, 75));
	character.shadow.setOrigin(8, -4);

	character.setPosition(x, y);

	//Set up important properties of the camera
	camera.setDisplay(window->getSize().x, window->getSize().y);
}

/*
draw
Parameters:
	window: The game window

This method draws the character model on to the games window
*/
void Player::draw(sf::RenderWindow* window)
{
	window->draw(character.shadow);
	window->draw(character.sprite);
}

/*
updatePosition
Parameters:
	window: The game window (NOT SURE WE NEED THIS)
	timeSinceLastFrame: This is the time elapsed between updates.

This method takes player input (if there is any) and moves the player character
in a direction that the player chooses.
*/
void Player::updatePosition(sf::RenderWindow* window)
{
	//LOCAL VARIABLES
	bool positionUpdated = true;

	//Move up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		y -= VELOCITY;		
		camera.updateCameraPosition(0, -VELOCITY, x, y);

		currentDirection = Animation::Up; //Set the character direction state for animation purposes
	}
	//Move down
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		y += VELOCITY;
		camera.updateCameraPosition(0, VELOCITY, x, y);
			
		currentDirection = Animation::Down; //Set the character direction state for animation purposes
	}
	//Move right
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		x += VELOCITY;
		camera.updateCameraPosition(VELOCITY, 0, x, y);

		currentDirection = Animation::Right; //Set the character direction state for animation purposes
	}
	//Move left
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		x -= VELOCITY;
		camera.updateCameraPosition(-VELOCITY, 0, x, y);

		currentDirection = Animation::Left; //Set the character direction state for animation purposes
	}
	else
		positionUpdated = false; //If the position was not updated, positionUpdated = false
	
	bool pastCameraBoundaryX = camera.pastCameraBoundaryX(x), 
		 pastCameraBoundaryY = camera.pastCameraBoundaryY(y);

	if (pastCameraBoundaryX && pastCameraBoundaryY)
		character.setPosition(x - camera.getXPos(), y - camera.getYPos());
	else if (pastCameraBoundaryX)
		character.setPosition(x - camera.getXPos(), window->getSize().y / 2);
	else if (pastCameraBoundaryY)
		character.setPosition(window->getSize().x / 2, y - camera.getYPos());

	Animation::updateAnimation(positionUpdated, currentDirection, &characterAnimation, &character.sprite); //Update the characters movement animation
}

/*
setColor
Parameters:
	r: The red scaling on the rgb scale
	g: The green scaling on the rgb scale
	b: The blue scaling on the rgb scale
	a: The alpha scaling

This method sets the color and transparency of a sprite
*/
void Player::setColor(int r, int g, int b, int a)
{
	character.sprite.setColor(sf::Color(r, g, b, a));
}