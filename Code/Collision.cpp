/*
Collision.cpp
The collision class provides methods for detecting collision.

@author Randall Brunecz
@version 1.0 9/11/2015
*/

#include "Collision.h"

//STATIC VARIABLE INTIALIZATION
Graphic** Collision::elements;
int Collision::eleSize = 0;

/*
intializeGraphicObjects
Parameters:
	graphics: This array contains a list of all graphical entities in the game
	graphicsSize: This is the size of the graphics array

This method gets a list of all graphic objects that need to be checked for collision
*/
void Collision::initializeGraphicObjects(Graphic** graphics, int graphicsSize)
{
	elements = graphics;
	eleSize = graphicsSize;
}

/*
collisionDetected
Parameters:
	rect: This is the bounding rect of a given entity to be check against other graphical objects

This method checks for collision in the game world. If collision is detected, the method returns false;
*/
bool Collision::collisionDetected(sf::IntRect* rect)
{
	//LOCAL VARIABLES
	bool collision = false;

	for (int i = 0; i < eleSize - 1; i++)
	{
		if (elements[i]->collisionDetected(rect))
			collision = true;
	}

	return collision;
}