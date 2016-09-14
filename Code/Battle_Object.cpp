#include "Battle_Object.h"

void Battle_Object::setVelocity(float x, float y, bool isPlayer)
{
	battleVelocity = Battle_Methods::setVelocity(movementVelocity, x, y, battleSprite.getPosition().x, battleSprite.getPosition().y);

	if (isPlayer)
	{
		if (x < battleSprite.getPosition().x)
			animator.changeBattleAnimation(&moving);
		else
			animator.changeBattleAnimation(&returning);
	}
	else
	{
		if (x > battleSprite.getPosition().x)
			animator.changeBattleAnimation(&moving);
		else
			animator.changeBattleAnimation(&returning);
	}
}

void Battle_Object::setSpritePosition(float x, float y)
{
	battleSprite.setPosition(x, y);
}

void Battle_Object::takeDamage(int damage, short hitStun)
{
	animator.changeBattleAnimation(&takingDamage);
	animator.getCurrentAnimation()->changeInitialFrameDelay(hitStun);
	isTakingDamage = true;
}

bool Battle_Object::inActiveFrame()
{
	if (isAttacking)
		return animator.getCurrentAttack()->inActiveFrame(&battleSprite);

	return false;
}

bool Battle_Object::attackInProgress()
{
	return isAttacking;
}


bool Battle_Object::inCrushState()
{
	bool retVal = isCrushable;

	if (isCrushable)
		isCrushable = false;

	return retVal;
}

bool Battle_Object::attackIsLauncher()
{
	return animator.getCurrentAttack()->isALauncher();
}

bool Battle_Object::attackIsMeteorSmash()
{
	return animator.getCurrentAttack()->isMeteorSmash();
}

bool Battle_Object::pastActiveFrame()
{
	return animator.getCurrentAttack()->pastActiveFrame(&battleSprite);
}

bool Battle_Object::launchSprite(float y, float elapsedTime)
{
	battleVelocity.y = LAUNCH_VELOCITY;

	if (battleSprite.getPosition().y + (battleVelocity.y * elapsedTime) < y)
		battleVelocity.y = (y - battleSprite.getPosition().y) / elapsedTime;

	if (battleSprite.getPosition().y > y)
		battleSprite.setPosition(battleSprite.getPosition().x, battleSprite.getPosition().y + (battleVelocity.y * elapsedTime));
	else
	{
		isAirBound = true;
		return true;
	}

	return false;
}

bool Battle_Object::launchSpriteDown(float y, float elapsedTime)
{
	battleVelocity.y = SMASH_VELOCITY;

	if (battleSprite.getPosition().y + (SMASH_VELOCITY * elapsedTime) > y)
		battleVelocity.y = (y - battleSprite.getPosition().y) / elapsedTime;

	if (battleSprite.getPosition().y < y)
	{
		battleSprite.setPosition(battleSprite.getPosition().x, battleSprite.getPosition().y + (battleVelocity.y * elapsedTime));
		Camera::move(0, battleVelocity.y * elapsedTime);
	}
	else
	{
		isAirBound = false;
		Camera::startCameraShake();
		return true;
	}

	return false;
}

bool Battle_Object::fall(float y, float elapsedTime)
{
	battleVelocity.y = FALL_VELOCITY;

	if (battleSprite.getPosition().y + (FALL_VELOCITY * elapsedTime) > y)
		battleVelocity.y = (y - battleSprite.getPosition().y) / elapsedTime;

	if (battleSprite.getPosition().y < y)
		battleSprite.setPosition(battleSprite.getPosition().x, battleSprite.getPosition().y + (battleVelocity.y * elapsedTime));
	else
	{
		isAirBound = false;
		return true;
	}

	return false;
}

// Gets the damage of the currently executing attack
int Battle_Object::getDamage(bool enemyInCrushState)
{
	return animator.getCurrentAttack()->getDamage(enemyInCrushState);
}

short Battle_Object::getHitStun(bool enemyInCrushState)
{
	return animator.getCurrentAttack()->getHitStun(&battleAniClock, &battleSprite, enemyInCrushState);
}

sf::Vector2i Battle_Object::getDimensions()
{
	return sf::Vector2i(battleWidth, battleHeight);
}

sf::Vector2f Battle_Object::getPosition()
{
	return battleSprite.getPosition();
}

bool Battle_Object::moveToPosition(float x, float y, float elapsedTime, bool movingToAttack)
{
	bool arrived = Battle_Methods::moveToPosition(x, y, elapsedTime, battleWidth / 2, battleVelocity, &battleSprite, animator.getCurrentAnimation(), &battleAniClock);

	if (arrived)
	{
		if (movingToAttack)
			animator.changeBattleAnimation(&attackStance);
		else
			animator.changeBattleAnimation(&standing);
	}

	return arrived;
}

bool Battle_Object::jumping(float y, float elapsedTime)
{
	// If the object is still attacking, say they are getting ready to jump.
	if (isAttacking)
		return true;

	// If the objects jump velocity is 0, change their current animation to a jumping animation
	if (jumpVelocity == 0 && battleSprite.getPosition().y > y)
	{
		animator.changeBattleAnimation(&moving);
		jumpVelocity = -JUMP_VELOCITY;
	}

	if (battleSprite.getPosition().y - y < 20 && battleSprite.getPosition().y - y > 2)
		jumpVelocity = (((y * y) - (battleSprite.getPosition().y * battleSprite.getPosition().y)) / 20);

	// If the next update will overshoot the objects target destination, reduce the next velocity update to place the object at the appropriate position
	if (battleSprite.getPosition().y + (jumpVelocity * elapsedTime) < y)
		jumpVelocity = (y - battleSprite.getPosition().y) / elapsedTime;

	// if the object has reached the frame of animation where it is considered moving, and the position is greater than the position it is trying to reach, update the camera and object position
	if (battleSprite.getPosition().y > y && animator.getCurrentAnimation()->onLoopFrame(&battleSprite))
	{
		battleSprite.setPosition(battleSprite.getPosition().x, battleSprite.getPosition().y + (jumpVelocity * elapsedTime));

		if (Camera::getCamera().getCenter().y > battleSprite.getPosition().y)
			Camera::move(0, (jumpVelocity * elapsedTime));
	} // The object has finished animating and moving. The object therefore is no longer jumping
	else if (battleSprite.getPosition().y <= y && animator.getCurrentAnimation()->onFirstFrame(&battleSprite, &battleAniClock))
	{
		isAirBound = true;
		jumpVelocity = 0;
		animator.changeBattleAnimation(&arielStance);

		return false;
	} // If the object has reached its destination, reverse the animation
	else if (battleSprite.getPosition().y <= y && !animator.getCurrentAnimation()->isReversing())
		animator.getCurrentAnimation()->reverseAnimation();

	return true;
}