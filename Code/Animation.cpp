/*
Animation.cpp
This class handles animations for various sprites.

@author Randall Brunecz
@version 1.0 9/4/2015
*/

#include "Animation.h"

/*
updateAnimation
Parameters:
	positionUpdated: If this is false, the character will return to a standing position. If true, have the sprite cycle through the walk animations
	currentDirection: This specifies the current direction the character is facing
	animationClock: This clock is used to animate the sprite fluidly
	sprite: This pointer contians information about the sprite that will be updated

This method updates the characters animation based on the direction the character is moving in
*/
void Animation::updateAnimation(bool positionUpdated, WalkingDirection currentDirection, sf::Clock* animationClock, sf::Sprite* sprite)
{
	switch (currentDirection)
	{
	case Down:
		if (animationClock->getElapsedTime() > sf::seconds(0.2f))
		{
			//If position has not been updated, return character to standing animation facing downward
			if (!positionUpdated)
				sprite->setTextureRect(sf::IntRect(32, 0, 32, 32));
			else if (sprite->getTextureRect().left == 64) //Shuffle walk animation
				sprite->setTextureRect(sf::IntRect(0, 0, 32, 32));
			else //Shuffle walk animation
				sprite->setTextureRect(sf::IntRect(64, 0, 32, 32));

			animationClock->restart(); //Restart the character animation clock
		}
		//This insures that the character animation is updated in a timely matter in the event the elapsed time is less that 0.2f and the direction the character is facing has changed
		else if (sprite->getTextureRect().top != 0)
			sprite->setTextureRect(sf::IntRect(0, 0, 32, 32));

		break;

	case Left:
		if (animationClock->getElapsedTime() > sf::seconds(0.2f))
		{
			//If position has not been updated, return character to standing animation facing left
			if (!positionUpdated)
				sprite->setTextureRect(sf::IntRect(32, 32, 32, 32));
			else if (sprite->getTextureRect().left == 64) //Shuffle walk animation
				sprite->setTextureRect(sf::IntRect(0, 32, 32, 32));
			else //Shuffle walk animation
				sprite->setTextureRect(sf::IntRect(64, 32, 32, 32));

			animationClock->restart(); //Restart the character animation clock
		}
		//This insures that the character animation is updated in a timely matter in the event the elapsed time is less that 0.2f and the direction the character is facing has changed
		else if (sprite->getTextureRect().top != 32)
			sprite->setTextureRect(sf::IntRect(0, 32, 32, 32));

		break;

	case Right:
		if (animationClock->getElapsedTime() > sf::seconds(0.2f))
		{
			//If position has not been updated, return character to standing animation facing right
			if (!positionUpdated)
				sprite->setTextureRect(sf::IntRect(32, 64, 32, 32));
			else if (sprite->getTextureRect().left == 64) //Shuffle walk animation
				sprite->setTextureRect(sf::IntRect(0, 64, 32, 32));
			else //Shuffle walk animation
				sprite->setTextureRect(sf::IntRect(64, 64, 32, 32));

			animationClock->restart(); //Restart the character animation clock
		}
		//This insures that the character animation is updated in a timely matter in the event the elapsed time is less that 0.2f and the direction the character is facing has changed
		else if (sprite->getTextureRect().top != 64)
			sprite->setTextureRect(sf::IntRect(0, 64, 32, 32));

		break;

	case Up:
		if (animationClock->getElapsedTime() > sf::seconds(0.2f))
		{
			//If position has not been updated, return character to standing animation facing upward
			if (!positionUpdated)
				sprite->setTextureRect(sf::IntRect(32, 96, 32, 32));
			else if (sprite->getTextureRect().left == 64) //Shuffle walk animation
				sprite->setTextureRect(sf::IntRect(0, 96, 32, 32));
			else //Shuffle walk animation
				sprite->setTextureRect(sf::IntRect(64, 96, 32, 32));

			animationClock->restart(); //Restart the character animation clock
		}
		//This insures that the character animation is updated in a timely matter in the event the elapsed time is less that 0.2f and the direction the character is facing has changed
		else if (sprite->getTextureRect().top != 96)
			sprite->setTextureRect(sf::IntRect(0, 96, 32, 32));

		break;
	}
}

void Animation::updateWaterAnimation(WaterDirection* direction, sf::Clock* waterAnimation, sf::Sprite* water, sf::RenderTexture* frames, unsigned short* currentFrame, unsigned short numFrames)
{
	if (waterAnimation->getElapsedTime() < sf::seconds(0.3f))
		return;
	
	waterAnimation->restart();

	switch (*direction)
	{
	case ShiftLeft:
		*currentFrame = *currentFrame - 1;

		water->setTexture(frames[*currentFrame].getTexture());

		if (*currentFrame == 0)
			*direction = ShiftRight;

		break;

	case ShiftRight:
		*currentFrame = *currentFrame + 1;

		water->setTexture(frames[*currentFrame].getTexture());

		if (*currentFrame == numFrames - 1)
			*direction = ShiftLeft;

		break;
	}
}