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
	battleVelocity = Battle_Methods::setVelocity(movementVelocity, x, y, battleSprite.getPosition().x, battleSprite.getPosition().y);

	if (isPlayer)
	{
		if (x < battleSprite.getPosition().x)
			animator.changeBattleAnimation(moving);
		else
			animator.changeBattleAnimation(returning);
	}
	else
	{
		if (x > battleSprite.getPosition().x)
			animator.changeBattleAnimation(moving);
		else
			animator.changeBattleAnimation(returning);
	}
}

void Battle_Object::setSpritePosition(float x, float y)
{
	battleSprite.setPosition(x, y);
}

void Battle_Object::takeDamage(int damage, short hitStun)
{
	animator.changeBattleAnimation(takingDamage);
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
		animator.changeBattleAnimation(constantTakingDamage);
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

		if (Game::camera->getCameraOrigins().y > Game::camera->getCamera().getCenter().y + (battleVelocity.y * elapsedTime))
			Game::camera->move(sf::Vector2f(0, battleVelocity.y * elapsedTime));
		else
			Game::camera->setCenter(Game::camera->getCameraOrigins().x, Game::camera->getCameraOrigins().y);
	}
	else
	{
		isAirBound = false;
		Game::camera->startCameraShake(1.0f);
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
			animator.changeBattleAnimation(attackStance);
		else
			animator.changeBattleAnimation(standing);
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
		animator.changeBattleAnimation(moving);
		jumpVelocity = -JUMP_VELOCITY;
		gravity = JUMP_VELOCITY;
	}

	/*	
		Determine if the velocity should be decreased. Check to see if the number of updates needed to get to the position y (multiplied by 2 to account for the gravity application when finally applied) is equal to the amount
		of steps gravity will need to reduce the jump velocity to 0.
	*/
	if (((battleSprite.getPosition().y - y) / (abs(jumpVelocity) * elapsedTime)) * 2 <= abs(jumpVelocity) / (gravity * elapsedTime))
		decreaseVelocity = true;
		
	// Ensure the velocity does not drop below -1.0
	if (decreaseVelocity && (jumpVelocity + (gravity * elapsedTime)) * elapsedTime < -1.0f)
		jumpVelocity += (gravity * elapsedTime);

	// if the object has reached the frame of animation where it is considered moving, and the position is greater than the position it is trying to reach, update the camera and object position
	if (battleSprite.getPosition().y > y && animator.getCurrentAnimation()->onLoopFrame(&battleSprite))
	{
		battleSprite.setPosition(battleSprite.getPosition().x, battleSprite.getPosition().y + (jumpVelocity * elapsedTime));

		if (Game::camera->getCamera().getCenter().y > battleSprite.getPosition().y)
			Game::camera->move(sf::Vector2f(0, jumpVelocity * elapsedTime));
	} // The object has finished animating and moving. The object therefore is no longer jumping
	else if (battleSprite.getPosition().y <= y && animator.getCurrentAnimation()->onFirstFrame(&battleSprite, &battleAniClock))
	{
		isAirBound = true;
		jumpVelocity = 0;
		animator.changeBattleAnimation(arielStance);
		decreaseVelocity = false;

		return false;
	} // If the object has reached its destination, reverse the animation
	else if (battleSprite.getPosition().y <= y && !animator.getCurrentAnimation()->isReversing())
	{
		// Set the sprites position to exactly to the y it was moving to.
		battleSprite.setPosition(battleSprite.getPosition().x, y);
		animator.getCurrentAnimation()->reverseAnimation(); // Reverse the animation
	}

	return true;
}