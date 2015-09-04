/*
Authors: Randy Brunecz, Joshua Gray
Player.h
9/3/2015

This class contains information about the player character such as position, graphic used and
characteristics.
*/

#ifndef Player_H
#define Player_H

#include <SFML\Graphics.hpp>
#include "Graphic.h"

class Player : public Graphic
{
public:
	//PUBLIC FUNCTIONS
	Player();
	void draw(sf::RenderWindow* window);
	void updatePosition(sf::RenderWindow* window, float timeSinceLastFrame);

private:
	//PRIVATE VARIABLES
	sf::CircleShape tmp;
	float x, y;
};

#endif