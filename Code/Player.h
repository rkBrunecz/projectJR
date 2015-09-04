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
#include "Animation.h"

class Player : public Graphic
{
public:
	//PUBLIC FUNCTIONS
	Player(sf::RenderWindow* window);
	void draw(sf::RenderWindow* window);
	void updatePosition(sf::RenderWindow* window);
	void setColor(int r, int g, int b, int a);

private:
	//PRIVATE CONSTANTS
	float VELOCITY = 1;

	//PRIVATE VARIABLES
	Animation::WalkingDirection currentDirection = Animation::Down;
	sf::Sprite sprite;
	sf::Texture spriteMap;
	sf::Clock characterAnimation;
	float x, y;
};

#endif