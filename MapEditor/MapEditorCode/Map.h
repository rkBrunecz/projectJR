/*
Map.h
This class handles creating and drawing a map. It contains additional functionality in the map editor to allow a user to easily manipulate
tile using a fairly simple user interface.

@author Randall Brunecz
@version 2.0 3/23/2016

@version 1.0 9/5/2015
*/

#ifndef Map_H
#define Map_H

#include "Graphic.h"
#include "Animation.h"
#include "UI.h"
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
	void createMap(unsigned int rows, unsigned int columns, Camera* camera, std::string mapName, std::string sheetFileName);
	void drawTileSheet(sf::RenderWindow* window, sf::Vector2f mousePos);
	void setTile(sf::Vector2i mouseCoords);
	void addTileToPos();
	void saveMap();
	void allowTileManipulation();
	void forceUpdate();

	static unsigned short getTileSize();

	bool isMapLoaded();

	sf::Vector2f mapSize();

	//TOOLS
	void displayCollsionLayer();
	void displayGridLayer();
	void displayTransitionLayer();

private:
	//PRIVATE structures
	struct Tile //Contains information about a tile such as the row and column it is found in the tileSheet, and special transformation information.
	{
		unsigned short row, column, rotation, height = 32, width = 32, bBX = 0, bBY = 0;
		bool collidable = false, hasTile = false, mirror = false;
		char tileType; //Indicates what kind of tile it is, such as water, grass, rock, object, etc.
		std::string mapName = "";
		sf::Vector2i transitionCoords;
		std::string boundingBox = "none";

		std::string toString()
		{
			std::string s = std::to_string(row) + std::to_string(column) + std::to_string(rotation) + std::to_string(mirror) + std::to_string(collidable) + tileType;

			if (boundingBox.compare("none") != 0)
				return  s + ":" + boundingBox;
			else
				return s;
		}
	};

	struct TransitionPoint
	{
		std::string transitionMapName;
		sf::Vector2i startingCoords;
		int numCoords = 0;
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
	void deleteTileFromPos(int row, int column);
	void deleteTransitionPoint(int row, int column);
	void rotateTile(int row, int column);
	void mirrorTileAtPos(int row, int column);
	void setTransitionPoint(int row, int column);

	bool collisionDetected(sf::IntRect* rect);
	bool checkCollisionOnLayer(sf::IntRect* rect, Tile**& layer, int row, int column);
	bool isSameTile(Tile**& layer, int row, int column);

	unsigned short addTileToMap(Tile** layer, std::string tile, unsigned int pos, unsigned short row, unsigned short column);

	int boolToString(bool b);

	std::string tileToString(Tile**& layer, int row, int column);

	//PRIVATE CONSTANTS
	static const unsigned short TILE_SIZE = 32; //This is the width and height of each of the tiles
	const unsigned short NUM_WATER_FRAMES = 4;
	const unsigned int NUM_TILES_IN_SHEET = 200;

	Tile** map; //This 2d array contains the all of the tile information needed for the map
	Tile** ground; //This 2d contains objects like bases trees, rocks, etc.
	Tile** canopy; //This 2d contains objects like trees, top of rocks, etc.
	Tile** mask; //This layer contains masks that are layered on top of other tiles to give depth
	sf::Sprite* groundSprites;
	sf::Sprite tiles, mapSprite, canopySprite, maskSprite, waterSprite;
	sf::Texture tileSheet, deleteTex, transitionTex, deleteTransTex, rotateTex, mirrorTex;
	sf::RenderTexture mapTexture, canopyTexture, groundTexture, maskTexture;
	sf::RenderTexture waterFrames[4];
	sf::Clock waterAnimation;
	sf::Text currentRowColumn;
	sf::Font font;
	Animation::WaterDirection waterShift = Animation::ShiftRight;

	sf::RectangleShape mousePos, selectedTile, deleteTile, transitionTile, rotationTile, mirrorTile, deleteTransTile;

	unsigned short currentWaterFrame = 0;
	int numRows, numColumns; //numRows and numColumns contain the total number of rows and columns in the array
	int numTransitionPoints = 0;
	bool mapLoaded = false, tileRotatedRecently = false, tileMirroredRecently = false, tileDeletedRecently = false, transitionPlacedRecently = false, transitionRemovedRecently = false;
	sf::Vector2i tileSheetCoords;
	std::string tileData[200];
	std::string currentTile = "No Tile", nameOfFile = "NULL", nameOfTileSheet = "NULL", nameOfSheetFile = "NULL"; //Options are tile data, "No Tile", "Rotate", "Transition", "Mirror", "DeleteTransition", and "Delete"
	std::vector<TransitionPoint> transitions; //Vector containing information about all transition points in a map

	//TOOLS
	sf::RenderTexture collisionTexture, gridTexture, transitionTexture;
	sf::Sprite collisionSprite, gridSprite, transitionSprite;
	bool renderCollisionLayer = false, renderGridLayer = false, renderTransitionLayer = false;
};

#endif