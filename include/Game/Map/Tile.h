#ifndef Tile_H
#define Tile_H

#define _USE_MATH_DEFINES

#include <string>
#include "SFML\System\Vector2.hpp"
#include <math.h>

class Tile //Contains information about a tile such as the row and column it is found in the tileSheet, and special transformation information.
{
public:
	// Variables
	unsigned short row, column, rotation, height = 32, width = 32, bBX = 0, bBY = 0;
	bool collidable = false, hasTile = false, mirror = false;
	char tileType; //Indicates what kind of tile it is, such as water, grass, rock, object, etc.
	std::string mapName = "";
	sf::Vector2f transitionCoords;
	std::string boundingBoxString = "none";
	sf::IntRect boundingBox;

	std::string toString()
	{
		std::string s = std::to_string(row) + std::to_string(column) + std::to_string(rotation) + std::to_string(mirror) + std::to_string(collidable) + tileType;

		return s;
	}

	void update(const sf::Time& t)
	{
		updateTile(t);
	}

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

	void updateTile(const sf::Time& t)
	{
		if (numLoops != 0 && currentLoop == numLoops)
		{
			currentLoop = 0;
			return;
		}

		float update = t.asSeconds() / updateInterval;

		if (int(update / numAnimationFrames) % 2 == 0)
			this->column = int(update) % numAnimationFrames;
		else
			this->column = (numAnimationFrames - 1) - (int(update) % numAnimationFrames);
	}
};

#endif;