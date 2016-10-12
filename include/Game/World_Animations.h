/*
Animation.h
This class handles animations for various sprites.

@author Randall Brunecz
@version 1.0 9/4/2015
*/

#ifndef World_Animations_H
#define World_Animations_H

// HEADERS
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderTexture.hpp>
#include <SFML\System\Clock.hpp>
#include <PBE\Graphics\Animation.h>
#include "Game\Map\Layer.h"

class Walk : public pb::Animation
{
public:
	//PUBLIC VARIABLES
	const enum WalkingDirection{
		Up,
		Down,
		Right,
		Left,
		None
	};

	// Constructor
	Walk(int numFrames, int startingLoopFrame, int finalLoopFrame, int width, int height, float updateInterval, int upTop, int downTop, int rightTop, int leftTop);

	// Public methods
	void updateWalkCycle(WalkingDirection direction, sf::Clock* animationClock);
	void updateAnimation(sf::Sprite* sprite);

private:
	void advanceAnimationFrame(int top);

	// Private constants
	int upWalkTop, downWalkTop, rightWalkTop, leftWalkTop, currentTop, currentLeft, frameAdvance;
};

#endif

