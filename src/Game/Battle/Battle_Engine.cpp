#include "Game\Battle\Battle_Engine.h"
#include "Game\Game.h"

Battle_Engine::Battle_Engine()
{
	monsters.resize(NUM_ENEMY_POS);
	players.resize(NUM_PLAYER_POS);
}

Battle_Engine::Battle_Engine(Battle_Object** player, int numPlayers, float displayWidth, float displayHeight)
{
	monsters.resize(NUM_ENEMY_POS);
	players.resize(NUM_PLAYER_POS);

	this->numPlayers = numPlayers;
	this->displayWidth = displayWidth;
	this->displayHeight = displayHeight;

	// Intialize possible monster positions
	for (unsigned int i = 0; i < monsters.size(); i++)
	{
		if (i > 2)
			monsters[i].x = 96 * 2;
		else
			monsters[i].x = 96;

		monsters[i].y = (displayHeight + 96 * 3) + (96 * (i % 3));
	}

	determineMonsterParty();

	// Intialize possible player positions
	for (unsigned int i = 0; i < players.size(); i++)
	{
		players[i].x = displayWidth - (player[0]->getDimensions().x);
		players[i].y = (displayHeight + player[0]->getDimensions().y * 3) + (player[0]->getDimensions().y * (i % 3));
	}

	for (int i = 0; i < numPlayers; i++)
		players[0].occupySpace(player[i], true);


	attacker = &players;
	defender = &monsters;

	// Intialize backdrop
	backdropSprite.setTexture(*Game::graphicManager->addTexture("battle_backdrop.png"));
	backdropSprite.setPosition(-20, 20);
}

void Battle_Engine::initialize(Battle_Object** player, int numPlayers, float displayWidth, float displayHeight)
{
	this->numPlayers = numPlayers;
	this->displayWidth = displayWidth;
	this->displayHeight = displayHeight;

	// Intialize possible monster positions
	for (unsigned int i = 0; i < monsters.size(); i++)
	{
		if (i > 2)
			monsters[i].x = 96 * 2;
		else
			monsters[i].x = 96;

		monsters[i].y = (displayHeight + 96 * 3) + (96 * (i % 3));
	}

	determineMonsterParty();

	// Intialize possible player positions
	for (unsigned int i = 0; i < players.size(); i++)
	{
		players[i].x = displayWidth - (player[0]->getDimensions().x);
		players[i].y = (displayHeight + player[0]->getDimensions().y * 3) + (player[0]->getDimensions().y * (i % 3));
	}

	for (int i = 0; i < numPlayers; i++)
		players[0].occupySpace(player[i], true);


	attacker = &players;
	defender = &monsters;

	// Intialize backdrop
	backdropSprite.setTexture(*Game::graphicManager->addTexture("battle_backdrop.png"));
	backdropSprite.setPosition(-20, 20);
}

void Battle_Engine::determineMonsterParty()
{
	monsters[4].occupySpace(new Ogre(), false);
}

void Battle_Engine::startTurn(sf::Event lastKeyPressed, float elapsedTime)
{
	switch (engineState)
	{
	case Deciding:
		// Do stuff if player turn
		if (playerTurn && sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		{
			engineState = Moving;
			positionSelected = 4;
			actorPosition = 0;

			float x = defender->at(positionSelected).x;
			float y = defender->at(positionSelected).y;

			attacker->at(actorPosition).setVelocity(x, y);
		}

		break;

	case Moving:
	{
		float x = defender->at(positionSelected).x + (defender->at(positionSelected).sprite->getDimensions().x / 2) + 2;
		float y = defender->at(positionSelected).y;
		
		if (!movementFinished)
			movementFinished = attacker->at(actorPosition).sprite->moveToPosition(x, y, elapsedTime, true);

		if (movementFinished)
		{
			movementFinished = false;
			engineState = Attacking;
		}

		break;
	}
	case Attacking:
	{
		// Used to determine if the number of attacks accounted for is the same or not. Keeps damage from reapplying itself over and over again.
		short previousNumberAttacks = attacksPerformed;
		bool meteorSmashing = false;

		// Use a specialize version of perfromBattleAction if players turn
		if (playerTurn)
			attacksPerformed = attacker->at(actorPosition).sprite->performBattleAction(lastKeyPressed, attacksPerformed);
		else
			attacksPerformed = attacker->at(actorPosition).sprite->performBattleAction(attacksPerformed);

		// Checks to see if the new attack has been accounted for
		if (previousNumberAttacks != attacksPerformed)
			attackAccountedFor = false;

		// DEBUG
		if (lastKeyPressed.key.code == sf::Keyboard::Num2)
			defender->at(positionSelected).sprite->toggleCrushState();

		// If an attack is in its active frame, apply its damage, hit stun, and other unique properties to the opponent
		if (!attackAccountedFor && attacker->at(actorPosition).sprite->inActiveFrame())
		{
			bool defenderCanBeCrushed = defender->at(positionSelected).sprite->inCrushState();

			int damage = attacker->at(actorPosition).sprite->getDamage(defenderCanBeCrushed);
			int hitStun = attacker->at(actorPosition).sprite->getHitStun(defenderCanBeCrushed);

			defender->at(positionSelected).sprite->takeDamage(damage, hitStun);

			if (defenderCanBeCrushed)
			{
				attacksPerformed = 0;
				defenderLaunched = attacker->at(actorPosition).sprite->attackIsLauncher();
			}

			if (arielAttacking)
				meteorSmashing = attacker->at(actorPosition).sprite->attackIsMeteorSmash();

			attackAccountedFor = true;
		}

		if (defenderLaunched && attacker->at(actorPosition).sprite->pastActiveFrame())
		{
			engineState = Launch;

			float x = defender->at(positionSelected).x;

			defenderLaunched = false;
		}

		if (meteorSmashing)
		{
			engineState = Falling;

			float defenderX = defender->at(positionSelected).x;
			float attackerX = attacker->at(actorPosition).sprite->getPosition().x;
			float y = defender->at(positionSelected).y;
		}

		// If the attacker has reached the attack cap, end the current attackers turn
		if (attacksPerformed == MAX_NUMBER_ATTACKS && !attacker->at(actorPosition).sprite->attackInProgress())
		{
			float x = attacker->at(actorPosition).x;
			float y = attacker->at(actorPosition).y;

			attacker->at(actorPosition).setVelocity(x, y);

			engineState = Return;
		}

		break;
	}
	case Launch:
	{
		if (!movementFinished)
			movementFinished = defender->at(positionSelected).sprite->launchSprite((float)LAUNCH_HEIGHT, elapsedTime);
		
		bool jumping = attacker->at(actorPosition).sprite->jumping((float)LAUNCH_HEIGHT, elapsedTime);

		if (movementFinished && !jumping)
		{
			arielAttacking = true;
			movementFinished = false;
			engineState = Attacking;
		}

		break;
	}
	case Falling:
	{
		float y = defender->at(positionSelected).y;
		bool finishedFalling = false;

		if (!movementFinished)
			movementFinished = defender->at(positionSelected).sprite->launchSpriteDown(y, elapsedTime);

		if (movementFinished && !Game::camera->cameraIsShaking())
			finishedFalling = attacker->at(actorPosition).sprite->fall(y, elapsedTime);

		if (movementFinished && finishedFalling)
		{
			float x = attacker->at(actorPosition).x;
			float y = attacker->at(actorPosition).y;

			attacker->at(actorPosition).setVelocity(x, y);

			movementFinished = false;

			engineState = Return;
		}

		break;
	}
	case Return:
	{
		float x = attacker->at(actorPosition).x;
		float y = attacker->at(actorPosition).y;
		
		if (!movementFinished)
			movementFinished = attacker->at(actorPosition).sprite->moveToPosition(x, y, elapsedTime, false);;

		if (movementFinished)
			changeTurns();

		break;
	}
	}
}

void Battle_Engine::changeTurns()
{
	/*
	if (playerTurn)
	{
		attacker = &monsters;
		defender = &players;
		playerTurn = false;
	}
	else
	{
		attacker = &players;
		defender = &monsters;
		playerTurn = true;
	}*/

	resetEngineValues();
}

void Battle_Engine::resetEngineValues()
{
	attacksPerformed = 0;
	arielAttacking = false;
	movementFinished = false;
	engineState = Deciding;
}

void Battle_Engine::updateDrawList(bool animate, double alpha)
{
	std::vector<Battle_Object*> drawAtPos;

	for (unsigned int i = 0; i < defender->size(); i++)
	{
		if (defender->at(i).isOccupied)
			drawAtPos.push_back(defender->at(i).sprite);
	}
	
	for (unsigned int i = 0; i < attacker->size(); i++)
	{
		if (attacker->at(i).isOccupied)
			drawAtPos.push_back(attacker->at(i).sprite);
	}
	
	Game::graphicManager->addToDrawList(&backdropSprite, false);
	for (unsigned int i = 0; i < drawAtPos.size(); i++)
	{
		drawAtPos[i]->renderBattlePosition(alpha);
		drawAtPos[i]->drawSprite(animate);
	}
}