#include "Ogre.h"

/*
constructor
Parameters:
window: The game window. Used to get the window size.

Creates the main characters sprite and sets other important information.
*/
Ogre::Ogre()
{
	// Initialize the height and width of the battle sprite
	battleHeight = 96;
	battleWidth = 96;

	initialize();
}

void Ogre::initialize()
{
	// Load sprite map
	battleSprite.setTexture(*Graphic::addTexture("Ogre_test.png"));

	// Set up the battle sprite defualt sprite
	battleSprite.setTextureRect(sf::IntRect(0, 0, 96, 96));
	battleSprite.setOrigin(48, 48);

	takingDamage.intializeAnimationData(3, 70, 70, 70, 1, 3, battleHeight, battleWidth, battleHeight, 0, true);
	constantTakingDamage.intializeAnimationData(3, 100, 100, 100, 1, 3, battleHeight, battleWidth, battleHeight, 0, false);
	standing.intializeAnimationData(3, 100, 300, 300, 1, 3, battleHeight, battleWidth, 0, 0, false);

	animator.changeBattleAnimation(&standing);
}

/*
draw
Parameters:
window: The game window

This method draws the character model on to the games window
*/
void Ogre::updateDrawList(bool animate)
{
	if (animate)
		animator.animate(&battleSprite, &battleAniClock, isAttacking);

	Graphic::addToDrawList(&battleSprite, false);

	if (isTakingDamage && !animator.getCurrentAnimation()->isAnimating(&battleSprite))
	{
		isTakingDamage = false;

		if (isAirBound)
			animator.changeBattleAnimation(&constantTakingDamage);
		else
			animator.changeBattleAnimation(&standing);
	}
}

void Ogre::drawSprite(bool animate)
{
	updateDrawList(animate);
}

short Ogre::performBattleAction(short numAttacksPerformed)
{
	return 0;
}

/*
setColor
Parameters:
r: The red scaling on the rgb scale
g: The green scaling on the rgb scale
b: The blue scaling on the rgb scale
a: The alpha scaling

This method sets the color and transparency of a sprite
*/
void Ogre::setColor(int r, int g, int b, int a)
{
	battleSprite.setColor(sf::Color(r, g, b, a));
}

bool Ogre::moveToPosition(float x, float y)
{
	return false;
}

void Ogre::toggleCrushState()
{
	if (isCrushable)
		isCrushable = false;
	else
		isCrushable = true;
}