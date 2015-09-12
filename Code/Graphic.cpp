/*
Authors: Randy Brunecz, Joshua Gray
Graphic.cpp
9/1/2015

Parent class that defines common elements that child objects will share.
*/

#include "Graphic.h"
#include <SFML\Graphics.hpp>

/*
draw
Parameters:
	window: This is the window where all graphics will be drawn

This method is shell. Every child of graphic will have it's own defined
draw method that will be specific to that object.
*/
void Graphic::draw(sf::RenderWindow* window)
{
	
}

/*
updatePosition
Parameters:
	window: Used to get information about the window and to poll events, if needed.

This method is shell. Every child of graphic will have it's own defined
updatePosition method which will dictate how positions of graphical objects
will move around. Return is the default use of this method. This is specifically
used for graphic elements that do not need their positions updated.
*/
void Graphic::updatePosition(sf::RenderWindow* window, Camera* camera)
{
	return;
}

/*
setColor
Parameters:
	r: This value determines the red scale for a graphic
	g: This value determines the green scale for a graphic
	b: This value determines the blue scale for a graphic
	a: This value determines the transparency of a graphic

This method sets the color and transparency for a given graphic object
*/
void Graphic::setColor(int r, int g, int b, int a)
{
	return;
}

bool Graphic::collisionDetected(sf::FloatRect tmp)
{
	return false;
}
