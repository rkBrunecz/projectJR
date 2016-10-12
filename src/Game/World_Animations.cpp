// HEADERS
#include "Game\World_Animations.h"

/* WALK Class */

Walk::Walk(int numFrames, int startingLoopFrame, int finalLoopFrame, int width, int height, float updateInterval, int upTop, int downTop, int rightTop, int leftTop) : Animation(numFrames, startingLoopFrame, finalLoopFrame, width, height, updateInterval)
{
	upWalkTop = upTop;
	downWalkTop = downTop;
	rightWalkTop = rightTop;
	leftWalkTop = leftTop;

	frameAdvance = WIDTH;
	currentTop = downWalkTop;
	currentLeft = 0;
}

void Walk::updateWalkCycle(WalkingDirection direction, sf::Clock* animationClock)
{
	int top;

	switch (direction)
	{
	case Up:
		top = upWalkTop;

		break;

	case Down:
		top = downWalkTop;

		break;

	case Left:
		top = leftWalkTop;

		break;

	case Right:
		top = rightWalkTop;

		break;

	case None:
		currentLeft = 0;
		frameAdvance = WIDTH;

		return; // Break out of method since no frame advance needs to be done

		break;
	}

	if (top == currentTop && animationClock->getElapsedTime().asSeconds() > updateInterval)
	{
		advanceAnimationFrame(top);
		animationClock->restart();
	}

	currentTop = top;
}

void Walk::updateAnimation(sf::Sprite* sprite)
{
	sprite->setTextureRect(sf::IntRect(currentLeft, currentTop, WIDTH, HEIGHT));
}

void Walk::advanceAnimationFrame(int top)
{
	if (currentLeft / WIDTH == STARTING_LOOP_FRAME)
		frameAdvance = WIDTH;
	else if (currentLeft / WIDTH == FINAL_LOOP_FRAME)
		frameAdvance = -WIDTH;

	if (currentTop != top)
		currentLeft = STARTING_LOOP_FRAME * WIDTH;
	else
		currentLeft += frameAdvance;
}
