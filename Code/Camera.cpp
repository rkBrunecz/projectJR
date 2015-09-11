#include "Camera.h"

void Camera::setBounds(int x, int y)
{
	cameraBoundX = x;
	cameraBoundY = y;
}

sf::Vector2f Camera::getCameraBounds()
{
	return sf::Vector2f(cameraBoundX - (getSize().x * 0.5), cameraBoundY - (getSize().y * 0.5));
}
