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

private:
	virtual void updateTile(const sf::Time& t) { };
};

class Animated_Tile : public Tile
{
public:
	// Variables
	unsigned short numAnimationFrames = 0;
	unsigned short numLoops = 0; // If 0, the tile animates infinitely. 
	float updateInterval = 0.f;

private:
	unsigned short currentLoop = 0, currentFrame = 0;

	void updateTile(const sf::Time& t);
};

#endif;