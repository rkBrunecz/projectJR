/*
Player.h
This class contains information about the player character such as position, graphic used and
characteristics.

@author Randall Brunecz
@version 1.0 9/3/2015
*/

#ifndef Player_H
#define Player_H

// HEADERS
#include "PBE\Graphic\Graphic_Entity.h"
#include "PBE\System\Collision.h"
#include "Game\Battle\Battle_Object.h"
#include "Game\Animation.h"

class Player : public pb::Graphic_Entity, public pb::Collidable, public Battle_Object
{
public:
	// PUBLIC CONSTANTS
	static const enum States
	{
		World,
		Battle
	};

	// Constructor
	Player();

	// Destructor
	~Player();

	// Copy constructor
	Player(const Player& p);

	// Operators
	const Player &operator= (const Player& p);

	// PUBLIC FUNCTIONS
	void changePlayerState(const States state);

	// Graphic entity methods
	void updateDrawList();
	void updatePosition(const float elapsedTime);
	void setPosition(const sf::Vector2f coords);
	const sf::IntRect getRect();

	// Collidable methods
	bool collisionDetected(const sf::IntRect& rect);

	// Battle functions
	void drawSprite();
	short performBattleAction(sf::Event lastKeyPressed, short numAttacksPerformed);

private:
	// PRIVATE CONSTANTS
	const float VELOCITY = 180;
	const short WIDTH = 20, HEIGHT = 3;
	const short MOVEMENT_UPDATES = 15;

	// PRIVATE VARIABLES
	Animation::WalkingDirection currentDirection = Animation::Down;
	sf::Clock characterAniClock;
	float x, y;

	States state = World;

	// Battle variables
	Attack *chop = NULL, *uppercut = NULL, *arielSlash = NULL;

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