/*
Authors: Randy Brunecz, Joshua Gray
Map
9/5/2015

This class handles creates and draws a map. It also performs other map related useful functions.
*/

#ifndef Map_H
#define Map_H

#include "Graphic.h"
#include <SFML\Graphics.hpp>

class Map : public Graphic
{
public:
	//PUBLIC FUNCTIONS
	Map(Camera* camera);
	~Map();
	void draw(sf::RenderWindow* window);
	void setColor(int r, int g, int b, int a);

private:
	//PRIAVTE FUNCTIONS
	void initialize(std::ifstream& mapFile, Camera* camera);
	void populateArray(std::ifstream& mapFile);
	void drawMap();

	//PRIVATE CONSTANTS
	int TILE_SIZE = 32; //This is the width and height of each of the tiles

	//PRIVATE VARIABLES
	struct Tile //Contains information about a tile such as the row and column it is found in the tileSheet, and special transformation information.
	{
		unsigned short row, column, transformation;
	};
	Tile** map; //This is the 2d array that contains the all of the tile information needed for the map
	sf::Sprite tiles, mapSprite;
	sf::Texture tileSheet;
	sf::RenderTexture mapTexture;
	int numRows, numColumns; //numRows and numColumns contain the total number of rows and columns in the array
};

#endif