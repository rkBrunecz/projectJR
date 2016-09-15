#ifndef Attack_H
#define Attack_H

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Camera.h"
#include "Audio_Engine.h"
#include "Audio.h"

class Attack
{
public:
	// Constructors
	void initializeAttackData(short numFrames, 
		short frameDelay, 
		short finalFrameDelay, 
		short activeFrame, 
		short hitStun, 
		short specialHitStun, 
		int damage, 
		int height,
		int width, 
		int top, 
		int left, 
		bool isLauncher,
		bool isMeteorSmash,
		Audio *sound);
	void initializeAttackData(short numFrames,
		short frameDelay,
		short finalFrameDelay,
		short activeFrame,
		short hitStun,
		short specialHitStun,
		int damage,
		int height,
		int width,
		int top,
		int left,
		bool isLauncher,
		bool isMeteorSmash,
		Audio *sound,
		Audio *specialSound);

	// PUBLIC FUNCTIONS
	bool animateAttack(sf::Sprite* sprite, sf::Clock* clock, bool reset);
	bool inActiveFrame(sf::Sprite* sprite);
	bool animationFinsihed();
	bool isALauncher();
	bool isMeteorSmash();
	bool pastActiveFrame(sf::Sprite* sprite);
	int getDamage(bool enemyInCrushState);
	short getHitStun(sf::Clock* clock, sf::Sprite* sprite, bool enemyInCrushState);

private:
	//PRIVATE FUNCTIONS
	void advanceFrame(sf::Sprite* sprite, sf::Clock* clock);
	void resetAnimation(sf::Sprite* sprite, sf::Clock* clock);

	//PRIVATE VARIABLES
	short numFrames = 0; //Number of frames in the attack
	short frameDelay = 0; //Amount of time to elapse before switching to next frame in an attack
	short finalFrameDelay = 0; //The final amount of frame delay on the last part of an attack
	short activeFrameDelay = 0;
	short activeFrame = 0; //Frame of an attack where it is considered to be making contact
	short hitStun = 0;
	short specialHitStun = 0;
	short soundId = -1, specialSoundId = -1;
	int damage = 0;
	int height = 0;
	int width = 0;
	int top = 0; //top of the row that the animation frames are in
	int defaultLeft = 0; //Left side of the frame of the first part of the animation
	int left = 0;
	bool animationFinished = false, isLauncher = false, meteorSmash = false;
};

#endif