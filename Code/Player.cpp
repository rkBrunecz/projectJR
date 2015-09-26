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
#include "Collision.h"
#include "Map.h"

/*
constructor
Parameters:
	window: The game window. Used to get the window size.

Creates the main characters sprite and sets other important information.
*/
Player::Player(sf::RenderWindow* window, Camera* camera)
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
	camera->setCenter(x, y);
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
	window: The game window 

This method takes player input (if there is any) and moves the player character
in a direction that the player chooses.
*/
void Player::updatePosition(sf::RenderWindow* window, Camera* camera)
{
	//LOCAL VARIABLES
	bool positionUpdated = true;
	float offSetX = 0, offSetY = 0;

	//Move up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		offSetY = -VELOCITY;

		currentDirection = Animation::Up; //Set the character direction state for animation purposes
	}
	//Move down
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		offSetY = VELOCITY;
			
		currentDirection = Animation::Down; //Set the character direction state for animation purposes
	}	
	//Move right
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		offSetX = VELOCITY;

		currentDirection = Animation::Right; //Set the character direction state for animation purposes
	}
	//Move left
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		offSetX = -VELOCITY;

		currentDirection = Animation::Left; //Set the character direction state for animation purposes
	}
	else
		positionUpdated = false; //If the position was not updated, positionUpdated = false

	//May seem unintuitive to place y first then x. Think of it as y = rows and x = columns
	if (positionUpdated && Collision::collisionDetected(&sf::IntRect(x + offSetX, y + offSetY, 16, 16)))
	{
		Animation::updateAnimation(false, currentDirection, &characterAniClock, &character.sprite); //Update the characters movement animation
		return;
	}

	//Update positions
	x += offSetX;
	y += offSetY;

	//Update the position of the camera and character. Do not update the camera position if it is at the end of the map.
	character.setPosition(x, y);
	camera->updatePosition(sf::Vector2i(x, y));
	
	Animation::updateAnimation(positionUpdated, currentDirection, &characterAniClock, &character.sprite); //Update the characters movement animation
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

/*
setPlayerPosition
Parameters:
	coords: These are the coordinates that the players position will be set to

This updates the players position in the game world.
*/
void Player::setPlayerPosition(sf::Vector2i coords)
{
	x = coords.x;
	y = coords.y;

	character.setPosition(x, y);

	Animation::updateAnimation(false, currentDirection, &characterAniClock, &character.sprite); //Update the characters animation
}

/*
getPlayerCoordinates
return:
	This method returns the players current position in the game world

This method simply stores the players position in a vector and returns it to the calling method.
*/
sf::Vector2i Player::getPlayerCoordinates()
{
	return sf::Vector2i(x, y);
}