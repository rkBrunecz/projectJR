/*
Authors: Randy Brunecz, Joshua Gray
Animation
9/4/2015

This class handles animations for sprites
*/

#ifndef Animation_H
#define Animation_H

#include <SFML\Graphics.hpp>

class Animation
{
public:
	//PUBLIC VARIABLES
	const enum WalkingDirection{
		Up,
		Down,
		Right,
		Left
	};
	const enum WaterDirection{
		ShiftLeft,
		ShiftRight
	};

	//PUBLIC FUNCTIONS
	static void updateAnimation(bool positionUpdated, WalkingDirection currentDirection, sf::Clock* animationClock, sf::Sprite* sprite);
	static void updateWaterAnimation(WaterDirection* direction, sf::Clock* waterAnimation, sf::Sprite* water, sf::RenderTexture* frames, unsigned short* currentFrame, unsigned short numFrames);
};

#endif