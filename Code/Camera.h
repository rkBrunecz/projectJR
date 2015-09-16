/*
Authors: Randy Brunecz, Joshua Gray
Camera
9/11/2015

This class inherits functions from the sf::View class and implements a few extra features.
*/

#ifndef Camera_H
#define Camera_H

#include <SFML\Graphics.hpp>

class Camera : public sf::View
{
public:
	void setBounds(int x, int y);
	void updatePosition(sf::Vector2i position);

	sf::Vector2f getCameraBounds();

private:
	int cameraBoundX, cameraBoundY;
};

#endif