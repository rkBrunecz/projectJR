#include "Camera.h"
#include <SFML\Graphics.hpp>

void Camera::updateCameraPosition(float velocityX, float velocityY, int playerX, int playerY)
{
	if (playerX > displayWidth / 2 && playerX < mapX - (displayWidth / 2))
		x += velocityX;

	if (playerY > displayHeight / 2 && playerY < mapY - (displayHeight / 2))
		y += velocityY;
}

void Camera::setDisplay(int displayW, int displayH)
{
	displayWidth = displayW;
	displayHeight = displayH;
}

void Camera::setCameraMaxBounds(int x, int y)
{
	mapX = x;
	mapY = y;
}

int Camera::getXPos()
{
	return x;
}

int Camera::getYPos()
{
	return y;
}

int Camera::getUpperBoundWidth(int tileSize)
{
	if ((x + displayWidth) / tileSize > mapX / tileSize)
		return mapX / tileSize;
	else
		return (x + displayWidth) / tileSize;
}

int Camera::getUpperBoundHeight(int tileSize)
{
	if ((y + displayHeight) / tileSize > mapY / tileSize)
		return mapY / tileSize;
	else
		return (y + displayHeight) / tileSize;
}

int Camera::getLowerBoundWidth(int tileSize)
{
	if ((x - displayWidth) / tileSize < 0)
		return 0;
	else
		return (x - displayWidth) / tileSize;
}

int Camera::getLowerBoundHeight(int tileSize)
{
	if ((y - displayHeight) / tileSize < 0)
		return 0;
	else
		return (y - displayHeight) / tileSize;
}

bool Camera::pastCameraBoundaryX(int x)
{
	if (x < displayWidth / 2 || x > mapX - (displayWidth / 2))
		return true;

	return false;
}

bool Camera::pastCameraBoundaryY(int y)
{
	if (y < displayHeight / 2 || y > mapY - (displayHeight / 2))
		return true;

	return false;
}