/*
Camera.cpp
This class inherits functions from the sf::View class and implements a few extra features.

@author Randall Brunecz
@version 1.0 9/11/2015
*/

#include "PBE\System\Camera.h"
#include "Game\Game.h"

pb::Camera::Camera(int windowWidth, int windowHeight, float zoomFactor)
{	
	camera.setSize((float)windowWidth, (float)windowHeight);
	defaultSizeX = (float)windowWidth;
	defaultSizeY = (float)windowHeight;

	camera.zoom(zoomFactor);
	
	camera.setCenter(windowWidth * 0.25f, windowHeight - (windowHeight * 0.25f));
	cameraOrigins = sf::Vector2f(camera.getCenter().x, camera.getCenter().y);
	previousX = camera.getCenter().x;
	previousY = camera.getCenter().y;

	shakeSoundId = -1;
}

pb::Camera::Camera(int windowWidth, int windowHeight, float zoomFactor, const std::string shakeSoundFileName) : Camera(windowWidth, windowHeight, zoomFactor)
{
	shakeSoundId = Game::audioManager->addSound(new Audio(shakeSoundFileName, 100));
}

void pb::Camera::setSize(float x, float y)
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
void pb::Camera::setBounds(float x, float y)
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
void pb::Camera::updatePosition(sf::Vector2f position)
{
	//LOCAL VARIABLES
	float cameraX = position.x, cameraY = position.y;

	//If the x position to be set is at the edge of the cameras bounds, set the cameras position to its boundaries 
	if (position.x < camera.getSize().x * 0.5)
		cameraX = camera.getSize().x * 0.5f;
	else if (position.x > cameraBounds.x - (camera.getSize().x * 0.5))
		cameraX = cameraBounds.x - (camera.getSize().x * 0.5f);

	//If the y position to be set is at the edge of the cameras bounds, set the cameras position to its boundaries 
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

void pb::Camera::move(const sf::Vector2f offset)
{
	camera.move(offset);
}

void pb::Camera::zoom(float zoomFactor)
{
	camera.zoom(zoomFactor);
}

void pb::Camera::setCenter(float x, float y)
{
	camera.setCenter(x, y);
}

const sf::View pb::Camera::getCamera()
{
	return camera;
}

const sf::Vector2f pb::Camera::getCameraOrigins()
{
	return cameraOrigins;
}

void pb::Camera::animateCamera()
{
	switch (animation)
	{
	case Shake:
		cameraShake();

		break;

	case EmphasisZoomIn:
		if (camera.getSize().x == defaultSizeX * zoomFactor)
			return;

		camera.setSize(defaultSizeX, defaultSizeY);
		camera.zoom(zoomFactor);
		camera.setCenter(zoomTo);

		break;

	case EmphasisZoomOut:

		camera.setSize(defaultSizeX, defaultSizeY);
		camera.zoom(previousZoom);
		camera.setCenter(previousX, previousY);

		animation = None;

		break;
	}
}

void pb::Camera::emphasisZoomIn(float x, float y, float zoomFactor)
{
	previousX = camera.getCenter().x;
	previousY = camera.getCenter().y;
	previousZoom = camera.getSize().x / defaultSizeX;
	this->zoomFactor = zoomFactor;
	zoomTo = sf::Vector2f(x, y);

	animation = EmphasisZoomIn;
}

void pb::Camera::emphasisZoomOut()
{
	animation = EmphasisZoomOut;
}

void pb::Camera::startCameraShake(const float factor)
{
	animation = Shake;
	previousX = camera.getCenter().x;
	previousY = camera.getCenter().y;

	displacements.push_back(sf::Vector2f(10, 10) * factor);
	displacements.push_back(sf::Vector2f(-10, 10) * factor);
	displacements.push_back(sf::Vector2f(10, -10) * factor);
	displacements.push_back(sf::Vector2f(-10, -10) * factor);
	displacements.push_back(sf::Vector2f(-2, -2) * factor);
	displacements.push_back(sf::Vector2f(1, 1) * factor);
	displacements.push_back(sf::Vector2f(3, 3) * factor);
	displacements.push_back(sf::Vector2f(5, -5) * factor);
	displacements.push_back(sf::Vector2f(3, 3) * factor);
	displacements.push_back(sf::Vector2f(2, 0) * factor);

	if (shakeSoundId != -1)
		Game::audioManager->playSound(shakeSoundId);
}

void pb::Camera::cameraShake()
{
	if (clock.getElapsedTime().asMilliseconds() < 20)
		return;

	if (displacementsPos == NUM_DISPLACEMENTS)
	{
		animation = None;
		displacementsPos = 0;
		camera.setCenter(previousX, previousY);
		
		return;
	}

	camera.setCenter(previousX, previousY);
	
	camera.move(displacements[displacementsPos].x, displacements[displacementsPos].y);
	
	displacementsPos++;
	clock.restart();
}

bool pb::Camera::cameraIsShaking()
{
	if (animation == Shake)
		return true;

	return false;
}