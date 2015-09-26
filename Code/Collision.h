/*
Authors: Randy Brunecz, Joshua Gray
Collision
9/11/2015

The collision class provides methods for detecting collision
*/

#ifndef Collision_H
#define Collision_H

#include <SFML\Graphics.hpp>
#include "Graphic.h"

class Collision 
{
public:
	static void intializeGraphicObjects(Graphic** graphics, int graphicsSize);
	static bool collisionDetected(sf::IntRect* rect);

private:
	static Graphic** elements;
	static int eleSize;
};

#endif