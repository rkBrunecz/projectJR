/*
Authors: Randy Brunecz, Joshua Gray
Map
9/5/2015

This class handles creates and draws a map. It also performs other useful functions.
*/

#include "Map.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

/*
Map 

The constructor of the map class creates a map that can be drawn.
*/
Map::Map()
{
	//LOCAL VARIABLES
	std::ifstream mapFile;

	//Open the mapFile file
	mapFile.open("bin/Maps/TestMap.jrm");

	if (mapFile.is_open())
	{
		initialize(mapFile); //Dynamically create an array to hold the map
		populateArray(mapFile); //Fill the array with the map data
	}
	else
		exit(EXIT_FAILURE); //Close the application with a failure code if the file does not open

	mapFile.close(); //Close the file
}

/*
initialize
Parameters:
	mapFile: This variable contains the file that will be read to initailize different variables.

The initialize method loads the tilesheet needed to draw the map; dynamically creates an array to hold all
of the tile objects; and stores the size of the created 2d array in variables numRow and numColumn.
*/
void Map::initialize(std::ifstream& mapFile)
{
	//LOCAL VARIABLES
	std::string input, //Used to get input from the getline method from the file
				mapRows,  //Used to store the total number of rows in the map that is read from the file.
				mapColumns; //Used to store the total number of columns in the map that is read from the file.

	//Get the path to the tile map and then open it.
	std::getline(mapFile, input);
	if (!tileSheet.loadFromFile(input))
		exit(EXIT_FAILURE); //Exit the application with a failure code if the tile map does not load

	tiles.setTexture(tileSheet);

	//Get the total number of rows and columns from the file and store them
	std::getline(mapFile, mapRows, 'x');
	std::getline(mapFile, mapColumns);

	//Convert the parsed strings to ints
	numRows = atoi(mapRows.c_str());
	numColumns = atoi(mapColumns.c_str());

	//Dynamically Create an array to hold the map
	map = new Tile*[numRows];
	for (int i = 0; i < numRows; i++)
		map[i] = new Tile[numColumns];
}

/*
populateArray
Parameters:
	mapFile: This variable contains the file that will be read to initailize the array.

This method populates the array that was dynamically created previously.
*/
void Map::populateArray(std::ifstream& mapFile)
{
	//LOCAL VARIABLES
	std::string input; //Used to get input from the getline method from the file
	int j = 0; //Iterator for the row when populating the array.
	int x = 0;

	//While the end of file marker has not been reached, continue to read the file
	while (std::getline(mapFile, input))
	{
		//This loop instantiates all of the Tiles in the tile 2d array
		for (int i = 0; i < numColumns; i++)
		{
			map[j][i].row = (unsigned int)input[x] - '0';
			map[j][i].column = (unsigned int)input[x + 1] - '0';
			map[j][i].transformation = (unsigned int)input[x + 2] - '0';

			x += 4; //x determines where in the string read in the file to pull a character from. We add 4 to skip over the white spaces in the file
		}

		x = 0; //Reset x to 0
		j++; //Increment to the next row
	}
}

/*
draw
Parameters:
	window: This is the game window where the map will be draw

This method draws the map to the window
*/
void Map::draw(sf::RenderWindow* window)
{
	//Step through each row in the maps array.
	for (int i = 0; i < numRows; i++)
	{
		//Step through each columns in the maps row
		for (int j = 0; j < numColumns; j++)
		{
			tiles.setPosition(j * tileSize, i * tileSize); //Set the position of the tile to be drawn
			
			//Set the part of the tile map to draw to the window
			tiles.setTextureRect(sf::IntRect((map[i][j].column * tileSize) - tileSize, 
											 (map[i][j].row * tileSize) - tileSize, 
											  map[i][j].column * tileSize, 
											  map[i][j].row * tileSize)); 
			
			window->draw(tiles); //Draw the tile
		}			
	}
}

/*
setColor
Parameters:
	r: The red scaling on the rgb scale
	g: The green scaling on the rgb scale
	b: The blue scaling on the rgb scale
	a: The alpha scaling

This method sets the color and transparency of a the tiles
*/
void Map::setColor(int r, int g, int b, int a)
{
	tiles.setColor(sf::Color(r, g, b, a));
}

/*
~Map

This destructor frees up the memory that was dynamically allocated for the 
2d array that stores the map.
*/
Map::~Map()
{
	//Free memory that was allocated for the map array
	for (int i = 0; i < numRows; i++)
		delete map[i];
	delete[] map;
}

/*
toString

This is a debug function that creates a string the data stored in the map array.
*/
std::string Map::toString()
{
	//LOCAL VARIABLES
	std::stringstream ss;

	//Step through each row in the maps array.
	for (int i = 0; i < numRows; i++)
	{
		//Step through each column in the maps array.
		for (int j = 0; j < numColumns; j++)
		{
			//If not at the end of the column, add more data to the string
			if (j != numColumns - 1)
				ss << map[i][j].row << map[i][j].column << map[i][j].transformation << " ";
			else
				ss << "\n"; //Add a end of the line character to the string if j == numColumns
		}
	}

	return ss.str(); //Return the string
}