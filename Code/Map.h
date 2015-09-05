/*
Authors: Randy Brunecz, Joshua Gray
Animation
9/4/2015

This class handles animations for sprites
*/

#ifndef Map_H
#define Map_H

#include <SFML\Graphics.hpp>
#include "Graphic.h"

class Map : public Graphic
{
public:
	//PUBLIC FUNCTIONS
	Map();
	~Map();
	std::string toString();
	void draw(sf::RenderWindow* window);

private:
	//PRIAVTE FUNCTIONS
	void createArray(std::ifstream& mapFile);
	void populateArray(std::ifstream& mapFile);

	//PRIVATE VARIABLES
	struct Tile
	{
		unsigned short row, column, transformation;
	};
	Tile** map;
	sf::Sprite tiles;
	sf::Texture tileSheet;
	int numRows, numColumns;
};

#endif