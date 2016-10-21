#include "Game\Map\Tile.h"

bool Tile::lightsOn = false;

// Constructor
Tile::Tile()
{

}

// Destructor
Tile::~Tile()
{
	delete light;
}

// Copy constructor
Tile::Tile(const Tile& t)
{
	// Unsigned shorts types
	row = t.row;
	column = t.column;
	rotation = t.rotation;
	height = t.height;
	width = t.width;
	bBX = t.bBX;
	bBY = t.bBY;

	// Boolean types
	collidable = t.collidable;
	hasTile = t.hasTile;
	mirror = t.mirror;

	// Character types
	tileType = t.tileType;

	// sf::Vector2f types
	transitionCoords = t.transitionCoords;

	// std::string types
	mapName = t.mapName;
	boundingBoxString = t.boundingBoxString;

	// Light
	if (t.light != 0)
	{
		delete light;
		light = new pb::Light(*t.light);
	}
}

// Assignment opertator
const Tile &Tile::operator= (const Tile& t)
{
	if (this == &t)
		return *this;

	// Unsigned shorts types
	row = t.row;
	column = t.column;
	rotation = t.rotation;
	height = t.height;
	width = t.width;
	bBX = t.bBX;
	bBY = t.bBY;

	// Boolean types
	collidable = t.collidable;
	hasTile = t.hasTile;
	mirror = t.mirror;

	// Character types
	tileType = t.tileType;

	// sf::Vector2f types
	transitionCoords = t.transitionCoords;

	// std::string types
	mapName = t.mapName;
	boundingBoxString = t.boundingBoxString;

	// Light
	if (t.light != 0)
	{
		delete light;
		light = new pb::Light(*t.light);
	}

	return *this;
}

std::string Tile::toString()
{
	std::string s = std::to_string(row) + std::to_string(column) + std::to_string(rotation) + std::to_string(mirror) + std::to_string(collidable) + tileType;

	return s;
}

void Tile::update(const sf::Time& t)
{
	updateTile(t);
}

void Animated_Tile::updateTile(const sf::Time& t)
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