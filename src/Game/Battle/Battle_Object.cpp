#include "Game\Battle\Battle_Object.h"
#include "Game\Game.h"

Battle_Object::Battle_Object(const Battle_Object& b)
{
	jumpVelocity = b.jumpVelocity;
	gravity = b.gravity; 
	movementVelocity = b.movementVelocity;
	battleVelocity = b.battleVelocity;
	
	battleSprite = b.battleSprite;
	
	battleAniClock = b.battleAniClock;
	
	battleHeight = b.battleHeight;
	battleWidth = b.battleWidth;
	
	isAttacking = b.isAttacking;
	isTakingDamage = b.isTakingDamage;
	isCrushable = b.isCrushable;
	isAirBound = b.isAirBound;
	decreaseVelocity = b.decreaseVelocity;

	battlePos = b.battlePos;
	lastBattlePos = b.lastBattlePos;

	// Allocate memory
	moving = new Battle_Animation();
	standing = new Battle_Animation();
	attackStance = new Battle_Animation();
	arielStance = new Battle_Animation();
	returning = new Battle_Animation();
	takingDamage = new Battle_Animation();
	constantTakingDamage = new Battle_Animation();

	if (b.moving != NULL) *moving = *b.moving;
	if (b.standing != NULL) *standing = *b.standing;
	if (b.attackStance != NULL) *attackStance = *b.attackStance;
	if (b.arielStance != NULL) *arielStance = *b.arielStance;
	if (b.returning != NULL) *returning = *b.returning;
	if (b.takingDamage != NULL) *takingDamage = *b.takingDamage;
	if (b.constantTakingDamage != NULL) *constantTakingDamage = *b.constantTakingDamage;
}

const Battle_Object &Battle_Object::operator=(const Battle_Object& b)
{
	// Check if the objects are the same
	if (this == &b)
		return *this;

	jumpVelocity = b.jumpVelocity;
	gravity = b.gravity;
	movementVelocity = b.movementVelocity;
	battleVelocity = b.battleVelocity;

	battleSprite = b.battleSprite;

	battleAniClock = b.battleAniClock;

	battleHeight = b.battleHeight;
	battleWidth = b.battleWidth;

	isAttacking = b.isAttacking;
	isTakingDamage = b.isTakingDamage;
	isCrushable = b.isCrushable;
	isAirBound = b.isAirBound;
	decreaseVelocity = b.decreaseVelocity;

	battlePos = b.battlePos;
	lastBattlePos = b.lastBattlePos;

	// Clear memory
	delete moving;
	delete standing;
	delete attackStance;
	delete arielStance;
	delete returning;
	delete takingDamage;
	delete constantTakingDamage;

	// Allocate memory
	moving = new Battle_Animation();
	standing = new Battle_Animation();
	attackStance = new Battle_Animation();
	arielStance = new Battle_Animation();
	returning = new Battle_Animation();
	takingDamage = new Battle_Animation();
	constantTakingDamage = new Battle_Animation();

	if (b.moving != NULL) *moving = *b.moving;
	if (b.standing != NULL) *standing = *b.standing;
	if (b.attackStance != NULL) *attackStance = *b.attackStance;
	if (b.arielStance != NULL) *arielStance = *b.arielStance;
	if (b.returning != NULL) *returning = *b.returning;
	if (b.takingDamage != NULL) *takingDamage = *b.takingDamage;
	if (b.constantTakingDamage != NULL) *constantTakingDamage = *b.constantTakingDamage;

	return *this;
}

Battle_Object::~Battle_Object()
{
	delete moving; 
	delete standing;
	delete attackStance;
	delete arielStance;
	delete returning;
	delete takingDamage;
	delete constantTakingDamage;
}

void Battle_Object::setVelocity(float x, float y, bool isPlayer)
{
	float distanceX = x - battlePos.x;
	float distanceY = y - battlePos.y;

	if (distanceX < 0)
		battleVelocity.x = distanceX - movementVelocity;
	else
		battleVelocity.x = distanceX + movementVelocity;

	if (distanceY < 0)
		battleVelocity.y = distanceY + (movementVelocity * (distanceY / distanceX));
	else
		battleVelocity.y = distanceY - (movementVelocity * (distanceY / distanceX));

	if (isPlayer)
	{
		if (x < battlePos.x)
			animator.changeBattleAnimation(moving);
		else
			animator.changeBattleAnimation(returning);
	}
	else
	{
		if (x > battlePos.x)
			animator.changeBattleAnimation(moving);
		else
			animator.changeBattleAnimation(returning);
	}
}

void Battle_Object::setSpritePosition(float x, float y)
{
	battlePos = sf::Vector2f(x, y);
	lastBattlePos = battlePos;

	battleSprite.setPosition(x, y);
}

void Battle_Object::takeDamage(int damage, short hitStun)
{
	animator.changeBattleAnimation(takingDamage);
	animator.getCurrentAnimation()->changeInitialFrameDelay(hitStun);
	isTakingDamage = true;
}

void Battle_Object::renderBattlePosition(double alpha)
{
	// Predict the players next position
	sf::Vector2f posInterpolation = sf::Vector2f(float(battlePos.x * alpha + lastBattlePos.x * (1.f - alpha)), float(battlePos.y * alpha + lastBattlePos.y * (1.f - alpha)));

	// Update the position of the camera and character. Do not update the camera position if it is at the end of the map.
	battleSprite.setPosition(posInterpolation);

	switch (battleState)
	{
	case Jumping:
		if (Game::camera->getCenter().y > battlePos.y)
			Game::camera->setCenter(sf::Vector2f(Game::camera->getCenter().x, posInterpolation.y));

		break;

	case LaunchingDown:			
		Game::camera->setCenter(sf::Vector2f(Game::camera->getCenter().x, posInterpolation.y));

		if (Game::camera->getCameraOrigins().y < Game::camera->getCenter().y)
			Game::camera->setCenter(Game::camera->getCameraOrigins());

		break;
	}
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

	lastBattlePos = battlePos;

	if (battlePos.y + (battleVelocity.y * elapsedTime) < y)
		battleVelocity.y = (y - battlePos.y) / elapsedTime;

	if (battlePos.y > y)
		battlePos.y += (battleVelocity.y * elapsedTime);
	else
	{
		isAirBound = true;
		animator.changeBattleAnimation(constantTakingDamage);

		return true;
	}

	return false;
}

bool Battle_Object::launchSpriteDown(float y, float elapsedTime)
{
	battleVelocity.y = SMASH_VELOCITY;

	lastBattlePos = battlePos;

	battleState = LaunchingDown;

	if (battlePos.y + (SMASH_VELOCITY * elapsedTime) > y)
		battleVelocity.y = (y - battlePos.y) / elapsedTime;

	if (battlePos.y < y)
		battlePos.y += (battleVelocity.y * elapsedTime);
	else
	{
		isAirBound = false;
		Game::camera->startCameraShake(1.f);

		battleState = Other;

		return true;
	}

	return false;
}

bool Battle_Object::fall(float y, float elapsedTime)
{
	battleVelocity.y = FALL_VELOCITY;

	lastBattlePos = battlePos;

	if (battlePos.y + (FALL_VELOCITY * elapsedTime) > y)
		battleVelocity.y = (y - battlePos.y) / elapsedTime;

	if (battlePos.y < y)
		battlePos.y += (battleVelocity.y * elapsedTime);
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
	return battlePos;
}

bool Battle_Object::moveToPosition(float x, float y, float elapsedTime, bool movingToAttack)
{
	bool arrived = move(x, y, elapsedTime);

	if (arrived)
	{
		if (movingToAttack)
			animator.changeBattleAnimation(attackStance);
		else
			animator.changeBattleAnimation(standing);
	}

	return arrived;
}

bool Battle_Object::move(float x, float y, float elapsedTime)
{
	lastBattlePos = battlePos;

	if (x <= battlePos.x)
	{
		// If the next update will places the sprite past the position x, set the next step to be enough to get the sprite exactly on the x and y provided
		if (battlePos.x + (battleVelocity.x * elapsedTime) < x)
		{
			battleVelocity.x = (x - battlePos.x) / elapsedTime;
			battleVelocity.y = (y - battlePos.y) / elapsedTime;
		}

		if (battlePos.x > x && animator.getCurrentAnimation()->onLoopFrame(&battleSprite))
			battlePos = sf::Vector2f(battlePos.x + (battleVelocity.x * elapsedTime), battlePos.y + (battleVelocity.y * elapsedTime));
		else if (battlePos.x <= x && animator.getCurrentAnimation()->onFirstFrame(&battleSprite, &battleAniClock))
			return true;
		else if (battlePos.x <= x && !animator.getCurrentAnimation()->isReversing())
			animator.getCurrentAnimation()->reverseAnimation();
	}
	else
	{
		// If the next update will places the sprite past the position x, set the next step to be enough to get the sprite exactly on x and y provided
		if (battlePos.x + (battleVelocity.x * elapsedTime) > x)
		{
			battleVelocity.x = (x - battlePos.x) / elapsedTime;
			battleVelocity.y = (y - battlePos.y) / elapsedTime;
		}

		if (battlePos.x + (battleVelocity.x * elapsedTime) <= x && animator.getCurrentAnimation()->onLoopFrame(&battleSprite))
			battlePos = sf::Vector2f(battlePos.x + (battleVelocity.x * elapsedTime), battlePos.y + (battleVelocity.y * elapsedTime));
		else if (battlePos.x + (battleVelocity.x * elapsedTime) >= x && animator.getCurrentAnimation()->onFirstFrame(&battleSprite, &battleAniClock))
			return true;
		else if (battlePos.x + (battleVelocity.x * elapsedTime) >= x && !animator.getCurrentAnimation()->isReversing())
			animator.getCurrentAnimation()->reverseAnimation();
	}

	return false;
}

bool Battle_Object::jumping(float y, float elapsedTime)
{
	// If the object is still attacking, say they are getting ready to jump.
	if (isAttacking)
		return true;

	lastBattlePos = battlePos;

	// If the objects jump velocity is 0, change their current animation to a jumping animation
	if (jumpVelocity == 0 && battlePos.y > y)
	{
		animator.changeBattleAnimation(moving);
		jumpVelocity = -JUMP_VELOCITY;
		gravity = JUMP_VELOCITY;

		battleState = Jumping;
	}

	/*	
		Determine if the velocity should be decreased. Check to see if the number of updates needed to get to the position y (multiplied by 2 to account for the gravity application when finally applied) is equal to the amount
		of steps gravity will need to reduce the jump velocity to 0.
	*/
	if (((battlePos.y - y) / (abs(jumpVelocity) * elapsedTime)) * 2 <= abs(jumpVelocity) / (gravity * elapsedTime))
		decreaseVelocity = true;
		
	// Ensure the velocity does not drop below -1.0
	if (decreaseVelocity && (jumpVelocity + (gravity * elapsedTime)) * elapsedTime < -1.0f)
		jumpVelocity += (gravity * elapsedTime);

	// if the object has reached the frame of animation where it is considered moving, and the position is greater than the position it is trying to reach, update the camera and object position
	if (battlePos.y > y && animator.getCurrentAnimation()->onLoopFrame(&battleSprite))
		battlePos.y += (jumpVelocity * elapsedTime);
	// The object has finished animating and moving. The object therefore is no longer jumping
	else if (battlePos.y <= y && animator.getCurrentAnimation()->onFirstFrame(&battleSprite, &battleAniClock))
	{
		isAirBound = true;
		jumpVelocity = 0;
		animator.changeBattleAnimation(arielStance);
		decreaseVelocity = false;

		battleState = Other;

		return false;
	} // If the object has reached its destination, reverse the animation
	else if (battlePos.y <= y && !animator.getCurrentAnimation()->isReversing())
	{
		// Set the sprites position to exactly to the y it was moving to.
		battlePos.y = y;
		animator.getCurrentAnimation()->reverseAnimation(); // Reverse the animation
	}

	return true;
}