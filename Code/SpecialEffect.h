/*
Authors: Randy Brunecz, Joshua Gray
SpecialEffect.h
9/3/2015

The special effects object contains many methods for creating special effects such as fades, screen dims, etc.
*/

#ifndef SpecialEffect_H
#define SpecialEffect_H

#include <SFML\Graphics.hpp>
#include "Player.h"
#include "Map.h"

class SpecialEffect
{
public:
	//PUBLIC FUNCTIONS
	static void fadeOut(sf::RenderWindow* window, Map* map, Player* player);
	static void fadeIn(sf::RenderWindow* window, Map* map, Player* player);
	static void screenDim(sf::RenderWindow* window);
};

#endif