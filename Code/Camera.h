#ifndef Camera_H
#define Camera_H

#include <SFML\Graphics.hpp>

class Camera : public sf::View
{
public:
	void setBounds(int x, int y);
	sf::Vector2f getCameraBounds();

private:
	int cameraBoundX, cameraBoundY;
};

#endif