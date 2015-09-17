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
	Player(sf::RenderWindow* window, Camera* camera);

	void draw(sf::RenderWindow* window);
	void updatePosition(sf::RenderWindow* window, Camera* camera);
	void setColor(int r, int g, int b, int a);
	void setPlayerPosition(sf::Vector2i coords);

	sf::Vector2i getPlayerCoordinates();

private:
	//PRIVATE CONSTANTS
	float VELOCITY = 10;

	//PRIVATE VARIABLES
	Animation::WalkingDirection currentDirection = Animation::Down;
	sf::Texture spriteMap;
	sf::Clock characterAniClock;
	int x, y;

	struct Character{
		sf::Sprite sprite;
		sf::CircleShape shadow;

		void setPosition(float x, float y)
		{
			sprite.setPosition(x, y);
			shadow.setPosition(x, y);
		}
	} character;
};

#endif