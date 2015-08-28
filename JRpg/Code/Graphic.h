//Author: Randy Brunecz

#ifndef Graphic_H
#define Graphic_H

#include <SFML\Graphics.hpp>

class Graphic
{
public:
	//PUBLIC FUNCTIONS
	virtual void draw(sf::RenderWindow* window);
	virtual void setAlpha(int alpha);
};

#endif