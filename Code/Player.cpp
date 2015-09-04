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
Parameters:
	window: The game window. Used to get the window size.

Currently, this is a temporary constructor for the player. Obviously, the character will use a sprite.
*/
Player::Player(sf::RenderWindow* window)
{
	//Set the start position for the character sprite
	x = window->getSize().x / 2;
	y = window->getSize().y / 2;

	currentDirection = Down;

	//Load sprite map
	if (!spriteMap.loadFromFile("bin/Graphics/test.png"))
		exit(EXIT_FAILURE);

	//Set up the sprites properties
	sprite.setTexture(spriteMap);
	sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
	sprite.setOrigin(16, 16);
	sprite.setPosition(x, y);
}

/*
draw
Parameters:
	window: The game window

This method draws the character model on to the games window
*/
void Player::draw(sf::RenderWindow* window)
{
	window->draw(sprite);
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
	//LOCAL VARIABLES
	bool positionUpdated = true;

	//Move up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		y -= VELOCITY;
		sprite.setPosition(x, y);

		currentDirection = Up; //Set the character direction state for animation purposes
	}
	//Move down
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		y += VELOCITY;
		sprite.setPosition(x, y);

		currentDirection = Down; //Set the character direction state for animation purposes
	}
	//Move right
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		x += VELOCITY;
		sprite.setPosition(x, y);

		currentDirection = Right; //Set the character direction state for animation purposes
	}
	//Move left
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		x -= VELOCITY;
		sprite.setPosition(x, y);

		currentDirection = Left; //Set the character direction state for animation purposes
	}
	else
		positionUpdated = false; //If the position was not updated, positionUpdated = false

	updateAnimation(positionUpdated); //Update the characters movement animation
}

/*
updateAnimation
Parameters:
	positionUpdated: This determines on the next animation update if the character should return to standing position or not.

This method updates the characters animation based on the direction the character is moving in
*/
void Player::updateAnimation(bool positionUpdated)
{
	switch (currentDirection)
	{
	case Down:
		if (characterAnimation.getElapsedTime() > sf::seconds(0.2f))
		{
			//If position has not been updated, return character to standing animation facing downward
			if (!positionUpdated)
				sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
			else if (sprite.getTextureRect().left == 64) //Shuffle walk animation
				sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
			else //Shuffle walk animation
				sprite.setTextureRect(sf::IntRect(64, 0, 32, 32));

			characterAnimation.restart(); //Restart the character animation clock
		}
		//This insures that the character animation is updated in a timely matter in the event the elapsed time is less that 0.2f
		else if (sprite.getTextureRect().top != 0)
			sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

		break;

	case Left:
		if (characterAnimation.getElapsedTime() > sf::seconds(0.2f))
		{
			//If position has not been updated, return character to standing animation facing left
			if (!positionUpdated)
				sprite.setTextureRect(sf::IntRect(32, 32, 32, 32));
			else if (sprite.getTextureRect().left == 64) //Shuffle walk animation
				sprite.setTextureRect(sf::IntRect(0, 32, 32, 32));
			else //Shuffle walk animation
				sprite.setTextureRect(sf::IntRect(64, 32, 32, 32));

			characterAnimation.restart(); //Restart the character animation clock
		}
		//This insures that the character animation is updated in a timely matter in the event the elapsed time is less that 0.2f
		else if (sprite.getTextureRect().top != 32)
			sprite.setTextureRect(sf::IntRect(0, 32, 32, 32));

		break;

	case Right:
		if (characterAnimation.getElapsedTime() > sf::seconds(0.2f))
		{
			//If position has not been updated, return character to standing animation facing right
			if (!positionUpdated)
				sprite.setTextureRect(sf::IntRect(32, 64, 32, 32));
			else if (sprite.getTextureRect().left == 64) //Shuffle walk animation
				sprite.setTextureRect(sf::IntRect(0, 64, 32, 32));
			else //Shuffle walk animation
				sprite.setTextureRect(sf::IntRect(64, 64, 32, 32));

			characterAnimation.restart(); //Restart the character animation clock
		}
		//This insures that the character animation is updated in a timely matter in the event the elapsed time is less that 0.2f
		else if (sprite.getTextureRect().top != 64)
			sprite.setTextureRect(sf::IntRect(0, 64, 32, 32));

		break;

	case Up:
		if (characterAnimation.getElapsedTime() > sf::seconds(0.2f))
		{
			//If position has not been updated, return character to standing animation facing upward
			if (!positionUpdated)
				sprite.setTextureRect(sf::IntRect(32, 96, 32, 32));
			else if (sprite.getTextureRect().left == 64) //Shuffle walk animation
				sprite.setTextureRect(sf::IntRect(0, 96, 32, 32));
			else //Shuffle walk animation
				sprite.setTextureRect(sf::IntRect(64, 96, 32, 32));

			characterAnimation.restart(); //Restart the character animation clock
		}
		//This insures that the character animation is updated in a timely matter in the event the elapsed time is less that 0.2f
		else if (sprite.getTextureRect().top != 96)
			sprite.setTextureRect(sf::IntRect(0, 96, 32, 32));

		break;
	}
}