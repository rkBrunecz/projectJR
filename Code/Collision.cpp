#include "Collision.h"

//STATIC VARIABLE INTIALIZATION
Graphic** Collision::elements;
int Collision::eleSize = 0;

void Collision::intializeGraphicObjects(Graphic** graphics, int graphicsSize)
{
	elements = graphics;
	eleSize = graphicsSize;
}

bool Collision::collisionDetected(sf::FloatRect rect)
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