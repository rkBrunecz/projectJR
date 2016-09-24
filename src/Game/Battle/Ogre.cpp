#include "Game\Battle\Ogre.h"
#include "Game\Game.h"

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

	// Load sprite map
	battleSprite.setTexture(*Game::graphicManager->addTexture("Ogre_test.png"));

	// Set up the battle sprite defualt sprite
	battleSprite.setTextureRect(sf::IntRect(0, 0, 96, 96));
	battleSprite.setOrigin(48, 48);

	takingDamage = new Battle_Animation(3, 70, 70, 70, 1, 3, battleHeight, battleWidth, battleHeight, 0, true);
	constantTakingDamage = new Battle_Animation(3, 100, 100, 100, 1, 3, battleHeight, battleWidth, battleHeight, 0, false);
	standing = new Battle_Animation(3, 100, 300, 300, 1, 3, battleHeight, battleWidth, 0, 0, false);

	animator.changeBattleAnimation(standing);
}

void Ogre::drawSprite(bool animate)
{
	if (animate)
		animator.animate(&battleSprite, &battleAniClock, isAttacking);

	Game::graphicManager->addToDrawList(&battleSprite, false);

	if (isTakingDamage && !animator.getCurrentAnimation()->isAnimating(&battleSprite))
	{
		isTakingDamage = false;

		if (isAirBound)
			animator.changeBattleAnimation(constantTakingDamage);
		else
			animator.changeBattleAnimation(standing);
	}
}

short Ogre::performBattleAction(short numAttacksPerformed)
{
	return 0;
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