/*
Authors: Randy Brunecz, Joshua Gray
Camera
9/11/2015

This class inherits functions from the sf::View class and implements a few extra features.
*/

#ifndef Collision_H
#define Collision_H

#include <SFML\Graphics.hpp>
#include "Graphic.h"

class Collision 
{
public:
	static void intializeGraphicObjects(Graphic** graphics, int graphicsSize);
	static bool collisionDetected(sf::FloatRect rect);

private:
	static Graphic** elements;
	static int eleSize;
};

#endif