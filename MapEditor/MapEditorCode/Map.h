/*
Authors: Randy Brunecz, Joshua Gray
Map
9/5/2015

This class handles creates and draws a map. It also performs other map related useful functions.
*/

#ifndef Map_H
#define Map_H

#include "Graphic.h"
#include "Animation.h"
#include <SFML\Graphics.hpp>
#include <vector>

class Map : public Graphic
{
public:
	//PUBLIC FUNCTIONS
	Map();
	~Map();
	
	void initializeTileSheetCoords(sf::RenderWindow* window);
	void draw(sf::RenderWindow* window, sf::Vector2f mouseCoords);
	void setColor(int r, int g, int b, int a);
	void loadMap(std::string mapName, Camera* camera);
	void createMap(unsigned int rows, unsigned int columns, Camera* camera, std::string mapName);
	void drawTileSheet(sf::RenderWindow* window);
	void setTile(sf::Vector2i mouseCoords);
	void addTileToPos();
	void deleteTileFromPos(int row, int column);
	void saveMap();

	static unsigned short getTileSize();

	bool isMapLoaded();

	sf::Vector2f mapSize();

	//DEBUG
	void displayCollsionLayer();
	void displayGridLayer();

private:
	//PRIVATE structures
	struct Tile //Contains information about a tile such as the row and column it is found in the tileSheet, and special transformation information.
	{
		unsigned short row, column, transformation, height = 32, width = 32, bBX = 0, bBY = 0;
		bool collidable = false, hasTile = false;
		char tileType; //Indicates what kind of tile it is, such as water, grass, rock, object, etc.
		std::string mapName = "";
		sf::Vector2i transitionCoords;
		std::string boundingBox;
	};

	struct TransitionPoint
	{
		std::string transitionMapName;
		sf::Vector2i startingCoords;
		int numCoords;
		std::vector<sf::Vector2i> transitionPoints;
	};

	//PRIAVTE FUNCTIONS
	void initialize(std::ifstream& mapFile, Camera* camera);
	void initialize(Camera* camera);
	void initializeTransitionPoints(std::ifstream& mapFile);
	void populateMap(std::ifstream& mapFile);
	void drawMap();
	void drawToTexture(sf::RenderTexture& texture, Tile**& layer, int row, int column);
	void emptyMap();
	void createGrid();
	void updateMap(sf::RenderTexture& texture, Tile**& layer);
	void quickTextureDraw(sf::RenderTexture& texture, Tile**& layer, int row, int column);

	bool collisionDetected(sf::IntRect* rect);
	bool checkCollisionOnLayer(sf::IntRect* rect, Tile**& layer, int row, int column);
	bool isSameTile(Tile**& layer, int row, int column);

	unsigned short addTileToMap(Tile** layer, std::string tile, unsigned int pos, unsigned short row, unsigned short column);

	std::string tileToString(Tile**& layer, int row, int column);

	//PRIVATE CONSTANTS
	static const unsigned short TILE_SIZE = 32; //This is the width and height of each of the tiles
	const unsigned short NUM_WATER_FRAMES = 4;
	const unsigned int NUM_TILES_IN_SHEET = 200;

	Tile** map; //This 2d array contains the all of the tile information needed for the map
	Tile** ground; //This 2d contains objects like bases trees, rocks, etc.
	Tile** canopy; //This 2d contains objects like trees, top of rocks, etc.
	Tile** mask; //This layer contains masks that are layered on top of other tiles to give depth
	sf::Sprite tiles, mapSprite, canopySprite, groundSprite, maskSprite, waterSprite;
	sf::Texture tileSheet, deleteTex, transitionTex;
	sf::RenderTexture mapTexture, canopyTexture, groundTexture, maskTexture;
	sf::RenderTexture waterFrames[4];
	sf::Clock waterAnimation;
	Animation::WaterDirection waterShift = Animation::ShiftRight;

	sf::RectangleShape mousePos, selectedTile, deleteTile, transitionTile;

	unsigned short currentWaterFrame = 0;
	int numRows, numColumns; //numRows and numColumns contain the total number of rows and columns in the array
	int numTransitionPoints = 0;
	bool mapLoaded = false;
	sf::Vector2i tileSheetCoords;
	std::string tileData[200];
	std::string currentTile = "No Tile", nameOfFile = "NULL", nameOfTileSheet = "NULL", nameOfSheetFile = "NULL"; //Options are tile data, "No Tile", and "Delete"
	std::vector<TransitionPoint> transitions; //Vector containing information about all transition points in a map

	//TOOLS
	sf::RenderTexture collisionTexture, gridTexture;
	sf::Sprite collisionSprite, gridSprite;
	bool renderCollisionLayer = false, renderGridLayer = false;
};

#endif