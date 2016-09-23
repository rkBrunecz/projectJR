#include "Game\Battle\Attack.h"
#include "Game\Game.h"

Attack::Attack(short numFrames, short frameDelay, short finalFrameDelay, short activeFrame, short hitStun, short specialHitStun, int damage, int height, int width, int top, int left, bool isLauncher, bool meteorSmash, pb::Audio *sound)
{
	this->numFrames = numFrames - 1;
	this->frameDelay = frameDelay;
	this->finalFrameDelay = finalFrameDelay;
	this->activeFrame = activeFrame;
	this->hitStun = hitStun;
	this->specialHitStun = specialHitStun;
	this->damage = damage;
	this->height = height;
	this->width = width;
	this->top = top;	
	this->defaultLeft = left;
	this->isLauncher = isLauncher;
	this->meteorSmash = meteorSmash;

	this->left = defaultLeft;
	this->activeFrameDelay = hitStun;

	soundId = Game::audioManager->addSound(sound);
}

Attack::Attack(short numFrames, short frameDelay, short finalFrameDelay, short activeFrame, short hitStun, short specialHitStun, int damage, int height, int width, int top, int left, bool isLauncher, bool meteorSmash, pb::Audio *sound, pb::Audio *specialSound) :
Attack(numFrames, frameDelay, finalFrameDelay, activeFrame, hitStun, specialHitStun, damage, height, width, top, left, isLauncher, meteorSmash, sound)
{
	specialSoundId = Game::audioManager->addSound(specialSound);
}

/*
animateAttack
Parameters:
	animationClock: Clock used to determine if the animation should move to the next frame
	sprite: The sprite where the attack animation is contained
	returnToDefault: Determines if the sprite should return to the default frame (used to add "oomph" to the last attack in a string, for example).

This method simply animates the attack. It returns true if the attack is still in the process of animating, false if the attack has
finished animating.
*/
bool Attack::animateAttack(sf::Sprite* sprite, sf::Clock* clock, bool reset)
{
	int spriteLeft = sprite->getTextureRect().left;

	if (reset)
	{
		resetAnimation(sprite, clock);
		return true;
	}

	if (spriteLeft / width < numFrames && spriteLeft / width != activeFrame &&
		clock->getElapsedTime().asMilliseconds() > frameDelay)
		advanceFrame(sprite, clock);
	else if (spriteLeft / width == activeFrame &&
		clock->getElapsedTime().asMilliseconds() > activeFrameDelay)
	{
		if (activeFrameDelay == specialHitStun)
			Game::camera->emphasisZoomOut();

		advanceFrame(sprite, clock);
	}
	else if (spriteLeft / width == numFrames && 
		clock->getElapsedTime().asMilliseconds() > finalFrameDelay)
	{
		clock->restart();
		left = defaultLeft;
		activeFrameDelay = hitStun;

		return false;
	}

	return true;
}

void Attack::resetAnimation(sf::Sprite* sprite, sf::Clock* clock)
{
	sprite->setTextureRect(sf::IntRect(defaultLeft, top, width, height));
	clock->restart();
	
	left = defaultLeft + width;
	activeFrameDelay = hitStun;
}

void Attack::advanceFrame(sf::Sprite* sprite, sf::Clock* clock)
{
	sprite->setTextureRect(sf::IntRect(left, top, width, height));
	left += width;

	clock->restart();
}

bool Attack::inActiveFrame(sf::Sprite* sprite)
{
	if (sprite->getTextureRect().top == top && sprite->getTextureRect().left / width == activeFrame)
		return true;

	return false;
}

bool Attack::isALauncher()
{
	return isLauncher;
}

bool Attack::isMeteorSmash()
{
	return meteorSmash;
}

bool Attack::pastActiveFrame(sf::Sprite* sprite)
{
	if (sprite->getTextureRect().top == top && sprite->getTextureRect().left / width > activeFrame)
		return true;

	return false;
}

int Attack::getDamage(bool enemyInCrushState)
{
	if (enemyInCrushState)
		return damage;
	else
		return damage * 3;
}

short Attack::getHitStun(sf::Clock* clock, sf::Sprite* sprite, bool enemyInCrushState)
{
	if (isLauncher && enemyInCrushState)
	{
		activeFrameDelay = specialHitStun;
		Game::camera->emphasisZoomIn(sprite->getPosition().x, sprite->getPosition().y, 0.25f);

		if (specialSoundId != -1)
			Game::audioManager->playSound(specialSoundId);
		else
			Game::audioManager->playSound(soundId);

		return specialHitStun - clock->getElapsedTime().asMilliseconds();
	}
	else 
	{
		Game::audioManager->playSound(soundId);
		activeFrameDelay = hitStun;
		return hitStun - clock->getElapsedTime().asMilliseconds();
	}
}