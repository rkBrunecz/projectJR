/*
Authors: Randy Brunecz, Joshua Gray
Map
9/5/2015

This class handles creates and draws a map. It also performs other useful map related functions.
*/

#include "Map.h"
#include <fstream>
#include <sstream>
#include "Animation.h"

/*
Map 

The constructor of the map class creates a map that can be drawn.
*/
Map::Map(Camera* camera)
{
	//LOCAL VARIABLES
	std::ifstream mapFile;

	//Open the mapFile file
	mapFile.open("bin/Maps/TestMap.jrm");

	if (mapFile.is_open())
		initialize(mapFile, camera); //Dynamically create an array to hold the map		
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
void Map::initialize(std::ifstream& mapFile, Camera* camera)
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

	mapTexture.create(numColumns * TILE_SIZE, numRows * TILE_SIZE);
	camera->setBounds(numColumns * TILE_SIZE, numRows * TILE_SIZE);

	//Dynamically Create an array to hold the map
	map = new Tile*[numRows];
	for (int i = 0; i < numRows; i++)
		map[i] = new Tile[numColumns];

	populateArray(mapFile); //Fill the array with the map data

	drawMap();
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
	texture: Draws a map to an offscreen texture that can be added to the window when to display the map

This method draws the map statically to a texture.
*/
void Map::drawMap()
{
	mapTexture.clear();

	//Step through each row in the maps array.
	for (int i = 0; i <= numRows - 1; i++)
	{
		//Step through each columns in the maps row
		for (int j = 0; j <= numColumns - 1; j++)
		{
			//Set the part of the tile map to draw to the window
			tiles.setTextureRect(sf::IntRect(map[i][j].column * TILE_SIZE,
				map[i][j].row * TILE_SIZE,
				TILE_SIZE,
				TILE_SIZE));

			tiles.setPosition(j * TILE_SIZE, i * TILE_SIZE); //Set the position of the tile to be drawn 
				
			mapTexture.draw(tiles); //Draw the tile
		}			
	}

	mapTexture.display();

	mapSprite.setTexture(mapTexture.getTexture());
}

void Map::draw(sf::RenderWindow* window)
{
	window->draw(mapSprite);
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
	mapSprite.setColor(sf::Color(r, g, b, a));
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