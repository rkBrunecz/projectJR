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

	//PUBLIC FUNCTIONS
	static void updateAnimation(bool positionUpdated, WalkingDirection currentDirection, sf::Clock* animationClock, sf::Sprite* sprite);
};

#endif