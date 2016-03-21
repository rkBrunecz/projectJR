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
	mousePos.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	mousePos.setFillColor(sf::Color(0, 0, 200, 125));

	selectedTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	selectedTile.setFillColor(sf::Color(200, 200, 0, 125));

	if(!deleteTex.loadFromFile("bin/Graphics/Delete.png")  || !transitionTex.loadFromFile("bin/Graphics/Transition.png"))
		exit(EXIT_FAILURE);
	
	deleteTile.setSize(sf::Vector2f(32, 32));
	deleteTile.setTexture(&deleteTex);

	transitionTile.setSize(sf::Vector2f(32, 32));
	transitionTile.setTexture(&transitionTex);
}

void Map::initializeTileSheetCoords(sf::RenderWindow* window)
{
	tileSheetCoords.x = window->getSize().x - 352;
	tileSheetCoords.y = 64;

	deleteTile.setPosition(window->getSize().x - (TILE_SIZE * 3) - (TILE_SIZE / 2), TILE_SIZE);
	transitionTile.setPosition(window->getSize().x - (TILE_SIZE * 2), TILE_SIZE);
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

	//If the rows and columns of the map have been set, it is safe to clear out the map file.
	if (numRows != 0 && numColumns != 0)
		emptyMap(); //Empty out the map file to make room for a new map.

	//Open the mapFile file
	mapFile.open(mapName);
	nameOfFile = mapName;

	if (mapFile.is_open())
		initialize(mapFile, camera); //Dynamically create an array to hold the map	
	else
		exit(EXIT_FAILURE); //Close the application with a failure code if the file does not open
	
	mapFile.close(); //Close the file

	mapLoaded = true;
}

void Map::createMap(unsigned int rows, unsigned int columns, Camera* camera, std::string mapName)
{
	//If the rows and columns of the map have been set, it is safe to clear out the map file.
	if (numRows != 0 && numColumns != 0)
		emptyMap(); //Empty out the map file to make room for a new map.

	//LOCAL VARIABLES
	numRows = rows; 
	numColumns = columns;

	nameOfFile = mapName;

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

	camera->setBounds((numColumns + 4) * TILE_SIZE, numRows * TILE_SIZE);

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

	mapTexture.clear(sf::Color(0, 0, 0, 0));
	canopyTexture.clear(sf::Color(0, 0, 0, 0));
	groundTexture.clear(sf::Color(0, 0, 0, 0));
	maskTexture.clear(sf::Color(0, 0, 0, 0));
	for (int i = 0; i < NUM_WATER_FRAMES; i++)
		waterFrames[i].clear(sf::Color(0, 0, 0, 0));
	
	collisionTexture.clear(sf::Color(0, 0, 0, 0));

	createGrid();

	//Let the textures know that they are done being drawn to
	mapTexture.display();
	canopyTexture.display();
	groundTexture.display();
	maskTexture.display();
	for (int i = 0; i < NUM_WATER_FRAMES; i++)
		waterFrames[i].display();

	//TOOLS
	collisionTexture.display();

	//Set the render textures to sprites
	mapSprite.setTexture(mapTexture.getTexture());
	canopySprite.setTexture(canopyTexture.getTexture());
	groundSprite.setTexture(groundTexture.getTexture());
	maskSprite.setTexture(maskTexture.getTexture());
	waterSprite.setTexture(waterFrames[0].getTexture());

	//TOOLS
	collisionSprite.setTexture(collisionTexture.getTexture());

	mapLoaded = true;
}

void Map::createGrid()
{
	gridTexture.clear(sf::Color(0, 0, 0, 0));

	//Used to draw grids lines 
	sf::RectangleShape r;
	r.setFillColor(sf::Color(0, 0, 0, 255));

	//Step through each row and draw grid lines
	for (int i = 0; i <= numRows - 1; i++)
	{
		//Step through each column and draw grid lines
		for (int j = 0; j <= numColumns - 1; j++)
		{
			if (i == numRows - 1)
			{
				//TOOLS
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

	r.setPosition(numRows * TILE_SIZE, 0);
	r.setSize(sf::Vector2f(1, TILE_SIZE * numRows));
	gridTexture.draw(r);

	r.setPosition(0, numColumns * TILE_SIZE);
	r.setSize(sf::Vector2f(TILE_SIZE * numColumns, 1));
	gridTexture.draw(r);

	gridTexture.display();
	gridSprite.setTexture(gridTexture.getTexture());
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
	std::ifstream tileFile;

	//Get the tile data so that tiles can be added to the map
	std::getline(mapFile, nameOfSheetFile);
	tileFile.open(nameOfSheetFile);

	if (!tileFile.is_open())
		exit(EXIT_FAILURE);

	for (int i = 0; i < NUM_TILES_IN_SHEET; i++)
	{
		std::getline(tileFile, input, '_');
		tileData[i] = input;

		//Get the end of line delimiter
		if ((i + 1) % 10 == 0)
			std::getline(tileFile, input);
	}

	tileFile.close(); //Close the tile file

	//Get the path to the tile map and then open it.
	std::getline(mapFile, nameOfTileSheet);
	if (!tileSheet.loadFromFile(nameOfTileSheet))
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
	
	createGrid();

	camera->setBounds((numColumns + 4) * TILE_SIZE, numRows * TILE_SIZE);

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
	std::string input;
	int numCoords;
	sf::Vector2i coords;

	//Get the number connected maps
	std::getline(mapFile, input);
	numTransitionPoints = atoi(input.c_str());

	for (int i = 0; i < numTransitionPoints; i++)
	{
		TransitionPoint p;

		//Get the file name for a connected map
		std::getline(mapFile, p.transitionMapName, '-');

		//Get the coordinates that the player character will start on in the next map
		std::getline(mapFile, input, 'x');
		coords.y = atoi(input.c_str()) * TILE_SIZE;
		std::getline(mapFile, input, '-');
		coords.x = atoi(input.c_str()) * TILE_SIZE;

		p.startingCoords.x = coords.x;
		p.startingCoords.y = coords.y;

		//Get the total number of map tiles that move the same map. Allows for an area to be defined to allow for more map transition flexibility
		std::getline(mapFile, input, '-');
		numCoords = atoi(input.c_str());

		p.numCoords = numCoords;

		//Loop to find all possible map transition points and store the coordinates for the next map, and the map name to the file.
		for (int i = 0; i < numCoords; i++)
		{
			sf::Vector2i tp;

			//Get the coordinates of a tile that is a transition point
			std::getline(mapFile, input, 'x');
			tp.y = atoi(input.c_str()) - 1;
			std::getline(mapFile, input, ',');
			tp.x = atoi(input.c_str()) - 1;

			//Store the file name and coordinates for the spawning position in the next map
			map[tp.y][tp.x].mapName = p.transitionMapName;
			map[tp.y][tp.x].transitionCoords = coords;

			p.transitionPoints.push_back(tp);
		}

		transitions.push_back(p);
	}

	//Clear out the line to get ready to read the map.
	std::getline(mapFile, input);
}

/*
populateMap
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
		layer[row][column].boundingBox = width + "x" + height + "x" + sBBX + "x" + sBBY;

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
		}
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

	//Set the render textures to sprites
	mapSprite.setTexture(mapTexture.getTexture());
	canopySprite.setTexture(canopyTexture.getTexture());
	groundSprite.setTexture(groundTexture.getTexture());
	maskSprite.setTexture(maskTexture.getTexture());
	waterSprite.setTexture(waterFrames[0].getTexture());
	
	//TOOLS
	collisionSprite.setTexture(collisionTexture.getTexture());
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
	//Set the part of the tile map to draw to the texture
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
	else if (layer[row][column].transformation == 4) //Mirror tile
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
void Map::draw(sf::RenderWindow* window, sf::Vector2f mouseCoords)
{
	//LOCAL VARIABLES
	std::vector<unsigned short> background;
	std::vector<unsigned short> foreground;

	Animation::updateWaterAnimation(&waterShift, &waterAnimation, &waterSprite, waterFrames, &currentWaterFrame, NUM_WATER_FRAMES);

	window->draw(waterSprite);
	window->draw(mapSprite);
	window->draw(maskSprite);
	window->draw(groundSprite);
	window->draw(canopySprite); //Draw the canopy
	
	//TOOLS
	if (renderCollisionLayer)
		window->draw(collisionSprite);
	if (renderGridLayer)
		window->draw(gridSprite);

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		mousePos.setPosition(sf::Vector2f(floor(mouseCoords.x / TILE_SIZE) * TILE_SIZE,
			floor(mouseCoords.y / TILE_SIZE) * TILE_SIZE));
	
		if (mousePos.getPosition().x <= TILE_SIZE * (numColumns - 1) && mousePos.getPosition().y <= TILE_SIZE * (numRows - 1))
			window->draw(mousePos);
	}
}

void Map::drawTileSheet(sf::RenderWindow* window)
{
	sf::Sprite tmp;
	tmp.setTexture(tileSheet);
	tmp.setPosition(tileSheetCoords.x, tileSheetCoords.y);
	window->draw(tmp);
	window->draw(deleteTile);
	window->draw(transitionTile);

	if (currentTile.compare("No Tile") != 0)
		window->draw(selectedTile);
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
	if (rect->top - rect->height < 0 || rect->top + rect->height > numRows * TILE_SIZE)
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

	//Check to see if the entity is inside of a objects colliding point
	return rect->intersects(boundingBox);
}

bool Map::isMapLoaded()
{
	return mapLoaded;
}

sf::Vector2f Map::mapSize()
{
	return sf::Vector2f(numColumns * TILE_SIZE, numRows * TILE_SIZE);
}

void Map::setTile(sf::Vector2i mouseCoords)
{
	if (mouseCoords.x < tileSheetCoords.x || mouseCoords.y < tileSheetCoords.y)
		return;

	//LOCAL VARIABLES
	int column = (mouseCoords.x - tileSheetCoords.x) / TILE_SIZE;
	int row = (mouseCoords.y - tileSheetCoords.y) / TILE_SIZE;
	int pos = row * 10 + column; // Multiply by 10 since there are 10 columns per row

	if (column > 9 || row > 19)
		return;

	currentTile = tileData[pos];
	
	if (currentTile.compare("&") != 0)
		selectedTile.setPosition(sf::Vector2f((mouseCoords.x / TILE_SIZE) * TILE_SIZE, (mouseCoords.y / TILE_SIZE) * TILE_SIZE));
	else
		currentTile = "No Tile";
}

void Map::addTileToPos()
{
	if (currentTile.compare("No Tile") == 0)
		return;

	//LOCAL VARIABLES
	int column = mousePos.getPosition().x / TILE_SIZE;
	int row = mousePos.getPosition().y / TILE_SIZE;

	if (column > numColumns - 1 || row > numRows - 1 || column < 0 || row < 0)
		return;

	if (currentTile[0] == '0' && !isSameTile(map, row, column))
	{
		addTileToMap(map, currentTile, 2, row, column);
		drawToTexture(mapTexture, map, row, column);
		mapTexture.display();
	}
	else if (currentTile[0] == '1' /*&& !isSameTile(map, row, column)*/)
	{
		addTileToMap(mask, currentTile, 2, row, column);
		
		if (currentTile[5] == '1' && map[row][column].collidable)
			map[row][column].collidable = false;

		updateMap(maskTexture, mask);
	}
	else if (currentTile[0] == '2' && !isSameTile(map, row, column))
	{
		addTileToMap(ground, currentTile, 2, row, column);
		updateMap(groundTexture, ground);
	}
	else if (currentTile[0] == '3' && !isSameTile(map, row, column))
	{
		addTileToMap(canopy, currentTile, 2, row, column);
		updateMap(canopyTexture, canopy);
	}
}

bool Map::isSameTile(Tile**& layer, int row, int column)
{
	Tile t;
	t.row = currentTile[2] - '0';
	t.column = currentTile[3] - '0';
	t.transformation = currentTile[4] - '0';
	t.collidable = currentTile[5] - '0'; //0 = false, 1 = true
	t.tileType = currentTile[6];

	if (layer[row][column].row == t.row &&
		layer[row][column].column == t.column &&
		layer[row][column].transformation == t.transformation &&
		layer[row][column].collidable == t.collidable &&
		layer[row][column].tileType == t.tileType)
		return true;
	
	return false;
}

void Map::updateMap(sf::RenderTexture& texture, Tile**& layer)
{
	texture.clear(sf::Color(0, 0, 0, 0));

	//Step through each row in the maps array.
	for (int i = 0; i <= numRows - 1; i++)
	{
		//Step through each columns in the maps row and add a tile where needed
		for (int j = 0; j <= numColumns - 1; j++)
			drawToTexture(texture, layer, i, j);
	}

	texture.display();
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

unsigned short Map::getTileSize()
{
	return TILE_SIZE;
}

void Map::saveMap()
{
	//LOCAL VARIABLES
	std::ofstream mapFile;
	bool hasTile = false;

	mapFile.open(nameOfFile);

	mapFile << nameOfSheetFile << "\n";
	mapFile << nameOfTileSheet << "\n";
	mapFile << numRows << "x" << numColumns << "\n";
	mapFile << numTransitionPoints << "\n";

	for (int i = 0; i < numTransitionPoints; i++)
	{
		mapFile << transitions[i].transitionMapName << "-" << transitions[i].startingCoords.y << "x" << transitions[i].startingCoords.x << "-" << transitions[i].numCoords << "-";

		for (int j = 0; j < transitions[i].numCoords; j++)
			mapFile << transitions[i].transitionPoints[j].y << "x" << transitions[i].transitionPoints[j].x << ",";

		mapFile << "\n";
	}

	for (int row = 0; row < numRows; row++)
	{
		for (int column = 0; column < numColumns; column++)
		{
			if (map[row][column].hasTile)
			{
				mapFile << "0-" << tileToString(map, row, column);
				hasTile = true;
			}
			if (mask[row][column].hasTile)
			{
				if (hasTile)
				{
					mapFile << ",";
					hasTile = false;
				}
				mapFile << "1-" << tileToString(mask, row, column);
				hasTile = true;
			}
			if (ground[row][column].hasTile)
			{
				if (hasTile)
				{
					mapFile << ",";
					hasTile = false;
				}
				mapFile << "2-" << tileToString(ground, row, column);
				hasTile = true;
			}
			if (canopy[row][column].hasTile)
			{
				if (hasTile)
				{
					mapFile << ",";
					hasTile = false;
				}
				mapFile << "3-" << tileToString(canopy, row, column);
				hasTile = true;
			}

			mapFile << "_";
		}

		mapFile << "\n";
	}

	mapFile.close();
}

std::string Map::tileToString(Tile**& layer, int row, int column)
{
	std::string s = std::to_string(layer[row][column].row) + std::to_string(layer[row][column].column) + std::to_string(layer[row][column].transformation);
	
	if (layer[row][column].collidable)
	{
		s = s + "1" + layer[row][column].tileType;

		if (layer[row][column].tileType != 'W')
			s = s + ":" + layer[row][column].boundingBox;
	}
	else
		s = s + "0" + layer[row][column].tileType;

	return s;
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