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
	std::getline(mapFile, input); //Read the first line out (intended for the map editor)
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
		maskTexture.create(numColumns * TILE_SIZE, numRows * TILE_SIZE);		
		for (int i = 0; i < NUM_WATER_FRAMES; i++)
			waterFrames[i].create(numColumns * TILE_SIZE, numRows * TILE_SIZE);

		//TOOLS
		collisionTexture.create(numColumns * TILE_SIZE, numRows * TILE_SIZE);
		gridTexture.create(numColumns * TILE_SIZE, numRows * TILE_SIZE);
	}

	camera->setBounds(numColumns * TILE_SIZE, numRows * TILE_SIZE);

	//Dynamically Create the arrays to hold the map
	map = new Tile*[numRows];
	canopy = new Tile*[numRows];
	ground = new Tile*[numRows];
	mask = new Tile*[numRows];
	for (int i = 0; i < numRows; i++)
	{
		map[i] = new Tile[numColumns];
		canopy[i] = new Tile[numColumns];
		ground[i] = new Tile[numColumns];
		mask[i] = new Tile[numColumns];
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
	unsigned short pos = 0;

	//While the end of file marker has not been reached, continue to read the file
	for (int j = 0; j < numRows; j++)
	{
		std::getline(mapFile, input, '_');

		//This loop instantiates all of the Tiles in the tile 2d array
		for (int i = 0; i < numColumns; i++)
		{
			pos = addTileToMap(map, input, 2, j, i);

			//Check to see if a canopy tile exists
			while (pos < input.size())
			{				
				if (input[pos] == '1')
					pos = addTileToMap(mask, input, pos + 2, j, i);
				else if (input[pos] == '2')
					pos = addTileToMap(ground, input, pos + 2, j, i);
				else if (input[pos] == '3')
					pos = addTileToMap(canopy, input, pos + 2, j, i);
			}

			if (i < (numColumns - 1))
				std::getline(mapFile, input, '_');
		}

		std::getline(mapFile, input);
	}
}

/*
addTileToMap
Parameters:
	layer: This is the layer that a tile will be added to
	input: This is the input string that contains all the info about a tile
	pos: This is the current position in the string
	row: This is the current row in the layer to add the tile
	column: This is the current column in the layer to add a tile to

This method adds tiles to a specific layer in the map.
*/
unsigned short Map::addTileToMap(Tile** layer, std::string input, unsigned int pos, unsigned short row, unsigned short column)
{
	layer[row][column].row = input[pos] - '0';
	layer[row][column].column = input[pos + 1] - '0';
	layer[row][column].transformation = input[pos + 2] - '0';
	layer[row][column].collidable = input[pos + 3] - '0'; //0 = false, 1 = true
	layer[row][column].tileType = input[pos + 4];
	layer[row][column].hasTile = true;

	//If collidable, add a bounding box
	if (layer[row][column].collidable && input[pos + 5] == ':')
	{
		short t = layer[row][column].transformation;
		std::string width = input.substr(pos + 6, 2), height = input.substr(pos + 9, 2), sBBX = input.substr(pos + 12, 2), sBBY = input.substr(pos + 15, 2);

		//This reverses the tiles height and width if the tile is flipped 90 degrees (1) or 270 degrees (3)
		if (t == 0 || t == 2)
		{
			layer[row][column].width = atoi(width.c_str());
			layer[row][column].height = atoi(height.c_str());
		}
		else
		{
			layer[row][column].width = atoi(width.c_str());
			layer[row][column].height = atoi(height.c_str());
		}

		//Sets the bounding box correctly if the tile is rotated
		if (t == 0) //No transformation applied
		{
			layer[row][column].bBX = atoi(sBBX.c_str());
			layer[row][column].bBY = atoi(sBBY.c_str());
		}
		else if (t == 2) //Tile rotated 180 degrees
		{
			layer[row][column].bBX = atoi(sBBX.c_str());
			layer[row][column].bBY = layer[row][column].height - atoi(sBBY.c_str());

			if (layer[row][column].height > atoi(sBBY.c_str()))
				layer[row][column].bBY = 0;
		}
		else if (t == 1) //Tile rotated 90 degrees
		{
			layer[row][column].bBX = atoi(sBBY.c_str());
			layer[row][column].bBY = atoi(sBBX.c_str());
		}
		else if (t == 3) //Tile rotate 270 degrees
		{
			layer[row][column].bBX = TILE_SIZE - atoi(sBBY.c_str());
			layer[row][column].bBY = atoi(sBBX.c_str());
		}

		return pos + 18;
	}
	else
		return pos + 6;
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
		delete[] mask[i];
	}
	delete[] map;
	delete[] canopy;
	delete[] ground;
	delete[] mask;
}

/*
drawMap

This method draws the map statically to a texture.
*/
void Map::drawMap()
{
	//Give the textures transparent backgrounds
	mapTexture.clear(sf::Color(0,0,0,0));
	canopyTexture.clear(sf::Color(0,0,0,0)); 
	groundTexture.clear(sf::Color(0,0,0,0));
	maskTexture.clear(sf::Color(0,0,0,0));
	for (int i = 0; i < NUM_WATER_FRAMES; i++)
		waterFrames[i].clear(sf::Color(0,0,0,0));

	//TOOLS
	collisionTexture.clear(sf::Color(0, 0, 0, 0));
	gridTexture.clear(sf::Color(0, 0, 0, 0));

	//Used to draw grids lines 
	sf::RectangleShape r;
	r.setFillColor(sf::Color(0, 0, 0, 255));

	//Step through each row in the maps array.
	for (int i = 0; i <= numRows - 1; i++)
	{
		//Step through each columns in the maps row and add a tile where needed
		for (int j = 0; j <= numColumns - 1; j++)
		{
			//If the current tile is not a water tile, add it to the base map layer
			if (map[i][j].tileType != 'W')
				drawToTexture(mapTexture, map, i, j);
			else //Draw it to the water layer
			{
				drawToTexture(waterFrames[0], map, i, j);

				for (int y = 1; y < NUM_WATER_FRAMES; y++)
				{
					//Set the part of the tile map to draw to the window
					tiles.setTextureRect(sf::IntRect((map[i][j].column + y) * TILE_SIZE,
						map[i][j].row * TILE_SIZE,
						TILE_SIZE,
						TILE_SIZE));

					tiles.setPosition(j * TILE_SIZE, i * TILE_SIZE);
					waterFrames[y].draw(tiles);
				}
			}

			//Do not add tiles to places where there are none.
			if (canopy[i][j].hasTile)
				drawToTexture(canopyTexture, canopy, i, j);
			if (ground[i][j].hasTile)
				drawToTexture(groundTexture, ground, i, j);
			if (mask[i][j].hasTile)
				drawToTexture(maskTexture, mask, i, j);

			//TOOLS
			if (i == numRows - 1)
			{
				r.setPosition(j * TILE_SIZE, 0);
				r.setSize(sf::Vector2f(1, TILE_SIZE * numRows));
				gridTexture.draw(r);
			}
		}

		//TOOLS
		r.setPosition(0, i * TILE_SIZE);
		r.setSize(sf::Vector2f(TILE_SIZE * numColumns, 1));
		gridTexture.draw(r);
	}

	//Let the textures know that they are done being drawn to
	mapTexture.display(); 
	canopyTexture.display(); 
	groundTexture.display();
	maskTexture.display();
	for (int i = 0; i < NUM_WATER_FRAMES; i++)
		waterFrames[i].display();

	//TOOLS
	collisionTexture.display();
	gridTexture.display();

	//Set the render textures to sprites
	mapSprite.setTexture(mapTexture.getTexture());
	canopySprite.setTexture(canopyTexture.getTexture());
	groundSprite.setTexture(groundTexture.getTexture());
	maskSprite.setTexture(maskTexture.getTexture());
	waterSprite.setTexture(waterFrames[0].getTexture());
	
	//TOOLS
	collisionSprite.setTexture(collisionTexture.getTexture());
	gridSprite.setTexture(gridTexture.getTexture());
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

	//TOOLS
	if (layer[row][column].collidable)
	{
		sf::RectangleShape r;
		r.setPosition(column * TILE_SIZE + layer[row][column].bBX, row * TILE_SIZE + layer[row][column].bBY);
		r.setSize(sf::Vector2f(layer[row][column].width, layer[row][column].height));
		r.setFillColor(sf::Color(255, 0, 0, 150));

		collisionTexture.draw(r);
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

	Animation::updateWaterAnimation(&waterShift, &waterAnimation, &waterSprite, waterFrames, &currentWaterFrame, NUM_WATER_FRAMES);

	window->draw(waterSprite);
	window->draw(mapSprite);
	window->draw(maskSprite);
	
	int column = (player->getPlayerCoordinates().left + (player->getPlayerCoordinates().width * 0.5)) / TILE_SIZE;
	
	//Check each row for collision with the player or with npc's
	for (int i = 0; i < numRows; i++)
	{
		//If the current row of tiles y position is greater than the players bottom y position, render the tiles in the foreground.
		if (ground[i][column].hasTile &&
			(i * TILE_SIZE) + ground[i][column].bBY > player->getPlayerCoordinates().top)
			foreground.push_back(i * TILE_SIZE);
		else if(column - 1 >= 0 && ground[i][column - 1].hasTile &&
			(i * TILE_SIZE) + ground[i][column - 1].bBY > player->getPlayerCoordinates().top)
			foreground.push_back(i * TILE_SIZE);
		else if (column + 1 < numRows && ground[i][column + 1].hasTile &&
			(i * TILE_SIZE) + ground[i][column + 1].bBY > player->getPlayerCoordinates().top)
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
	
	//TOOLS
	if (renderCollisionLayer)
		window->draw(collisionSprite);
	if (renderGridLayer)
		window->draw(gridSprite);
}


/*
drawNoAni
Parameters:
window: This is the window that the map will be drawn on
player: This is used to get the players position in the game to determine the order of
the drawing calls.

Draws the map to the game window with no animations
*/
void Map::drawNoAni(sf::RenderWindow* window, Player* player)
{
	//LOCAL VARIABLES
	std::vector<unsigned short> background;
	std::vector<unsigned short> foreground;

	window->draw(waterSprite);
	window->draw(mapSprite);
	window->draw(maskSprite);

	unsigned int column = player->getPlayerCoordinates().left / TILE_SIZE;

	//Check each row for collision with the player or with npc's
	for (int i = 0; i < numRows; i++)
	{
		//If the current row of tiles y position is greater than the players bottom y position, render the tiles in the foreground.
		if (ground[i][column].hasTile &&
			(i * TILE_SIZE) + ground[i][column].bBY > player->getPlayerCoordinates().top + player->getPlayerCoordinates().height)
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
	maskSprite.setColor(sf::Color(r, g, b, a));
	waterSprite.setColor(sf::Color(r, g, b, a));
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
	int row = player->getPlayerCoordinates().top / TILE_SIZE;
	int column = player->getPlayerCoordinates().left / TILE_SIZE;
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
	if (rect->left < 0 || rect->left + rect->width > numColumns * TILE_SIZE)
		return true;

	//If the entity is at the maps edge along the y-axis, return true
	if ((rect->top + rect->height + 2) - TILE_SIZE < 0 || rect->top + rect->height > numRows * TILE_SIZE)
		return true;
	
	unsigned int row = (rect->top + rect->height) / TILE_SIZE, column = rect->left / TILE_SIZE;
	bool collision = false;

	//Check collision of the player on the left side of player
	if (map[row][column].collidable)
		collision = checkCollisionOnLayer(rect, map, row, column);
	else if (ground[row][column].collidable)
		collision = checkCollisionOnLayer(rect, ground, row, column);
	else if (mask[row][column].collidable)
		collision = checkCollisionOnLayer(rect, mask, row, column);
	
	//Return true if collision has been detected
	if (collision)
		return collision;

	//Check collision of the player on the right side of the player
	column = (rect->left + rect->width) / TILE_SIZE;
	if (map[row][column].collidable)
		collision = checkCollisionOnLayer(rect, map, row, column);
	else if (ground[row][column].collidable)
		collision = checkCollisionOnLayer(rect, ground, row, column);
	else if (mask[row][column].collidable)
		collision = checkCollisionOnLayer(rect, mask, row, column);

	return collision; //Return collision
}

/*
checkCollisionOnLayer
Parameters:
	rect: This is an entity's bounding box
	layer: This is the layer we are checking collision against

This method is designed to provide more precise collision.
*/
bool Map::checkCollisionOnLayer(sf::IntRect* rect, Tile**& layer, int row, int column)
{
	//Create a bounding box for the current tile.
	sf::IntRect boundingBox = sf::IntRect(column * TILE_SIZE + layer[row][column].bBX,
		row * TILE_SIZE + layer[row][column].bBY,
		layer[row][column].width,
		layer[row][column].height);

	if (boundingBox.top == rect->height + rect->top)
		boundingBox.top -= 1;

	//Check to see if the entity is inside of a objects colliding point
	return rect->intersects(boundingBox);
}

/*
transitioning

This method is used to determine if the game state should switch to the transitioning state or not.
*/
bool Map::transitioning(Player* player)
{
	//This checks to see if the tile being moved to is a transition tile
	if (map[(int)player->getPlayerCoordinates().top / TILE_SIZE][(int)player->getPlayerCoordinates().left / TILE_SIZE].tileType == 'E')
		return true; //No collision has been detected

	return false;
}

void Map::displayCollsionLayer()
{
	if (!renderCollisionLayer)
		renderCollisionLayer = true;
	else
		renderCollisionLayer = false;
}

void Map::displayGridLayer()
{
	if (!renderGridLayer)
		renderGridLayer = true;
	else
		renderGridLayer = false;
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
		delete[] mask[i];
	}
	delete[] map;
	delete[] canopy;
	delete[] ground;
	delete[] mask;
}