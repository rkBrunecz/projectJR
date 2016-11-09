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
#include "PBE\Graphics\Graphic_Entity.h"
#include "PBE\System\Collision.h"
#include "Game\Battle\Battle_Object.h"
#include "Game\World_Animations.h"

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
	void loadState(const States state);

	// TEST
	void renderPosition(double alpha);

	// Graphic entity methods
	void updateDrawList();
	void updatePosition(const float elapsedTime);
	void setPosition(const sf::Vector2f coords);
	const sf::IntRect getRect();

	sf::Vector2u getPosition();

	// Obtains the position in front of current facing direction of the sprite
	sf::Vector2u getPosition(unsigned int tileSize);

	// Collidable methods
	bool collisionDetected(const sf::IntRect& rect);

	// Battle functions
	void drawSprite(bool animate);
	short performBattleAction(sf::Event lastKeyPressed, short numAttacksPerformed);

private:
	// PRIVATE CONSTANTS
	const float VELOCITY = 150;
	const short WIDTH = 20, HEIGHT = 3;
	const short MOVEMENT_UPDATES = 15;
	const enum Directions {
		Up,
		Down,
		Left,
		Right
	};

	// PRIVATE VARIABLES
	sf::Clock characterAniClock;
	sf::Vector2f pos, lastPos;
	sf::Vector2f velocity;

	States state = World;
	Directions facingDirection = Down;

	// Battle variables
	Attack *chop = 0, *uppercut = 0, *arielSlash = 0;
	Walk *walk = 0;

	struct Character{
		sf::Sprite sprite;
		sf::CircleShape shadow;

		void setPosition(sf::Vector2i pos)
		{
			sprite.setPosition((sf::Vector2f)pos);
			shadow.setPosition((float)pos.x - 8, (float)pos.y - 8);;
		}
	} character;
};

#endif