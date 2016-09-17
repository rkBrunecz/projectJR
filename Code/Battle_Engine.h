#ifndef Battle_Engine_H
#define Battle_Engine_H

#include "Battle_Object.h"
#include "Audio_Engine.h"
#include "Graphic.h"
#include "Ogre.h"

class Battle_Engine
{
public:
	Battle_Engine();

	void initialize(Battle_Object** player, int numPlayers, float displayWidth, float displayHeight);
	void startTurn(sf::Event lastKeyPressed, float elapsedTime);
	void updateDrawList(bool animate);

private:
	void changeTurns();
	void resetEngineValues();
	void determineMonsterParty();

	const int LAUNCH_HEIGHT = 300;
	const short NUM_ENEMY_POS = 6, NUM_PLAYER_POS = 3, MAX_NUMBER_ATTACKS = 3;
	
	enum States
	{
		Deciding,
		Moving,
		Attacking,
		Launch,
		Falling,
		Return
	} engineState = Deciding;

	int numPlayers;
	float displayWidth, displayHeight;
	short positionSelected, 
		attacksPerformed = 0,
		actorPosition; // "Actor" refers to the monster or player currently having an action decided for them
	bool playerTurn = true, attackAccountedFor = true, defenderLaunched = false, arielAttacking = false, movementFinished = false;
	sf::Sprite backdropSprite;

	struct Sprite_Position
	{
		float x, y;
		Battle_Object* sprite = NULL;
		bool isOccupied = false;

		void occupySpace(Battle_Object* object, bool isPlayer)
		{
			sprite = object;
			sprite->setSpritePosition(x, y);
			isOccupied = true;
			this->isPlayer = isPlayer;
		}

		void setVelocity(float x, float y)
		{
			sprite->setVelocity(x, y, isPlayer);
		}

		~Sprite_Position()
		{
			if (!isPlayer && sprite != NULL)
				delete sprite;
		}

	private:
		bool isPlayer = false;
	};

	std::vector<Sprite_Position> monsters, players;
	std::vector<Sprite_Position> *attacker, *defender;
}; 

#endif