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

bool Tile::collidedWithTile(const sf::IntRect& rect, unsigned int row, unsigned int column, unsigned int tileSize)
{
	bool collision = false;

	//Create a bounding box for the current tile.
	sf::IntRect boundingBox(column * tileSize + bBX,
		row * tileSize + bBY,
		width,
		height);

	if (boundingBox.top == rect.height + rect.top)
	{
		tileCollision(sf::Vector2i(rect.top, rect.left));
		collision = true;
	}
	else
		collision = rect.intersects(boundingBox);

	if (collision)
		tileCollision(sf::Vector2i(rect.top, rect.left));

	//Check to see if the entity is inside of a objects colliding point
	return collidable == true ? collision : false;
}

void Animated_Tile::updateTile(const sf::Time& t)
{
	if (ANIMATION_TYPE == Collision && !tileInteractedWith)
		return;

	if (numLoops != 0 && currentLoop == numLoops)
	{
		currentLoop = 0;
		currentFrame = 0;
		tileInteractedWith = false;

		return;
	}

	float update = t.asSeconds() / updateInterval;

	// Divide by number of animation frames to see if they divide into the current time evenly or oddly
	if (int(update / numAnimationFrames) % 2 == 0)
		this->currentFrame = int(update) % numAnimationFrames;
	else
	{
		short frame = (numAnimationFrames - 1) - (int(update) % numAnimationFrames);
		
		if (frame != currentFrame)
		{
			this->currentFrame = frame;

			if (currentFrame == 0 && ANIMATION_TYPE != Infinite)
				currentLoop++;
		}
	}
}

void Animated_Tile::tileCollision(const sf::Vector2i& position)
{
	if (ANIMATION_TYPE != Collision || lastPositionUpdate == position)
		return;

	lastPositionUpdate = position;

	tileInteractedWith = true;
	currentLoop = 0;
	currentFrame = 0;
}