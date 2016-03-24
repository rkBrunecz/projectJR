/*
Player.h
This class contains information about the player character such as position, graphic used and
characteristics.

@author Randall Brunecz
@version 1.0 9/3/2015
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
	Player(Camera* camera);

	void draw(sf::RenderWindow* window);
	void updatePosition(sf::RenderWindow* window, Camera* camera);
	void setColor(int r, int g, int b, int a);
	void setPlayerPosition(sf::Vector2i coords);

	sf::IntRect getRect();

private:
	//PRIVATE CONSTANTS
	float VELOCITY = 3;
	short WIDTH = 14, HEIGHT = 3;

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
			shadow.setPosition(x, y - 11);
		}
	} character;
};

#endif