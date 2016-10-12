/*
Map.h
This class handles creating and drawing a map. It also performs other map related useful functions. This object is NON-COPYABLE.

@author Randall Brunecz
@version 2.0 3/23/2016
	-Overhauled how the map draws itself. Will allow for straight forward implementation of NPC's. The new version is also a large optimization over the previous implementation.

@version 1.0 9/5/2015
*/

#ifndef Map_H
#define Map_H

#include <SFML\Graphics.hpp>
#include <fstream>
#include <sstream>
#include "PBE\System\Collision.h"
#include "Game\Map\Layer.h"
#include "Game\Map\Primitive_Layer.h"
#include "Game\Map\Tile.h"
#include "Game\World_Animations.h"
#include "Game\Player.h"

class Map : public pb::Collidable
{
public:
	// Constructor
	Map() { };
	Map(std::string mapName);
	
	// Copy constructor
	Map(const Map& m) = delete;

	// Operators
	const Map &operator=(const Map& m) = delete;

	// Destructor
	~Map();

	//PUBLIC FUNCTIONS
	virtual void updateDrawList(Player* player, const sf::Time& currentTime, bool animate);
	virtual void loadMap(std::string mapName);
	
	const std::string moveToMap(Player* player);
	
	const sf::Vector2i getMapSize();

	bool transitioning(Player* player);

	//DEBUG tools
	void displayCollsionLayer();
	void displayGridLayer();
	void displayTransitionLayer();

protected:
	// Protected methods
	void initializeTileData(const std::string& jrsFile);
	void initializeTile(const std::string& input, int inputPos, Tile* t);

	virtual void initialize(std::ifstream& mapFile);
	virtual void initializeTransitionPoints(std::ifstream& mapFile);

	void populateMap(std::ifstream& mapFile);
	void updateBoundingBox(Tile *t);

	void emptyMap();

	unsigned short addTileToMap(const std::string& input, unsigned short pos, unsigned short row, unsigned short column);

	struct Tile_Data
	{
		enum Tile_Type
		{
			Normal, 
			Animated
		};

		Tile_Type type;
		Tile *t = 0;
		Animated_Tile *at = 0;
		unsigned short layer;

		Tile *getTile()
		{
			if (t != 0)
				return t;
			else if (at != 0)
				return at;
			else
				return 0;
		}

		~Tile_Data() 
		{ 
			delete t;
			delete at;
		}
	};
	std::vector<Tile_Data *> tileData;

	//PRIVATE CONSTANTS
	static const unsigned short TILE_SIZE = 32; //This is the width and height of each of the tiles

	Layer *mapLayer, *canopyLayer, *maskLayer, 
		**groundLayers;

	unsigned int numRows, numColumns, tileDataColumns; //numRows and numColumns contain the total number of rows and columns in the array

	//TOOLS
	Primitive_Layer *collisionLayer, *transitionLayer, *gridLayer;
	bool renderCollisionLayer = false, renderGridLayer = false, renderTransitionLayer = false;

private:
	// Collision methods
	bool collisionDetected(const sf::IntRect& rect);
};

#endif