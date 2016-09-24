/*
Collision.cpp
The collision class provides methods for detecting collision.

@author Randall Brunecz
@version 1.0 9/11/2015
*/

#include "PBE\System\Collision.h"

namespace
{
	std::vector<pb::Collidable *> elements;
}

/*
intializeGraphicObjects
Parameters:
	graphics: This array contains a list of all graphical entities in the game
	graphicsSize: This is the size of the graphics array

This method gets a list of all graphic objects that need to be checked for collision
*/
void pb::addToCollisionList(Collidable *c)
{
	elements.push_back(c);
}

/*
collisionDetected
Parameters:
	rect: This is the bounding rect of a given entity to be check against other graphical objects

This method checks for collision in the game world. If collision is detected, the method returns true
*/
bool pb::collisionDetected(const sf::IntRect& rect)
{
	//LOCAL VARIABLES
	bool collision = false;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		if (elements[i]->collisionDetected(rect))
			collision = true;
	}

	return collision;
}