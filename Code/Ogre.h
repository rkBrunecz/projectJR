#ifndef Ogre_H
#define Ogre_H

#include "Battle_Object.h"

class Ogre : public Graphic, public Battle_Object
{
public:
	//PUBLIC FUNCTIONS
	Ogre();
	~Ogre() { };

	// Graphics functions
	void updateDrawList(bool animate);
	void setColor(int r, int g, int b, int a);

	// Battle functions
	void drawSprite(bool animate);
	void initialize();
	short performBattleAction(short numAttacksPerformed);

	// Movement functions
	bool moveToPosition(float x, float y); 
	//bool jumping(float y);

	// DEBUG
	void toggleCrushState();
};

#endif