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

	walk = new Walk(3, 1, 2, 32, 32, 0.2f, 3 * 32, 0, 2 * 32, 32);
}

Player::Player(const Player& p) : Battle_Object(p)
{
	state = p.state;
	characterAniClock = p.characterAniClock;
	pos = p.pos;
	lastPos = p.lastPos;

	// Allocate memory
	uppercut = new Attack();
	arielSlash = new Attack();
	chop = new Attack();

	if(p.uppercut != 0) *uppercut = *p.uppercut;
	if (p.arielSlash != 0) *arielSlash = *p.arielSlash;
	if (p.chop != 0) *chop = *p.chop;
}

const Player &Player::operator=(const Player& p)
{
	// Check if the objects are the same
	if (this == &p)
		return *this;

	state = p.state;
	characterAniClock = p.characterAniClock;
	pos = p.pos;
	lastPos = p.lastPos;

	Battle_Object::operator=(p);

	// Free memory
	delete uppercut;
	delete arielSlash;
	delete chop;

	// Allocate memory
	uppercut = new Attack();
	arielSlash = new Attack();
	chop = new Attack();

	if (p.uppercut != 0) *uppercut = *p.uppercut;
	if (p.arielSlash != 0) *arielSlash = *p.arielSlash;
	if (p.chop != 0) *chop = *p.chop;

	return *this;
}

Player::~Player()
{
	delete uppercut;
	delete chop;
	delete arielSlash;
	
	delete walk;
}

void Player::loadState(const States state)
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
	uppercut = 0;
	chop = 0;
	arielSlash = 0;

	moving = 0;
	returning = 0;
	standing = 0;
	attackStance = 0;
	arielStance = 0;

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

		walk->updateWalkCycle(Walk::None, &characterAniClock);
		Game::camera->updatePosition(pos);

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
			walk->updateAnimation(&character.sprite);

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
	lastPos = pos;

	// LOCAL VARIABLES
	bool positionUpdated = true;
	Walk::WalkingDirection dir = Walk::None;
	float offSetX = 0, offSetY = 0;
	float offSetYDir = 0; // This variables allows for offsetting the player by a certain amount based on which direction the player is facing in the y direction

	// Move up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		offSetY = -VELOCITY * elapsedTime;
		offSetYDir = 5;

		dir = Walk::Up;
		facingDirection = Up;
	}
	// Move down
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		offSetY = VELOCITY * elapsedTime;

		dir = Walk::Down;
		facingDirection = Down;
	}	
	// Move right
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		offSetX = VELOCITY * elapsedTime;

		dir = Walk::Right;
		facingDirection = Right;
	}
	// Move left
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		offSetX = -VELOCITY * elapsedTime;

		dir = Walk::Left;
		facingDirection = Left;
	}

	walk->updateWalkCycle(dir, &characterAniClock);

	// Create a bounding box to check for collision
	sf::IntRect bb((int)(pos.x - (WIDTH * 0.5f) + offSetX), (int)(pos.y - offSetYDir + offSetY), WIDTH, HEIGHT);

	// May seem unintuitive to place y first then x. Think of it as y = rows and x = columns
	if (positionUpdated && pb::collisionDetected(bb))
	{
		velocity = sf::Vector2f(0, 0);
		walk->updateWalkCycle(Walk::None, &characterAniClock);
		return;
	}

	// Update positions
	velocity = sf::Vector2f(offSetX, offSetY);
	pos += velocity;
}

void Player::renderPosition(double alpha)
{
	// Predict the players next position
	sf::Vector2f posInterpolation = sf::Vector2f(float(pos.x * alpha + lastPos.x * (1.f - alpha)), float(pos.y * alpha + lastPos.y * (1.f - alpha)));

	// Update the position of the camera and character. Do not update the camera position if it is at the end of the map.
	character.setPosition((sf::Vector2i)posInterpolation);
	Game::camera->updatePosition(sf::Vector2f((sf::Vector2i)posInterpolation));
}

sf::Vector2u Player::getPosition()
{
	return sf::Vector2u(character.sprite.getPosition());
}

sf::Vector2u Player::getPosition(unsigned int tileSize)
{
	sf::Vector2u pos = sf::Vector2u(character.sprite.getPosition());

	switch (facingDirection)
	{
	case Up:
		pos.y = (pos.y - tileSize < 0 ? 0 : pos.y - tileSize);

		break;

	case Down:
		pos.y += tileSize;

		break;

	case Right:
		pos.x += tileSize;

		break;

	case Left:
		pos.x = (pos.x - tileSize < 0 ? 0 : pos.x - tileSize);

		break;
	}

	return pos;
}

/*
setPlayerPosition
Parameters:
	coords: These are the coordinates that the players position will be set to

This updates the players position in the game world.
*/
void Player::setPosition(const sf::Vector2f coords)
{
	pos = coords;
	lastPos = coords;

	// Update the position of the camera and character. Do not update the camera position if it is at the end of the map.
	character.setPosition((sf::Vector2i)pos);

	walk->updateWalkCycle(Walk::None, &characterAniClock);
}

/*
getPlayerCoordinates
return:
	This method returns the players current position in the game world

This method returns the players bounding box so that the calling method has information about the players position and the players height and width.
*/
const sf::IntRect Player::getRect()
{
	return sf::IntRect((int)(pos.x - (WIDTH * 0.5f)), (int)pos.y, WIDTH, HEIGHT);
}

bool Player::collisionDetected(const sf::IntRect& rect)
{
	return false;
}