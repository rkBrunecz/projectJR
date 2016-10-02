/*
Camera.cpp
This class inherits functions from the sf::View class and implements a few extra features.

@author Randall Brunecz
@version 1.0 9/11/2015
*/

#include "PBE\System\Camera.h"
#include "Game\Game.h"
namespace pb
{
	Camera::Camera(int windowWidth, int windowHeight, float zoomFactor)
	{
		setCameraSize((float)windowWidth, (float)windowHeight);

		zoom(zoomFactor);

		setCenter(windowWidth * 0.25f, windowHeight - (windowHeight * 0.25f));
		cameraOrigins = sf::Vector2f(getCenter().x, getCenter().y);
		previousX = getCenter().x;
		previousY = getCenter().y;

		shakeSoundId = -1;
	}

	Camera::Camera(int windowWidth, int windowHeight, float zoomFactor, const std::string& shakeSoundFileName) : Camera(windowWidth, windowHeight, zoomFactor)
	{
		shakeSoundId = Game::audioManager->addSound(new Audio(shakeSoundFileName, 100));
	}

	void Camera::setCameraSize(float x, float y)
	{
		defaultSizeX = x;
		defaultSizeY = y;

		setSize(x, y);
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
	void Camera::updatePosition(const sf::Vector2f& position)
	{
		//LOCAL VARIABLES
		float cameraX = position.x, cameraY = position.y;

		//If the x position to be set is at the edge of the cameras bounds, set the cameras position to its boundaries 
		if (position.x < getSize().x * 0.5)
			cameraX = getSize().x * 0.5f;
		else if (position.x > cameraBounds.x - (getSize().x * 0.5))
			cameraX = cameraBounds.x - (getSize().x * 0.5f);

		//If the y position to be set is at the edge of the cameras bounds, set the cameras position to its boundaries 
		if (position.y < getSize().y * 0.5)
			cameraY = getSize().y * 0.5f;
		else if (position.y > cameraBounds.y - (getSize().y * 0.5))
			cameraY = cameraBounds.y - (getSize().y * 0.5f);

		if (cameraBounds.x < getSize().x)
			cameraX = (cameraBounds.x / 2);
		if (cameraBounds.y < getSize().y)
			cameraY = (cameraBounds.y / 2);

		setCenter(cameraX, cameraY);
	}

	const sf::Vector2f Camera::getCameraOrigins()
	{
		return cameraOrigins;
	}

	void Camera::animateCamera()
	{
		switch (animation)
		{
		case Shake:
			cameraShake();

			break;

		case EmphasisZoomIn:
			if (getSize().x == defaultSizeX * zoomFactor)
				return;

			setSize(defaultSizeX, defaultSizeY);
			zoom(zoomFactor);
			setCenter(zoomTo);

			break;

		case EmphasisZoomOut:

			setSize(defaultSizeX, defaultSizeY);
			zoom(previousZoom);
			setCenter(previousX, previousY);

			animation = None;

			break;
		}
	}

	void Camera::emphasisZoomIn(float x, float y, float zoomFactor)
	{
		previousX = getCenter().x;
		previousY = getCenter().y;
		previousZoom = getSize().x / defaultSizeX;
		this->zoomFactor = zoomFactor;
		zoomTo = sf::Vector2f(x, y);

		animation = EmphasisZoomIn;
	}

	void Camera::emphasisZoomOut()
	{
		animation = EmphasisZoomOut;
	}

	void Camera::startCameraShake(const float factor)
	{
		animation = Shake;
		previousX = getCenter().x;
		previousY = getCenter().y;

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

	void Camera::cameraShake()
	{
		if (clock.getElapsedTime().asMilliseconds() < 20)
			return;

		if (displacementsPos == NUM_DISPLACEMENTS)
		{
			animation = None;
			displacementsPos = 0;
			setCenter(previousX, previousY);

			return;
		}

		setCenter(previousX, previousY);

		move(displacements[displacementsPos].x, displacements[displacementsPos].y);

		displacementsPos++;
		clock.restart();
	}

	bool Camera::cameraIsShaking()
	{
		if (animation == Shake)
			return true;

		return false;
	}
}