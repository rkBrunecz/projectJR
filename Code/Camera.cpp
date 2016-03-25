/*
Camera.cpp
This class inherits functions from the sf::View class and implements a few extra features.

@author Randall Brunecz
@version 1.0 9/11/2015
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
	cameraBounds = sf::Vector2f(x, y);
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
	else if (position.x > cameraBounds.x - (getSize().x * 0.5))
		cameraX = cameraBounds.x - (getSize().x * 0.5);

	//If the y position to be set is at the edge of the game map, set the cameras position to its boundaries 
	if (position.y < getSize().y * 0.5)
		cameraY = getSize().y * 0.5;
	else if (position.y > cameraBounds.y - (getSize().y * 0.5))
		cameraY = cameraBounds.y - (getSize().y * 0.5);

	if (cameraBounds.x < getSize().x)
		cameraX = (cameraBounds.x / 2);
	if (cameraBounds.y < getSize().y)
		cameraY = (cameraBounds.y / 2);

	setCenter(cameraX, cameraY);
}
