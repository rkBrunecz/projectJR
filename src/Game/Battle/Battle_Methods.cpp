#include "Game\Battle\Battle_Methods.h"

bool Battle_Methods::moveToPosition(float x, float y, float elapsedTime, int width, sf::Vector2f& velocity, sf::Sprite* sprite, Battle_Animation* animation, sf::Clock* battleAniClock)
{
	Battle_Methods::Movements direction;

	if (x <= sprite->getPosition().x)
		direction = Battle_Methods::Left;
	else
		direction = Battle_Methods::Right;

	switch (direction)
	{
	case Battle_Methods::Left:
		// If the next update will places the sprite past the position x, set the next step to be enough to get the sprite exactly on x and y provided
		if (sprite->getPosition().x + (velocity.x * elapsedTime) < x)
		{
			velocity.x = (x - sprite->getPosition().x) / elapsedTime;
			velocity.y = (y - sprite->getPosition().y) / elapsedTime;
		}

		if (sprite->getPosition().x > x && animation->onLoopFrame(sprite))
			sprite->setPosition(sprite->getPosition().x + (velocity.x * elapsedTime), sprite->getPosition().y + (velocity.y * elapsedTime));
		else if (sprite->getPosition().x <= x && animation->onFirstFrame(sprite, battleAniClock))
			return true;
		else if (sprite->getPosition().x <= x && !animation->isReversing())
			animation->reverseAnimation();

		break;

	case Battle_Methods::Right:
		// If the next update will places the sprite past the position x, set the next step to be enough to get the sprite exactly on x and y provided
		if (sprite->getPosition().x + (velocity.x * elapsedTime) > x)
		{
			velocity.x = (x - sprite->getPosition().x) / elapsedTime;
			velocity.y = (y - sprite->getPosition().y) / elapsedTime;
		}

		if (sprite->getPosition().x + (velocity.x * elapsedTime) <= x && animation->onLoopFrame(sprite))
			sprite->setPosition(sprite->getPosition().x + (velocity.x * elapsedTime), sprite->getPosition().y + (velocity.y * elapsedTime));
		else if (sprite->getPosition().x + (velocity.x * elapsedTime) >= x && animation->onFirstFrame(sprite, battleAniClock))
			return true;
		else if (sprite->getPosition().x + (velocity.x * elapsedTime) >= x && !animation->isReversing())
			animation->reverseAnimation();

		break;
	}

	return false;
}

sf::Vector2f Battle_Methods::setVelocity(float speed, float x, float y, float spritePosX, float spritePosY)
{
	sf::Vector2f velocity;

	float distanceX = x - spritePosX;
	float distanceY = y - spritePosY;

	if (distanceX < 0)
		velocity.x = distanceX - speed;
	else
		velocity.x = distanceX + speed;

	if (distanceY < 0)
		velocity.y = distanceY + (speed * (distanceY / distanceX));
	else
		velocity.y = distanceY - (speed * (distanceY / distanceX));
	
	return velocity;
}