/*
Collision.h
The collision class provides methods for detecting collision.

@author Randall Brunecz
@version 1.0 9/11/2015
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