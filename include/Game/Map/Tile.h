#ifndef Tile_H
#define Tile_H

#include "SFML\System\Vector2.hpp"
#include "SFML\System\Time.hpp"
#include "PBE\Graphics\Light.h"
#include <string>

// Contains information about a tile such as the row and column it is found in the tileSheet, and special transformation information.
class Tile
{
public:
	static bool lightsOn;

	// Variables
	unsigned short row, column, rotation, height = 32, width = 32, bBX = 0, bBY = 0;
	bool collidable = false, hasTile = false, mirror = false;
	char tileType; //Indicates what kind of tile it is, such as water, grass, rock, object, etc.
	sf::Vector2f transitionCoords;
	std::string mapName = "";
	std::string boundingBoxString = "none";
	pb::Light *light = 0;

	// Constructor
	Tile();

	// Destructor
	~Tile();

	// Copy constructor
	Tile(const Tile& t);

	// Assignment opertator
	const Tile &operator= (const Tile& t);

	std::string toString();

	void update(const sf::Time& t);

	bool collidedWithTile(const sf::IntRect& rect, unsigned int row, unsigned int column, unsigned int tileSize);

	virtual void interact() { };

	virtual unsigned short getColumn() { return column; };

	virtual pb::Light *getLight() 
	{	
		if (!lightsOn)
			return 0;

		return light;
	}

private:
	virtual void tileCollision(const sf::Vector2i& position) { };
	virtual void updateTile(const sf::Time& t) { };
};

class Animated_Tile : public Tile
{
public:
	const enum Type
	{
		Collision,
		Toggle,
		Infinite
	} ANIMATION_TYPE;

	// Variables
	sf::Vector2i lastPositionUpdate = sf::Vector2i(-1, -1);
	unsigned short numAnimationFrames = 0;
	unsigned short numLoops = 0; // If 0, the tile animates infinitely. 
	bool tileInteractedWith;
	float updateInterval = 0.f;

	Animated_Tile(Type t) : ANIMATION_TYPE(t), Tile() { };

	void interact();

	pb::Light *getLight();

private:
	unsigned short currentLoop = 0, currentFrame = 0;

	void tileCollision(const sf::Vector2i& position);
	void updateTile(const sf::Time& t);

	unsigned short getColumn() { return column + currentFrame; }
};

#endif;