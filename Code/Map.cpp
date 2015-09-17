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
Map::Map()
{

}

/*
loadMap
Parameters:
	mapName: This is the name of the map file that needs to be loaded in
	camera:  This is the game camera. This is modified to have new boundaries upon entering a new map.

Load map simply loads in all relavent map data to the map array to ready it to be displayer later.
*/
void Map::loadMap(std::string mapName, Camera* camera)
{
	//LOCAL VARIABLES
	std::ifstream mapFile;

	//If the rows and columns of the map have been set, you it is safe to clear out the map file.
	if (numRows != 0 && numColumns != 0)
		emptyMap(); //Empty out the map file to make room for a new map.

	//Open the mapFile file
	mapFile.open(mapName);

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

	//Recreate the mapTexture ONLY when needed
	if (mapTexture.getSize().x < numRows * TILE_SIZE && mapTexture.getSize().y < numColumns * TILE_SIZE)
		mapTexture.create(numColumns * TILE_SIZE, numRows * TILE_SIZE);

	camera->setBounds(numColumns * TILE_SIZE, numRows * TILE_SIZE);

	//Dynamically Create an array to hold the map
	map = new Tile*[numRows];
	for (int i = 0; i < numRows; i++)
		map[i] = new Tile[numColumns];

	initializeTransitionPoints(mapFile); //Intialize all transition points in the map

	populateMap(mapFile); //Fill the array with the map data

	drawMap(); //Draw the map
}

/*
initializeTransitionPoints
Parameters:
	mapFile: This file contains the information about the transition points in the map

This method reads the map file and sets the tiles indicated in the map file as transition points to another
map. The information stored in a tile include the coordinates in the next map the player will spawn at, and the 
name of the map file.
*/
void Map::initializeTransitionPoints(std::ifstream& mapFile)
{
	//LOCAL VARIABLES
	std::string input, mapFileName;
	int tileRow, tileColumn, numTransitionAreas, numCoords;
	sf::Vector2i coords;

	//Get the number connected maps
	std::getline(mapFile, input);
	numTransitionAreas = atoi(input.c_str());

	for (int i = 0; i < numTransitionAreas; i++)
	{
		//Get the file name for a connected map
		std::getline(mapFile, input, '-');
		mapFileName = input;

		//Get the coordinates that the player character will start on in the next map
		std::getline(mapFile, input, 'x');
		coords.y = atoi(input.c_str()) * TILE_SIZE;
		std::getline(mapFile, input, '-');
		coords.x = atoi(input.c_str()) * TILE_SIZE;

		//Get the total number of map tiles that move the same map. Allows for an area to be defined to allow for more map transition flexibility
		std::getline(mapFile, input, '-');
		numCoords = atoi(input.c_str());

		//Loop to find all possible map transition points and store the coordinates for the next map, and the map name to the file.
		for (int i = 0; i < numCoords; i++)
		{
			//Get the coordinates of a tile that is a transition point
			std::getline(mapFile, input, 'x');
			tileRow = atoi(input.c_str()) - 1;
			std::getline(mapFile, input, ',');
			tileColumn = atoi(input.c_str()) - 1;

			//Store the file name and coordinates for the spawning position in the next map
			map[tileRow][tileColumn].mapName = mapFileName;
			map[tileRow][tileColumn].transitionCoords = coords;
		}
	}

	//Clear out the line to get ready to read the map.
	std::getline(mapFile, input);
}

/*
populateArray
Parameters:
	mapFile: This variable contains the file that will be read to initailize the array.

This method populates the array that was dynamically created previously.
*/
void Map::populateMap(std::ifstream& mapFile)
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
			map[j][i].collidable = (unsigned int)input[x + 3] - '0'; //0 = false, 1 = true
			map[j][i].transitionTile = (unsigned int)input[x + 4] - '0'; //0 = false, 1 = true

			x += 6; //x determines where in the string read in the file to pull a character from. We add 4 to skip over the white spaces in the file
		}

		x = 0; //Reset x to 0
		j++; //Increment to the next row
	}
}

/*
emptyMap

This method simply releases the memory used to store the previous map.
*/
void Map::emptyMap()
{
	//Free memory that was allocated for the map array
	for (int i = 0; i < numRows; i++)
		delete[] map[i];
	delete[] map;
}

/*
drawMap

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
			
			if (map[i][j].transformation == 0)
			{
				tiles.setPosition(j * TILE_SIZE, i * TILE_SIZE); //Set the position of the tile to be drawn 
				
				mapTexture.draw(tiles); //Draw the tile
			}
			else if (map[i][j].transformation >= 1 && map[i][j].transformation <= 3) //Rotate the tile if the transformation value is between 1 and 3, inclusive
			{
				//Create a temporary texture to hold the tile
				sf::Texture temp;
				temp.loadFromImage(tiles.getTexture()->copyToImage(), tiles.getTextureRect());
				
				//Create a temporary sprite to display the rotation of sprites
				sf::Sprite tmp(temp);
				
				//The tiles are 32x32, so the origin is 16, 16
				tmp.setOrigin(16, 16);
				tmp.rotate(map[i][j].transformation * 90);

				tmp.setPosition(j * TILE_SIZE + 16, i * TILE_SIZE + 16); //Set the position of the tile to be drawn 

				mapTexture.draw(tmp); //Draw the tile
			}
			else if (map[i][j].transformation == 4) //Mirro tile
			{
				//Create a temporary texture to hold the tile
				sf::Texture temp;
				temp.loadFromImage(tiles.getTexture()->copyToImage(), tiles.getTextureRect());

				//Create a temporary sprite to display the rotation of sprites
				sf::Sprite tmp(temp);

				//The tiles are 32x32, so the origin is 16, 16
				tmp.setOrigin(16, 16);
				tmp.scale(-1.f, 1.f); //Mirror the tile

				tmp.setPosition(j * TILE_SIZE + 16, i * TILE_SIZE + 16); //Set the position of the tile to be drawn 

				mapTexture.draw(tmp); //Draw the tile
			}
		}			
	}

	mapTexture.display(); //Let the map texture know that it is no longer being drawn to

	mapSprite.setTexture(mapTexture.getTexture()); //Set the map texture to a sprite.
}

/*
draw
Parameters:
	window: This is the window that the map will be drawn on

Draws the map to the game window
*/
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
moveToMap
Parameters:
	player: The player character will have its position set to the indicated spawning point
	camera: The camera will have its position updated to center on the player

This method moves from one map to another when a transition tile is collided with.
*/
void Map::moveToMap(Player* player, Camera* camera)
{
	//LOCAL VARIABLES
	int row = player->getPlayerCoordinates().y / TILE_SIZE;
	int column = player->getPlayerCoordinates().x / TILE_SIZE;
	sf::Vector2i startPosition = map[row][column].transitionCoords;

	loadMap(map[row][column].mapName, camera); //Load the next map

	player->setPlayerPosition(startPosition); 
	camera->updatePosition(startPosition);
}

/*
collisionDetected
Parameters:
	rect: This rectangle contains the boundaries of some graphic entity that needs to be checked.

This method checks for collision between a tile and some entity that exists within the map. Returns true 
if collision has been detected. False otherwise.
*/
bool Map::collisionDetected(sf::FloatRect rect)
{
	//If the entity is at the maps edge along the x-axis, return true
	if ((rect.left - rect.width) < 0 || rect.left + rect.width > numColumns * TILE_SIZE)
		return true;

	//If the entity is at the maps edge along the y-axis, return true
	if (rect.top < 0 || rect.top + rect.height > numRows * TILE_SIZE)
		return true;

	//Check to see if one of the four sides of some entity has collided with a collidable tile
	if (map[(int)rect.top / TILE_SIZE][(int)(rect.left - rect.width) / TILE_SIZE].collidable) //Top left collision
		return true;
	else if (map[(int)rect.top / TILE_SIZE][(int)(rect.left + rect.width) / TILE_SIZE].collidable) //Top right collision
		return true;
	else if (map[(int)(rect.top + rect.height) / TILE_SIZE][(int)(rect.left - rect.width) / TILE_SIZE].collidable) //Bottom left collision
		return true;
	else if (map[(int)(rect.top + rect.height) / TILE_SIZE][(int)(rect.left + rect.width) / TILE_SIZE].collidable) //Bottom right collision
		return true;

	//No collision has been detected
	return false;
}

/*
transitioning

This method is used to determine if the game state should switch to the transitioning state or not.
*/
bool Map::transitioning(Player* player)
{
	//This checks to see if the tile being moved to is a transition tile
	if (map[(int)player->getPlayerCoordinates().y / TILE_SIZE][(int)player->getPlayerCoordinates().x / TILE_SIZE].transitionTile)
		return true; //No collision has been detected

	return false;
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
		delete[] map[i];
	delete[] map;
}