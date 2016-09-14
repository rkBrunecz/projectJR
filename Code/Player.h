/*
Player.h
This class contains information about the player character such as position, graphic used and
characteristics.

@author Randall Brunecz
@version 1.0 9/3/2015
*/

#ifndef Player_H
#define Player_H

#include "Battle_Object.h"
#include "Animation.h"
#include "Collision.h"

class Player : public Graphic, public Battle_Object
{
public:
	//PUBLIC FUNCTIONS
	Player();
	void changePlayerState();

	// Graphics functions
	void updateDrawList(bool animate);
	void setColor(int r, int g, int b, int a);

	// Overworld functions
	void updatePosition(float elapsedTime);
	void setPlayerPosition(sf::Vector2f coords);
	sf::IntRect getRect();

	// Battle functions
	void drawSprite(bool animate);
	void initialize();
	short performBattleAction(sf::Event lastKeyPressed, short numAttacksPerformed);

private:
	//PRIVATE CONSTANTS
	const float VELOCITY = 180;
	const short WIDTH = 20, HEIGHT = 3;
	const short MOVEMENT_UPDATES = 15;
	const enum States
	{
		World,
		Battle
	};

	//PRIVATE VARIABLES
	Animation::WalkingDirection currentDirection = Animation::Down;
	sf::Clock characterAniClock;
	float x, y;

	States state = World;

	//Battle variables
	Attack chop, uppercut, arielSlash;

	struct Character{
		sf::Sprite sprite, shadow;
		sf::CircleShape shadowShape;
		sf::RenderTexture tex;

		void setPosition(float x, float y)
		{
			sprite.setPosition(x, y);
			shadow.setPosition(x - 8, y - 8);
		}
	} character;
};

#endif