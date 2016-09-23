#ifndef Ogre_H
#define Ogre_H

#include "Battle_Object.h"

class Ogre : public Battle_Object
{
public:
	//PUBLIC FUNCTIONS
	Ogre();
	~Ogre() { };

	// Battle functions
	void drawSprite();
	short performBattleAction(short numAttacksPerformed);

	// Movement functions
	bool moveToPosition(float x, float y); 
	//bool jumping(float y);

	// DEBUG
	void toggleCrushState();
};

#endif