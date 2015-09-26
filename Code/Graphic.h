/*
Authors: Randy Brunecz, Joshua Gray
Graphic.h
9/1/2015

Parent class that defines common elements that child objects will share.
*/

#ifndef Graphic_H
#define Graphic_H

#include <SFML\Graphics.hpp>
#include "Camera.h"

class Graphic
{
public:
	//PUBLIC FUNCTIONS
	virtual void draw(sf::RenderWindow* window);
	virtual void updatePosition(sf::RenderWindow* window, Camera* camera);
	virtual void setColor(int r, int g, int b, int a);

	virtual bool collisionDetected(sf::IntRect* tmp);
};

#endif