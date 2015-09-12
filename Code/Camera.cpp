/*
Authors: Randy Brunecz, Joshua Gray
Camera
9/11/2015

This class inherits functions from the sf::View class and implements a few extra features.
*/

#include "Camera.h"

/*
setBounds
Parameters:
	x: This is the bound along the x-axis
	y: This is the bound along the y-axis

This method stores the bounds for the camera along the x and y axis.
*/
void Camera::setBounds(int x, int y)
{
	cameraBoundX = x;
	cameraBoundY = y;
}

/*
setBounds
return: Returns a 2d vector that contains the x and y camera bounds.

Gets the camera bounds for the map.
*/
sf::Vector2f Camera::getCameraBounds()
{
	return sf::Vector2f(cameraBoundX - (getSize().x * 0.5), cameraBoundY - (getSize().y * 0.5));
}
