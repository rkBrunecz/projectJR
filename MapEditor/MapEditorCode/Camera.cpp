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
updatePosition
Parameters:
	x: The x position to set the camera to
	y: The y position to set the camera to

This method simply updates the position of the camera.
*/
void Camera::updatePosition(sf::Vector2i position)
{
	//LOCAL VARIABLES
	int cameraX = position.x, cameraY = position.y;

	//If the x position to be set is at the edge of the game map, set the cameras position to its boundaries 
	if (position.x < getSize().x * 0.5)
		cameraX = getSize().x * 0.5;
	else if (position.x > getCameraBounds().x)
		cameraX = getCameraBounds().x;

	//If the y position to be set is at the edge of the game map, set the cameras position to its boundaries 
	if (position.y < getSize().y * 0.5)
		cameraY = getSize().y * 0.5;
	else if (position.y > getCameraBounds().y)
		cameraY = getCameraBounds().y;

	setCenter(cameraX, cameraY);
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
