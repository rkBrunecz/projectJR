/*
Graphic.h
Parent class that defines the behaviour of any graphic object within the game.

@author Randall Brunecz
@version 1.0 9/1/2015
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