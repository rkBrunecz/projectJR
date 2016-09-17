#include "Battle_Animation.h"

void Battle_Animation::initializeAnimationData(short numFrames, short frameDelay, short finalFrameDelay, short initialFrameDelay, short startLoopFrame, short finalLoopFrame,
	int height, int width, int top, int defaultLeft, bool loopOnce)
{
	this->numFrames = numFrames - 1;
	this->frameDelay = frameDelay;
	this->finalFrameDelay = finalFrameDelay;
	this->initialFrameDelay = initialFrameDelay;
	this->startLoopFrame = startLoopFrame - 1;
	this->finalLoopFrame = finalLoopFrame - 1;
	this->height = height;
	this->width = width;
	this->top = top;
	this->defaultLeft = defaultLeft;
	this->loopOnce = loopOnce;

	this->left = defaultLeft;
}

void Battle_Animation::animate(sf::Sprite* sprite, sf::Clock* clock, bool reset)
{
	if (reset || sprite->getTextureRect().top != top)
	{
		resetAnimation(sprite, clock);
		return;
	}

	if (loopOnce && counter > 0 && sprite->getTextureRect().left == defaultLeft)
		return;

	if (numFrames == 0)
		return;

	int spriteLeft = sprite->getTextureRect().left;

	if (spriteLeft >= defaultLeft + width &&
		spriteLeft < width * numFrames &&
		clock->getElapsedTime().asMilliseconds() > frameDelay)
		nextFrame(sprite, clock);
	// Check if the initialFrameDelay should be applied
	else if (spriteLeft == defaultLeft && 
		clock->getElapsedTime().asMilliseconds() > initialFrameDelay)
		nextFrame(sprite, clock);
	// Check if the finalFrameDelay should be applied
	else if (spriteLeft == width * numFrames &&
		clock->getElapsedTime().asMilliseconds() > finalFrameDelay)
		nextFrame(sprite, clock);
}

void Battle_Animation::nextFrame(sf::Sprite* sprite, sf::Clock* clock)
{
	// Update the texture rect and restart the animation clock.
	sprite->setTextureRect(sf::IntRect(left, top, width, height));
	clock->restart();

	// Incrememnt the counter if the animation only loops once
	if (loopOnce)
		counter++;

	// if there is only one loop frame and the animation is not returning to the first frame.
	if (!reverse && startLoopFrame == finalLoopFrame && sprite->getTextureRect().left == width * startLoopFrame)
		return;

	switch (state)
	{
	case Forward:
		left += width;
		if (left == width * finalLoopFrame)
			state = Backward;

		break;

	case Backward:
		left -= width;
		if (left == defaultLeft)
			state = Forward;

		break;
	}
}

void Battle_Animation::reverseAnimation()
{
	if (reverse)
		reverse = false;
	else
		reverse = true;

	state = Backward;
}

void Battle_Animation::resetAnimation(sf::Sprite* sprite, sf::Clock* clock)
{
	sprite->setTextureRect(sf::IntRect(defaultLeft, top, width, height));
	clock->restart();

	if (numFrames > 0)
		left = defaultLeft + width;
	else
		left = defaultLeft;

	state = Forward;
	reverse = false;
	counter = 0;
}

void Battle_Animation::changeFrameDelay(short delay) 
{ 
	frameDelay = delay; 
}

void Battle_Animation::changeInitialFrameDelay(short delay) 
{ 
	initialFrameDelay = delay; 
}

void Battle_Animation::changeFinalFrameDelay(short delay) 
{ 
	finalFrameDelay = delay; 
}

bool Battle_Animation::onFirstFrame(sf::Sprite* sprite, sf::Clock* clock)
{
	if (sprite->getTextureRect().left == defaultLeft && clock->getElapsedTime().asMilliseconds() > initialFrameDelay)
	{
		clock->restart();
		return true;
	}

	return false;
}

bool Battle_Animation::onLoopFrame(sf::Sprite* sprite)
{
	if (sprite->getTextureRect().left >= (width * startLoopFrame) + defaultLeft && 
		sprite->getTextureRect().left <= (width * finalLoopFrame) + defaultLeft)
		return true;

	return false;
}

bool Battle_Animation::isReversing()
{
	return reverse;
}

bool Battle_Animation::isAnimating(sf::Sprite* sprite)
{
	if (loopOnce && counter > 0 && sprite->getTextureRect().left == defaultLeft)
		return false;
	
	return true;
}