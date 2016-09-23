/*
Collision.h
The collision class provides methods for detecting collision.

@author Randall Brunecz
@version 1.0 9/11/2015
*/

#ifndef Collision_H
#define Collision_H

#include <SFML\Graphics.hpp>
#include "Collidable.h"

namespace pb
{
	void addToCollisionList(Collidable *c);
	bool collisionDetected(const sf::IntRect& rect);
}

#endif