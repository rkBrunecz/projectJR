/*
Camera.cpp
This class inherits functions from the sf::View class and implements a few extra features.

@author Randall Brunecz
@version 1.0 9/11/2015
*/

#include "Camera.h"

// STATIC CONSTANT VARIABLE INTIALIZATION
const int Camera::NUM_DISPLACEMENTS;

// STATIC VARIABLE INTIALIZATION
sf::View Camera::camera;
sf::Vector2f Camera::cameraBounds(0, 0);
sf::Clock Camera::clock;
sf::Vector2f Camera::displacements[NUM_DISPLACEMENTS];
float Camera::previousX = 0, Camera::previousY = 0, Camera::previousZoom = 0, Camera::defaultSizeX = 0, Camera::defaultSizeY = 0, Camera::defaultBattleX, Camera::defaultBattleY;
int Camera::displacementsPos = 0;
bool Camera::shakeCamera = false;
short Camera::shakeSoundId = 0;
Camera::States Camera::state = Overworld;

void Camera::setCameraState(Camera::States state)
{
	Camera::state = state;
}

void Camera::initialize(int windowWidth, int windowHeight)
{	
	camera.setCenter(windowWidth * (0.5f / 2), windowHeight - (windowHeight * (0.5f / 2)));
	defaultBattleX = camera.getCenter().x;
	defaultBattleY = camera.getCenter().y;

	displacements[0] = sf::Vector2f(10, 10);
	displacements[1] = sf::Vector2f(-10, 10);
	displacements[2] = sf::Vector2f(10, -10);
	displacements[3] = sf::Vector2f(-10, -10);
	displacements[4] = sf::Vector2f(-2, -2);
	displacements[5] = sf::Vector2f(1, 1);
	displacements[6] = sf::Vector2f(3, 3);
	displacements[7] = sf::Vector2f(5, -5);
	displacements[8] = sf::Vector2f(3, 3);
	displacements[9] = sf::Vector2f(2, 0);

	shakeSoundId = Audio_Engine::addSound(new Audio("screenshake.wav", 100));
}

void Camera::setSize(float x, float y)
{
	defaultSizeX = x;
	defaultSizeY = y;

	camera.setSize((float)x, (float)y);
}

/*
setBounds
Parameters:
	x: This is the bound along the x-axis
	y: This is the bound along the y-axis

This method stores the bounds for the camera along the x and y axis.
*/
void Camera::setBounds(float x, float y)
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
void Camera::updatePosition(sf::Vector2f position)
{
	//LOCAL VARIABLES
	float cameraX = position.x, cameraY = position.y;

	//If the x position to be set is at the edge of the game map, set the cameras position to its boundaries 
	if (position.x < camera.getSize().x * 0.5)
		cameraX = camera.getSize().x * 0.5f;
	else if (position.x > cameraBounds.x - (camera.getSize().x * 0.5))
		cameraX = cameraBounds.x - (camera.getSize().x * 0.5f);

	//If the y position to be set is at the edge of the game map, set the cameras position to its boundaries 
	if (position.y < camera.getSize().y * 0.5)
		cameraY = camera.getSize().y * 0.5f;
	else if (position.y > cameraBounds.y - (camera.getSize().y * 0.5))
		cameraY = cameraBounds.y - (camera.getSize().y * 0.5f);

	if (cameraBounds.x < camera.getSize().x)
		cameraX = (cameraBounds.x / 2);
	if (cameraBounds.y < camera.getSize().y)
		cameraY = (cameraBounds.y / 2);

	camera.setCenter(cameraX, cameraY);
}

void Camera::zoom(float zoomFactor)
{
	camera.zoom(zoomFactor);
}

void Camera::emphasisZoomIn(float x, float y, float zoomFactor)
{
	if (camera.getSize().x == defaultSizeX * zoomFactor)
		return;

	previousX = camera.getCenter().x;
	previousY = camera.getCenter().y;
	previousZoom = camera.getSize().x / defaultSizeX;

	camera.setSize(defaultSizeX, defaultSizeY);
	camera.zoom(zoomFactor);
	camera.setCenter(x, y);
}

void Camera::emphasisZoomOut()
{
	camera.setSize(defaultSizeX, defaultSizeY);
	camera.zoom(previousZoom);
	camera.setCenter(previousX, previousY);
}

void Camera::move(float x, float y)
{
	switch (state)
	{
	case Overworld:
		camera.move(x, y);

		break;

	case Battle:
		if (camera.getCenter().y + y > defaultBattleY)
			camera.setCenter(defaultBattleX, defaultBattleY);
		else
			camera.move(x, y);

		break;
	}
}

void Camera::resetBattleCamera()
{
	camera.setCenter(defaultBattleX, defaultBattleY);
}

void Camera::setCenter(float x, float y)
{
	camera.setCenter(x, y);
}

void Camera::animateCamera()
{
	if (shakeCamera)
		cameraShake();
}

void Camera::cameraShake()
{
	if (clock.getElapsedTime().asMilliseconds() < 20)
		return;

	if (displacementsPos == NUM_DISPLACEMENTS)
	{
		shakeCamera = false;
		displacementsPos = 0;
		camera.setCenter(previousX, previousY);
		
		return;
	}

	camera.setCenter(previousX, previousY);
	
	camera.move(displacements[displacementsPos].x, displacements[displacementsPos].y);
	
	displacementsPos++;
	clock.restart();
}

void Camera::startCameraShake()
{
	shakeCamera = true;
	previousX = camera.getCenter().x;
	previousY = camera.getCenter().y;
	
	Audio_Engine::playSound(shakeSoundId);
}

sf::View Camera::getCamera()
{
	return camera;
}

bool Camera::cameraIsShaking()
{
	return shakeCamera;
}