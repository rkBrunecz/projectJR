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
	virtual void updateDrawList(Player* player, bool animate);
	virtual void loadMap(std::string mapName);
	
	const std::string moveToMap(Player* player);
	
	const sf::Vector2i getMapSize();

	bool transitioning(Player* player);

	//DEBUG
	void displayCollsionLayer();
	void displayGridLayer();
	void displayTransitionLayer();

protected:
	//PRIVATE structures
	struct Tile //Contains information about a tile such as the row and column it is found in the tileSheet, and special transformation information.
	{
		unsigned short row, column, rotation, height = 32, width = 32, bBX = 0, bBY = 0;
		bool collidable = false, hasTile = false, mirror = false;
		char tileType; //Indicates what kind of tile it is, such as water, grass, rock, object, etc.
		std::string mapName = "";
		sf::Vector2f transitionCoords;
		std::string boundingBox = "none";

		std::string toString()
		{
			std::string s = std::to_string(row) + std::to_string(column) + std::to_string(rotation) + std::to_string(mirror) + std::to_string(collidable) +tileType;

			if (boundingBox.compare("none") != 0)
				return  s + ":" + boundingBox;
			else
				return s;
		}
	};

	//PRIAVTE FUNCTIONS
	virtual void initialize(std::ifstream& mapFile);
	virtual void initializeTransitionPoints(std::ifstream& mapFile);
	void populateMap(std::ifstream& mapFile);
	void drawMap();
	void drawToTexture(sf::RenderTexture& texture, Tile**& layer, int row, int column);
	void clearRenderTextures(sf::Color color);
	void emptyMap();

	unsigned short addTileToMap(Tile** layer, std::string tile, unsigned int pos, unsigned short row, unsigned short column);

	//PRIVATE CONSTANTS
	static const unsigned short TILE_SIZE = 32; //This is the width and height of each of the tiles
	const unsigned short NUM_WATER_FRAMES = 4;

	Tile** map = 0; //This 2d array contains the all of the tile information needed for the map
	Tile** ground = 0; //This 2d contains objects like bases trees, rocks, etc.
	Tile** canopy = 0; //This 2d contains objects like trees, top of rocks, etc.
	Tile** mask = 0; //This layer contains masks that are layered on top of other tiles to give depth
	sf::Sprite* groundSprites;
	sf::Sprite tiles, mapSprite, canopySprite, maskSprite, waterSprite;
	sf::RenderTexture mapTexture, canopyTexture, groundTexture, maskTexture;
	sf::RenderTexture waterFrames[4];
	sf::Clock waterAniClock;
	Water *waterAnimator = 0;

	int numRows, numColumns; //numRows and numColumns contain the total number of rows and columns in the array

	//TOOLS
	sf::RenderTexture collisionTexture, gridTexture, transitionTexture;
	sf::Sprite collisionSprite, gridSprite, transitionSprite;
	bool renderCollisionLayer = false, renderGridLayer = false, renderTransitionLayer = false;

private:
	// Collision methods
	bool collisionDetected(const sf::IntRect& rect);
	bool checkCollisionOnLayer(const sf::IntRect& rect, Tile**& layer, int row, int column);

};

#endif