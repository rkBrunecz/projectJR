#include "Map.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

Map::Map()
{
	//LOCAL VARIABLES
	std::ifstream mapFile;

	//Open the mapFile file
	mapFile.open("bin/Maps/TestMap.jrm");

	if (mapFile.is_open())
	{
		createArray(mapFile); //Dynamically create an array to hold the map
		populateArray(mapFile); //Fill the array with the map data
	}
	else
		exit(EXIT_FAILURE);

	mapFile.close(); //Close the file
}

void Map::createArray(std::ifstream& mapFile)
{
	//LOCAL VARIABLES
	std::string input, mapX, mapY;

	std::getline(mapFile, input);
	if (!tileSheet.loadFromFile(input))
		exit(EXIT_FAILURE);

	tiles.setTexture(tileSheet);

	std::getline(mapFile, mapX, 'x');
	std::getline(mapFile, mapY);

	std::cout << mapX << "  " << mapY;

	//Convert the parsed strings 
	numRows = atoi(mapX.c_str());
	numColumns = atoi(mapY.c_str());

	std::cout << numRows << numColumns;

	//Dynamically Create an array to hold the map
	map = new Tile*[numRows];
	for (int i = 0; i < numRows; i++)
		map[i] = new Tile[numColumns];
}

void Map::populateArray(std::ifstream& mapFile)
{
	//LOCAL VARIABLES
	std::string input;
	int j = 0; //Iterator for the row when populating the array.

	while (std::getline(mapFile, input))
	{
		for (int x = 0, i = 0; x < numColumns; x++, i += 4)
		{
			map[j][x].row = (unsigned int)input[i] - '0';
			map[j][x].column = (unsigned int)input[i + 1] - '0';
			map[j][x].transformation = (unsigned int)input[i + 2] - '0';
		}

		j++;
	}
}

void Map::draw(sf::RenderWindow* window)
{
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numColumns; j++)
		{
			tiles.setPosition(j * 32, i * 32);
			tiles.setTextureRect(sf::IntRect((map[i][j].column * 32) - 32, (map[i][j].row * 32) - 32, map[i][j].column *32, map[i][j].row * 32));
			window->draw(tiles);
		}			
	}
}

void Map::setColor(int r, int g, int b, int a)
{
	tiles.setColor(sf::Color(r, g, b, a));
}

Map::~Map()
{
	//Free memory that was allocated for the map array
	for (int i = 0; i < numRows; i++)
		delete map[i];
	delete[] map;
}

std::string Map::toString()
{
	//LOCAL VARIABLES
	std::stringstream ss;

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numColumns; j++)
		{
			if (j != numColumns - 1)
				ss << map[i][j].row << map[i][j].column << map[i][j].transformation << " ";
			else
				ss << "\n";
		}
	}

	return ss.str();
}