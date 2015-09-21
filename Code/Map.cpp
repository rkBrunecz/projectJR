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

	//Recreate the mapTexture, canopyTexture and groundTexture ONLY when needed
	if (mapTexture.getSize().x < numRows * TILE_SIZE && mapTexture.getSize().y < numColumns * TILE_SIZE)
	{
		mapTexture.create(numColumns * TILE_SIZE, numRows * TILE_SIZE);
		canopyTexture.create(numColumns * TILE_SIZE, numRows * TILE_SIZE);
		groundTexture.create(numColumns * TILE_SIZE, numRows * TILE_SIZE);
	}

	camera->setBounds(numColumns * TILE_SIZE, numRows * TILE_SIZE);

	//Dynamically Create the arrays to hold the map
	map = new Tile*[numRows];
	canopy = new Tile*[numRows];
	ground = new Tile*[numRows];
	for (int i = 0; i < numRows; i++)
	{
		map[i] = new Tile[numColumns];
		canopy[i] = new Tile[numColumns];
		ground[i] = new Tile[numColumns];
	}

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
	int x = 0;

	//While the end of file marker has not been reached, continue to read the file
	for (int j = 0; j < numRows; j++)
	{
		std::getline(mapFile, input, '_');

		//This loop instantiates all of the Tiles in the tile 2d array
		for (int i = 0; i < numColumns; i++)
		{
			map[j][i].row = (unsigned int)input[0] - '0';
			map[j][i].column = (unsigned int)input[1] - '0';
			map[j][i].transformation = (unsigned int)input[2] - '0';
			map[j][i].collidable = (unsigned int)input[3] - '0'; //0 = false, 1 = true
			map[j][i].transitionTile = (unsigned int)input[4] - '0'; //0 = false, 1 = true

			//Check to see if a canopy tile exists
			if (input.size() > 5)
			{
				if (input[x + 9] - '0' == 1)
				{
					ground[j][i].row = input[6] - '0';
					ground[j][i].column = input[7] - '0';
					ground[j][i].transformation = input[8] - '0';
					ground[j][i].collidable = input[9] - '0'; //0 = false, 1 = true				
					ground[j][i].transitionTile = input[10] - '0'; //0 = false, 1 = true
					ground[j][i].hasTile = true; //Set this to true if a tile exists in this space
					
					std::string s = input.substr(12, 13), ss = input.substr(15, 16);
					ground[j][i].width = atoi(s.c_str());
					ground[j][i].height = atoi(ss.c_str());
				}
				else
				{
					canopy[j][i].row = input[6] - '0';
					canopy[j][i].column = input[7] - '0';
					canopy[j][i].transformation = input[8] - '0';
					canopy[j][i].collidable = input[9] - '0'; //0 = false, 1 = true				
					canopy[j][i].transitionTile = input[10] - '0'; //0 = false, 1 = true
					canopy[j][i].hasTile = true; //Set this to true if a tile exists in this space
				}	
			}

			if (i < (numColumns - 1))
				std::getline(mapFile, input, '_');
		}

		std::getline(mapFile, input);
	}
}

/*
emptyMap

This method simply releases the memory used to store the previous map.
*/
void Map::emptyMap()
{
	//Free memory that was allocated for the map arrays
	for (int i = 0; i < numRows; i++)
	{
		delete[] map[i];
		delete[] canopy[i];
		delete[] ground[i];
	}
	delete[] map;
	delete[] canopy;
	delete[] ground;
}

/*
drawMap

This method draws the map statically to a texture.
*/
void Map::drawMap()
{
	mapTexture.clear();
	canopyTexture.clear(sf::Color(0,0,0,0)); //Give the canopy a transparent background
	groundTexture.clear(sf::Color(0, 0, 0, 0));

	//Step through each row in the maps array.
	for (int i = 0; i <= numRows - 1; i++)
	{
		//Step through each columns in the maps row and add a tile where needed
		for (int j = 0; j <= numColumns - 1; j++)
		{
			drawToTexture(mapTexture, map, i, j);

			//Do not add tiles to places where there are none.
			if (canopy[i][j].hasTile)
				drawToTexture(canopyTexture, canopy, i, j);
			else if (ground[i][j].hasTile)
				drawToTexture(groundTexture, ground, i, j);
		}
	}

	//Let the textures know that they are done being drawn to
	mapTexture.display(); 
	canopyTexture.display(); 
	groundTexture.display();

	//Set the render textures to sprites
	mapSprite.setTexture(mapTexture.getTexture());
	canopySprite.setTexture(canopyTexture.getTexture());
	groundSprite.setTexture(groundTexture.getTexture());
}

/*
drawToTexture
Parameters:
	texture: This is the rendertexture that will be drawn to
	layer: This is the layer we want to check for a tile
	row: This is the row on the map we want to draw to
	column: This is the column we want to draw to

This method allows for a layer to have the proper tiles drawn to it. It checks for a tile,
then applies the correct transformations to the tile, and then draws the tile.
*/
void Map::drawToTexture(sf::RenderTexture& texture, Tile**& layer, int row, int column)
{
	//Set the part of the tile map to draw to the window
	tiles.setTextureRect(sf::IntRect(layer[row][column].column * TILE_SIZE,
		layer[row][column].row * TILE_SIZE,
		TILE_SIZE,
		TILE_SIZE));

	if (layer[row][column].transformation == 0)
	{
		tiles.setPosition(column * TILE_SIZE, row * TILE_SIZE); //Set the position of the tile to be drawn 

		texture.draw(tiles); //Draw the tile
	}
	else if (layer[row][column].transformation >= 1 && layer[row][column].transformation <= 3) //Rotate the tile if the transformation value is between 1 and 3, inclusive
	{
		//Create a temporary texture to hold the tile
		sf::Texture temp;
		temp.loadFromImage(tiles.getTexture()->copyToImage(), tiles.getTextureRect());

		//Create a temporary sprite to display the rotation of sprites
		sf::Sprite tmp(temp);

		//The tiles are 32x32, so the origin is 16, 16
		tmp.setOrigin(16, 16);
		tmp.rotate(layer[row][column].transformation * 90);

		tmp.setPosition(column * TILE_SIZE + 16, row * TILE_SIZE + 16); //Set the position of the tile to be drawn (The tile will be offset by 16 when rotated, so move it over and down by 16)

		texture.draw(tmp); //Draw the tile
	}
	else if (layer[row][column].transformation == 4) //Mirro tile
	{
		//Create a temporary texture to hold the tile
		sf::Texture temp;
		temp.loadFromImage(tiles.getTexture()->copyToImage(), tiles.getTextureRect());

		//Create a temporary sprite to display the rotation of sprites
		sf::Sprite tmp(temp);

		//The tiles are 32x32, so the origin is 16, 16
		tmp.setOrigin(16, 16);
		tmp.scale(-1.f, 1.f); //Mirror the tile

		tmp.setPosition(column * TILE_SIZE + 16, row * TILE_SIZE + 16); //Set the position of the tile to be drawn 

		texture.draw(tmp); //Draw the tile
	}
}

/*
draw
Parameters:
	window: This is the window that the map will be drawn on
	player: This is used to get the players position in the game to determine the order of
		    the drawing calls.

Draws the map to the game window
*/
void Map::draw(sf::RenderWindow* window, Player* player)
{
	//LOCAL VARIABLES
	std::vector<unsigned short> background;
	std::vector<unsigned short> foreground;

	window->draw(mapSprite);
	
	
	//Check each row for collision with the player or with npc's
	for (int i = 0; i < numRows; i++)
	{
		//If the current row of tiles y position is greater than the players bottom y position, render the tiles in the foreground.
		if (ground[i][player->getPlayerCoordinates().x / TILE_SIZE].hasTile &&
			ground[i][player->getPlayerCoordinates().x / TILE_SIZE].height + (i * TILE_SIZE) > player->getPlayerCoordinates().y + 16)
			foreground.push_back(i * TILE_SIZE);
		else //Otherwise, place the tiles in the background
			background.push_back(i * TILE_SIZE);
	}

	//Draw the background
	for (int i = 0; i < background.size(); i++)
	{
		groundSprite.setTextureRect(sf::IntRect(0,
			background[i],
			numColumns * TILE_SIZE,
			TILE_SIZE));
		groundSprite.setPosition(0, background[i]);

		window->draw(groundSprite);
	}

	player->draw(window); //Draw the player in between the background and foreground
	
	
	//Draw the foreground
	for (int i = 0; i < foreground.size(); i++)
	{
		groundSprite.setTextureRect(sf::IntRect(0,
			foreground[i],
			numColumns * TILE_SIZE,
			TILE_SIZE));
		groundSprite.setPosition(0, foreground[i]);

		window->draw(groundSprite);
	}

	window->draw(canopySprite); //Draw the canopy
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
	groundSprite.setColor(sf::Color(r, g, b, a));
	canopySprite.setColor(sf::Color(r, g, b, a));
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
bool Map::collisionDetected(sf::IntRect* rect)
{
	//If the entity is at the maps edge along the x-axis, return true
	if ((rect->left - rect->width) < 0 || rect->left + rect->width > numColumns * TILE_SIZE)
		return true;

	//If the entity is at the maps edge along the y-axis, return true
	if (rect->top < 0 || rect->top + rect->height > numRows * TILE_SIZE)
		return true;

	if (map[rect->top / TILE_SIZE][rect->left / TILE_SIZE].collidable)
		return checkCollisionOnLayer(rect, map);
	else if (ground[rect->top / TILE_SIZE][rect->left / TILE_SIZE].collidable)
		return checkCollisionOnLayer(rect, ground);
	else
		return false;
}

/*
checkCollisionOnLayer
Parameters:
	rect: This is an entity's bounding box
	layer: This is the layer we are checking collision against

This method is designed to provide more precise collision.
*/
bool Map::checkCollisionOnLayer(sf::IntRect* rect, Tile**& layer)
{
	//Create a bounding box for the current tile.
	sf::IntRect boundingBox = sf::IntRect((rect->left / TILE_SIZE) * TILE_SIZE + ((TILE_SIZE - layer[rect->top / TILE_SIZE][rect->left / TILE_SIZE].width) / 2),
		(rect->top / TILE_SIZE) * TILE_SIZE + ((TILE_SIZE - layer[rect->top / TILE_SIZE][rect->left / TILE_SIZE].height) / 2),
		layer[rect->top / TILE_SIZE][rect->left / TILE_SIZE].width, 
		layer[rect->top / TILE_SIZE][rect->left / TILE_SIZE].height);

	//Check to see if the entity is inside of a objects colliding point
	if (boundingBox.contains(rect->left, rect->top + 8))
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
	//Free memory that was allocated for the map arrays
	for (int i = 0; i < numRows; i++)
	{
		delete[] map[i];
		delete[] canopy[i];
		delete[] ground[i];
	}
	delete[] map;
	delete[] canopy;
	delete[] ground;
}