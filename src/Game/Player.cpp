/*
Player.cpp
This class contains information about the player character such as position, graphic used and
characteristics.

@author Randall Brunecz
@version 1.0 9/3/2015
*/

#include "Game\Player.h"
#include "Game\Game.h"

/*
constructor
Parameters:
	window: The game window. Used to get the window size.

Creates the main characters sprite and sets other important information.
*/
Player::Player()
{
	// Initialize the height and width of the battle sprite
	battleHeight = 96;
	battleWidth = 96;

	pb::addToCollisionList(this);

	// Load sprite maps
	character.sprite.setTexture(*Game::graphicManager->addTexture("test.png"));

	// Set up the sprites properties
	character.sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
	character.sprite.setOrigin(16, 27);

	// Set up the sprites shadow properties
	character.shadow.setRadius(8);
	character.shadow.setFillColor(sf::Color(0, 0, 0, 110));

	character.setPosition(x, y);

	Game::camera->updatePosition(sf::Vector2f(x, y));
}

Player::Player(const Player& p) : Battle_Object(p)
{
	state = p.state;
	currentDirection = p.currentDirection;
	characterAniClock = p.characterAniClock;
	x = p.x;
	y = p.y;

	// Allocate memory
	uppercut = new Attack();
	arielSlash = new Attack();
	chop = new Attack();

	if(p.uppercut != NULL) *uppercut = *p.uppercut;
	if (p.arielSlash != NULL) *arielSlash = *p.arielSlash;
	if (p.chop != NULL) *chop = *p.chop;
}

const Player &Player::operator=(const Player& p)
{
	// Check if the objects are the same
	if (this == &p)
		return *this;

	state = p.state;
	currentDirection = p.currentDirection;
	characterAniClock = p.characterAniClock;
	x = p.x;
	y = p.y;

	Battle_Object::operator=(p);

	// Free memory
	delete uppercut;
	delete arielSlash;
	delete chop;

	// Allocate memory
	uppercut = new Attack();
	arielSlash = new Attack();
	chop = new Attack();

	if (p.uppercut != NULL) *uppercut = *p.uppercut;
	if (p.arielSlash != NULL) *arielSlash = *p.arielSlash;
	if (p.chop != NULL) *chop = *p.chop;

	return *this;
}

Player::~Player()
{
	delete uppercut;
	delete chop;
	delete arielSlash;
}

void Player::changePlayerState(const States state)
{
	this->state = state;

	// Free battle memory when changing player states
	delete uppercut;
	delete chop;
	delete arielSlash;

	delete moving;
	delete returning;
	delete standing;
	delete attackStance;
	delete arielStance;

	// Set the pointers to NULL to avoid a potential double delete
	uppercut = NULL;
	chop = NULL;
	arielSlash = NULL;

	moving = NULL;
	returning = NULL;
	standing = NULL;
	attackStance = NULL;
	arielStance = NULL;

	switch (this->state)
	{
	case World:
		// Load sprite maps
		character.sprite.setTexture(*Game::graphicManager->addTexture("test.png"));

		// Set up the sprites properties
		character.sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
		character.sprite.setOrigin(16, 27);

		// Set up the sprites shadow properties
		character.shadow.setRadius(8);
		character.shadow.setFillColor(sf::Color(0, 0, 0, 110));

		character.setPosition(x, y);
		Game::camera->updatePosition(sf::Vector2f(x, y));

		break;

	case Battle:
		battleSprite.setTexture(*Game::graphicManager->addTexture("trans_battle_test.png"));
		movementVelocity = 2000;

		// Set up the battle sprite defualt sprite
		battleSprite.setTextureRect(sf::IntRect(0, 0, 96, 96));
		battleSprite.setOrigin(48, 48);
		battleSprite.setPosition(960, 540);

		// Set up attack data
		uppercut = new Attack(6, 50, 100, 2, 300, 700, 10, battleHeight, battleWidth, battleHeight * 2, 0, true, false, new pb::Audio("uppercut.wav", 60), new pb::Audio("break.wav", 100));
		chop = new Attack(5, 50, 100, 3, 300, 0, 10, battleHeight, battleWidth, battleHeight, 0, false, true, new pb::Audio("chop.wav", 60));
		arielSlash = new Attack(5, 50, 100, 2, 300, 0, 10, battleHeight, battleWidth, battleHeight * 5, 0, false, false, new pb::Audio("chop.wav", 60));

		// Set up additional battle data
		moving = new Battle_Animation(3, 20, 70, 100, 3, 3, battleHeight, battleWidth, battleHeight * 3, 0, true);
		returning = new Battle_Animation(3, 20, 70, 100, 3, 3, battleHeight, battleWidth, battleHeight * 4, 0, true);
		standing = new Battle_Animation(1, 20, 20, 20, 1, 1, battleHeight, battleWidth, 0, 0, false);
		attackStance = new Battle_Animation(1, 20, 20, 20, 1, 1, battleHeight, battleWidth, 0, battleWidth, false);
		arielStance = new Battle_Animation(1, 20, 20, 20, 1, 1, battleHeight, battleWidth, 0, battleWidth * 2, false);

		animator.changeBattleAnimation(standing);

		break;
	}
}

/*
draw
Parameters:
	window: The game window

This method draws the character model on to the games window
*/
void Player::updateDrawList()
{
	switch (state)
	{
		case World:
			Game::graphicManager->addToDrawList(&character.shadow, true);
			Game::graphicManager->addToDrawList(&character.sprite, false);
			
			break;

		case Battle:
			Game::graphicManager->addToDrawList(&battleSprite, false);
			
			break;
	}
}

void Player::drawSprite(bool animate)
{
	if (animate)
		animator.animate(&battleSprite, &battleAniClock, isAttacking);
	
	updateDrawList();
}

short Player::performBattleAction(sf::Event lastKeyPressed, short numAttacksPerformed)
{
	if (!isAttacking)
	{
		if (lastKeyPressed.key.code == sf::Keyboard::W)
		{
			animator.changeAttackAnimation(chop);
			isAttacking = true;
		}
		else if (lastKeyPressed.key.code == sf::Keyboard::S)
		{
			animator.changeAttackAnimation(uppercut);
			isAttacking = true;
		}
		else if (isAirBound && lastKeyPressed.key.code == sf::Keyboard::A)
		{
			animator.changeAttackAnimation(arielSlash);
			isAttacking = true;
		}

		if (isAttacking)
			return ++numAttacksPerformed;
	}

	return numAttacksPerformed;
}

/*
updatePosition

This method takes player input (if there is any) and moves the player character
in a direction that the player chooses.
*/
void Player::updatePosition(const float elapsedTime)
{
	// LOCAL VARIABLES
	bool positionUpdated = true;
	float offSetX = 0, offSetY = 0;
	float offSetYDir = 0; // This variables allows for offsetting the player by a certain amount based on which direction the player is facing in the y direction

	// Move up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		offSetY = -VELOCITY * elapsedTime;
		offSetYDir = 5;

		currentDirection = Animation::Up; // Set the character direction state for animation purposes
	}
	// Move down
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		offSetY = VELOCITY * elapsedTime;

		currentDirection = Animation::Down; // Set the character direction state for animation purposes
	}	
	// Move right
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		offSetX = VELOCITY * elapsedTime;

		currentDirection = Animation::Right; // Set the character direction state for animation purposes
	}
	// Move left
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		offSetX = -VELOCITY * elapsedTime;

		currentDirection = Animation::Left; // Set the character direction state for animation purposes
	}
	else
		positionUpdated = false; // If the position was not updated, positionUpdated = false

	// Create a bounding box to check for collision
	sf::IntRect bb((int)(x - (WIDTH * 0.5f) + offSetX), (int)(y - offSetYDir + offSetY), WIDTH, HEIGHT);

	// May seem unintuitive to place y first then x. Think of it as y = rows and x = columns
	if (positionUpdated && pb::collisionDetected(bb))
	{
		Animation::updateAnimation(false, currentDirection, &characterAniClock, &character.sprite); // Update the characters movement animation
		return;
	}

	// Update positions
	x += offSetX;
	y += offSetY;

	// Update the position of the camera and character. Do not update the camera position if it is at the end of the map.
	character.setPosition(x, y);
	Game::camera->updatePosition(sf::Vector2f(x, y));

	Animation::updateAnimation(positionUpdated, currentDirection, &characterAniClock, &character.sprite); // Update the characters movement animation
}

/*
setPlayerPosition
Parameters:
	coords: These are the coordinates that the players position will be set to

This updates the players position in the game world.
*/
void Player::setPosition(const sf::Vector2f coords)
{
	x = coords.x;
	y = coords.y;

	character.setPosition(x, y);

	Animation::updateAnimation(false, currentDirection, &characterAniClock, &character.sprite); // Update the characters animation
}

/*
getPlayerCoordinates
return:
	This method returns the players current position in the game world

This method returns the players bounding box so that the calling method has information about the players position and the players height and width.
*/
const sf::IntRect Player::getRect()
{
	return sf::IntRect((int)(x - (WIDTH * 0.5f)), (int)y, WIDTH, HEIGHT);
}

bool Player::collisionDetected(const sf::IntRect& rect)
{
	return false;
}