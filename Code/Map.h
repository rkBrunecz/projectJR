/*
Authors: Randy Brunecz, Joshua Gray
Map
9/5/2015

This class handles creates and draws a map. It also performs other map related useful functions.
*/

#ifndef Map_H
#define Map_H

#include "Graphic.h"
#include "Player.h"
#include <SFML\Graphics.hpp>

class Map : public Graphic
{
public:
	//PUBLIC FUNCTIONS
	Map();
	~Map();
	
	void draw(sf::RenderWindow* window, Player* player);
	void setColor(int r, int g, int b, int a);
	void loadMap(std::string mapName, Camera* camera);
	void moveToMap(Player* player, Camera* camera);
	
	static bool tileCollision();
	bool transitioning(Player* player);

private:
	//PRIVATE structures
	struct Tile //Contains information about a tile such as the row and column it is found in the tileSheet, and special transformation information.
	{
		unsigned short row, column, transformation, height = 32, width = 32;
		bool collidable = false, transitionTile, hasTile = false;
		std::string mapName = "";
		sf::Vector2i transitionCoords;
	};

	//PRIAVTE FUNCTIONS
	void initialize(std::ifstream& mapFile, Camera* camera);
	void initializeTransitionPoints(std::ifstream& mapFile);
	void populateMap(std::ifstream& mapFile);
	void drawMap();
	void drawToTexture(sf::RenderTexture& texture, Tile**& layer, int row, int column);
	void emptyMap();

	bool collisionDetected(sf::IntRect* rect);
	bool checkCollisionOnLayer(sf::IntRect* rect, Tile**& layer);

	//PRIVATE CONSTANTS
	int TILE_SIZE = 32; //This is the width and height of each of the tiles

	Tile** map; //This 2d array contains the all of the tile information needed for the map
	Tile** ground; //This 2d contains objects like trees, rocks, etc.
	Tile** canopy; //This 2d contains objects like trees, rocks, etc.
	sf::Sprite tiles, mapSprite, canopySprite, groundSprite;
	sf::Texture tileSheet;
	sf::RenderTexture mapTexture, canopyTexture, groundTexture;
	
	int numRows, numColumns; //numRows and numColumns contain the total number of rows and columns in the array
};

#endif