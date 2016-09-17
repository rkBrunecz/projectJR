/*
Battle_Methods

The purpose of this class is to provide default behavior to various actions in battle. This allows individual battle_objects to use either specialized behaviour, or rely on methods defined within
this class for default behaviour.
*/

#ifndef Battle_Methods_H
#define Battle_Methods_H

#include <SFML\Graphics.hpp>
#include "Battle_Animation.h"
#include "Attack.h"

class Battle_Methods
{
public:
	//PUBLIC METHODS
	static bool moveToPosition(float x, float y, float elapsedTime, int width, sf::Vector2f& velocity, sf::Sprite* sprite, Battle_Animation* animation, sf::Clock* battleAniClock);
	static sf::Vector2f setVelocity(float speed, float x, float y, float spritePosX, float spritePosY);

private:
	//CONSTANTS
	static const enum Movements
	{
		Left,
		Right
	};
};

#endif