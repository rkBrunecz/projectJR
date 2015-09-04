/*
Authors: Randy Brunecz, Joshua Gray
SpecialEffect.h
9/3/2015

The special effects object contains many methods for creating special effects such as fades, screen dims, etc.
*/

#ifndef SpecialEffect_H
#define SpecialEffect_H

#include <SFML\Graphics.hpp>
#include "Graphic.h"

class SpecialEffect
{
public:
	//PUBLIC FUNCTIONS
	static void fadeOut(sf::RenderWindow* window, Graphic* graphics[], int arraySize);
	static void fadeIn(sf::RenderWindow* window, Graphic* graphics[], int arraySize);
	static void screenDim(Graphic* graphics[], int arraySize);
	static void resetScreenDim(Graphic* graphics[], int arraySize);
};

#endif