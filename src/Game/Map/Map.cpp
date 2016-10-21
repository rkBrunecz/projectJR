/*
Map.cpp
This class handles createing and drawing a map. It also performs other map related useful functions.

@author Randall Brunecz
@version 2.0 3/23/2016
-Overhauled how the map draws itself. Will allow for straight forward implementation of NPC's. The new version is also a large optimization over the previous implementation.

@version 1.0 9/5/2015
*/

#include "Game\Map\Map.h"
#include "Game\Game.h"

/*
Map 

The constructor of the map class creates a map that can be drawn.
*/
Map::Map(std::string mapName)
{
	pb::addToCollisionList(this);
	Tile::lightsOn = false;

	loadMap(mapName);
}

/*
loadMap
Parameters:
	mapName: This is the name of the map file that needs to be loaded in
	camera:  This is the game camera. This is modified to have new boundaries upon entering a new map.

Load map simply loads in all relavent map data to the map array to ready it to be displayer later.
*/
void Map::loadMap(std::string mapName)
{
	//LOCAL VARIABLES
	std::ifstream mapFile;

	//If the rows and columns of the map have been set, you it is safe to clear out the map file.
	if (numRows != 0 && numColumns != 0)
		emptyMap(); //Empty out the map file to make room for a new map.

	//Open the mapFile file
	mapFile.open(mapName);

	if (mapFile.is_open())
		initialize(mapFile); //Dynamically create an array to hold the map		
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

	// Get jrs and initialize tile data
	std::getline(mapFile, input);
	initializeTileData(input);

	//Get the path to the tile map and then open it.
	std::getline(mapFile, input);

	//Get the total number of rows and columns from the file and store them
	std::getline(mapFile, mapRows, 'x');
	std::getline(mapFile, mapColumns);

	//Convert the parsed strings to ints
	numRows = atoi(mapRows.c_str());
	numColumns = atoi(mapColumns.c_str());

	// Initialize layers
	mapLayer = new Layer(input, numRows, numColumns, Game::graphicManager);
	canopyLayer = new Layer(input, numRows, numColumns, Game::graphicManager);
	maskLayer = new Layer(input, numRows, numColumns, Game::graphicManager);

	groundLayers = new Layer*[numRows];
	for (unsigned int i = 0; i < numRows; i++)
		groundLayers[i] = new Layer(input, 1, numColumns, Game::graphicManager);

	// Tools
	collisionLayer = new Primitive_Layer(numRows, numColumns, sf::Quads, sf::Color(255, 0, 0, 150));
	transitionLayer = new Primitive_Layer(numRows, numColumns, sf::Quads, sf::Color(0, 255, 0, 255));
	gridLayer = new Primitive_Layer(numRows, numColumns, sf::LinesStrip, sf::Color::Black);

	Game::camera->setBounds((float)(numColumns * TILE_SIZE), (float)(numRows * TILE_SIZE));

	initializeTransitionPoints(mapFile); //Intialize all transition points in the map

	populateMap(mapFile); //Fill the array with the map data
}

void Map::initializeTileData(const std::string& jrsFile)
{
	// Local variables
	std::ifstream jrsFileStream;
	std::string input;
	unsigned int rows;

	// Open file
	jrsFileStream.open(jrsFile);
	if (!jrsFileStream.is_open())
		exit(EXIT_FAILURE);

	// Get rows and columns of the jrs sheet
	std::getline(jrsFileStream, input);
	rows = atoi(input.c_str());

	std::getline(jrsFileStream, input);
	tileDataColumns = atoi(input.c_str());

	// Resize the tileData vector
	tileData.resize(rows * tileDataColumns);

	for (unsigned int i = 0; i < rows; i++)
	{
		for (unsigned int j = 0; j < tileDataColumns; j++)
		{
			std::getline(jrsFileStream, input, '_');

			if (input[0] != '&')
			{
				// Position in vector
				int pos = i * tileDataColumns + j;
				int inputPos = 0;

				tileData[pos] = new Tile_Data();

				// Get layer
				tileData[pos]->layer = input[inputPos++] - '0';

				// Determine tile
				if (input[1] == '-')
				{
					tileData[pos]->t = new Tile();
					inputPos++;

					tileData[pos]->type = Tile_Data::Normal;

					initializeTile(input, inputPos, tileData[pos]->t);
				}
				else
				{
					tileData[pos]->at = new Animated_Tile();

					tileData[pos]->at->numAnimationFrames = atoi(input.substr(2, input.find('(')).c_str());
					tileData[pos]->at->updateInterval = 1 / (float)atoi(input.substr(input.find('(') + 1, input.find(')')).c_str());

					inputPos = input.find_last_of('*');
					tileData[pos]->at->numLoops = atoi(input.substr(input.find(')') + 1, inputPos).c_str());

					inputPos++;

					tileData[pos]->type = Tile_Data::Animated;

					initializeTile(input, inputPos, tileData[pos]->at);
				}
			}
		}

		// Consume line at the end of a row
		std::getline(jrsFileStream, input);
	}

	// Close file
	jrsFileStream.close();
}

void Map::initializeTile(const std::string& input, int inputPos, Tile *t)
{
	// Determine if tile produces light
	if (input[inputPos] == 'L')
	{
		// Obtain the distance the light travels out in all directions
		inputPos++;
		float steps = (float)atoi(input.substr(inputPos, input.find('(')).c_str());
		
		// Obtain the color of the light
		inputPos = input.find('(') + 1;
		int r = atoi(input.substr(inputPos, input.find(',')).c_str());
		inputPos = input.find(',', inputPos) + 1;
		int g = atoi(input.substr(inputPos, input.find(inputPos, ',')).c_str());
		inputPos = input.find(',', inputPos) + 1;
		int b = atoi(input.substr(inputPos, input.find(inputPos, ',')).c_str());
		inputPos = input.find(',', inputPos) + 1;
		int a = atoi(input.substr(inputPos, input.find(')')).c_str());

		// Obtain the intensity of the light
		inputPos = input.find(')') + 1;
		float intensity = (float)atof(input.substr(inputPos, input.find('L', inputPos)).c_str());
		inputPos = input.find('L', inputPos) + 1;

		t->light = new pb::Light(sf::Color(r, g, b, a), steps, intensity);
	}

	t->row = input[inputPos++] - '0';
	t->column = input[inputPos++] - '0';
	t->rotation = input[inputPos++] - '0';

	if (input[inputPos++] - '0' == 1)
		t->mirror = true;
	if (input[inputPos++] - '0' == 1)
		t->collidable = true;
	else
		t->collidable = false;

	t->tileType = input[inputPos++];
	t->hasTile = true;
	t->boundingBoxString = "none";

	//If collidable, add a bounding box
	if (t->collidable && input[inputPos++] == ':')
	{
		short transform = t->rotation;
		std::string width = input.substr(inputPos, 2), height = input.substr(inputPos + 3, 2), sBBX = input.substr(inputPos + 6, 2), sBBY = input.substr(inputPos + 9, 2);
		t->boundingBoxString = width + "x" + height + "x" + sBBX + "x" + sBBY;

		t->width = atoi(width.c_str());
		t->height = atoi(height.c_str());

		t->bBX = atoi(sBBX.c_str());
		t->bBY = atoi(sBBY.c_str());
	}
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
	sf::Vector2f coords;

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
		coords.y = (float)atoi(input.c_str()) * TILE_SIZE;
		std::getline(mapFile, input, '-');
		coords.x = (float)atoi(input.c_str()) * TILE_SIZE;

		//Get the total number of map tiles that move the same map. Allows for an area to be defined to allow for more map transition flexibility
		std::getline(mapFile, input, '-');
		numCoords = atoi(input.c_str());

		//Loop to find all possible map transition points and store the coordinates for the next map, and the map name to the file.
		for (int i = 0; i < numCoords; i++)
		{
			//Get the coordinates of a tile that is a transition point
			std::getline(mapFile, input, 'x');
			tileRow = atoi(input.c_str());
			std::getline(mapFile, input, ',');
			tileColumn = atoi(input.c_str());

			// Store the file name and coordinates for the spawning position in the next map
			mapLayer->addTransitionPoint(tileRow, tileColumn, mapFileName, coords);
		}

		std::getline(mapFile, input);
	}
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
	for (unsigned int j = 0; j < numRows; j++)
	{
		std::getline(mapFile, input, '_');

		//This loop instantiates all of the Tiles in the tile 2d array
		for (unsigned int i = 0; i < numColumns; i++)
		{
			pos = 0;
			pos = addTileToMap(input, pos , j, i);
	
			while (pos < input.size())
				pos = addTileToMap(input, pos , j, i);

			if (i < (numColumns - 1))
				std::getline(mapFile, input, '_');
		}

		std::getline(mapFile, input);
	}
}


unsigned short Map::addTileToMap(const std::string& input, unsigned short pos, unsigned short row, unsigned short column)
{
	// Local variables
	unsigned short realRow = row;
	int tileDataRow = input[pos] - '0';
	int tileDataColumn = input[pos + 1] - '0';
	int rotation = input[pos + 2] - '0';
	bool mirrored = false;
	Layer *L = 0;

	if (input[pos + 3] - '0' == 1)
		mirrored = true; //0 = false, 1 = true

	char tileType = input[pos + 5];
	
	// Add a transition point to the transition layer (if the current tile is a transition tile)
	if (tileType == 'E')
		transitionLayer->addVerticies(row, column, float(column * TILE_SIZE), float(row * TILE_SIZE), TILE_SIZE, TILE_SIZE);

	// Determine what layer the tile should be added to
	if (tileData[tileDataRow * tileDataColumns + tileDataColumn]->layer == 0)
		L = mapLayer;
	else if (tileData[tileDataRow * tileDataColumns + tileDataColumn]->layer == 1)
		L = maskLayer;
	else if (tileData[tileDataRow * tileDataColumns + tileDataColumn]->layer == 2)
	{
		L = groundLayers[row];
		row = 0; // Set row to zero since the ground layer is split into multiple layer strips
	}
	else if (tileData[tileDataRow * tileDataColumns + tileDataColumn]->layer == 3)
		L = canopyLayer;

	// Add grid to grid layer
	gridLayer->addVerticies(realRow, column, float(column * TILE_SIZE), float(realRow * TILE_SIZE), TILE_SIZE, TILE_SIZE);

	// Determine what kind of tile to add to the layer
	if (tileData[tileDataRow * tileDataColumns + tileDataColumn]->type == Tile_Data::Animated)
	{
		Animated_Tile at(*tileData[tileDataRow * tileDataColumns + tileDataColumn]->at);
		at.rotation = rotation;
		at.mirror = mirrored;
		at.tileType = tileType;

		if (rotation != 0)
			updateBoundingBox(&at);
		
		// Update light position
		if (at.light != 0)
			at.light->lightPos = sf::Vector2f(float(column * TILE_SIZE + (TILE_SIZE * 0.5f)), float(row * TILE_SIZE + TILE_SIZE));

		L->addTile(at, row, column);

		if (at.collidable)
		{
			if (tileData[tileDataRow * tileDataColumns + tileDataColumn]->layer == 1)
				mapLayer->changeTileCollision(row, column, false);

			collisionLayer->addVerticies(realRow, column, float(column * TILE_SIZE + at.bBX), float(realRow * TILE_SIZE + at.bBY), at.width, at.height);
		}
	}
	else
	{
		Tile t(*tileData[tileDataRow * tileDataColumns + tileDataColumn]->t);
		t.rotation = rotation;
		t.mirror = mirrored;
		t.tileType = tileType;

		if (rotation != 0)
			updateBoundingBox(&t);

		// Update light position
		if (t.light != 0)
			t.light->lightPos = sf::Vector2f(float(column * TILE_SIZE + (TILE_SIZE * 0.5f)), float(row * TILE_SIZE + TILE_SIZE));

		// Add tile to layer
		L->addTile(t, row, column);

		if (t.collidable)
		{
			if(tileData[tileDataRow * tileDataColumns + tileDataColumn]->layer == 1)
				mapLayer->changeTileCollision(row, column, false);

			collisionLayer->addVerticies(realRow, column, float(column * TILE_SIZE + t.bBX), float(realRow * TILE_SIZE + t.bBY), t.width, t.height);
		}	
	}

	return pos + 7;
}

void Map::updateBoundingBox(Tile *t)
{
	unsigned int transform = t->rotation;
	Tile *tD = tileData[t->row * tileDataColumns + t->column]->getTile();

	if (tD == 0)
		return;

	//This reverses the tiles height and width if the tile is flipped 90 degrees (1) or 270 degrees (3)
	if (transform == 1 || transform == 3)
	{
		t->width = tD->height;
		t->height = tD->width;
	}
	else
	{
		t->width = tD->width;
		t->height = tD->height;
	}

	//Sets the bounding box correctly if the tile is rotated
	if (transform == 0)
	{
		t->bBX = tD->bBX;
		t->bBY = tD->bBY;
	}
	else if (transform == 1) //Tile rotated 90 degrees
	{
		t->bBX = TILE_SIZE - tD->height - tD->bBY;
		t->bBY = TILE_SIZE - tD->width - tD->bBX;
	}
	else if (transform == 2) //Tile rotated 180 degrees
	{
		t->bBX = TILE_SIZE - tD->width - tD->bBX;
		t->bBY = TILE_SIZE - tD->height - tD->bBY;
	}
	else if (transform == 3) //Tile rotate 270 degrees
	{
		t->bBX = tD->bBY;
		t->bBY = tD->bBX;
	}
}

/*
emptyMap

This method simply releases the memory used to store the previous map.
*/
void Map::emptyMap()
{
	// Delete layers
	delete mapLayer;
	delete canopyLayer;
	delete maskLayer;

	for (unsigned int i = 0; i < numRows; i++)
		delete groundLayers[i];
	delete[] groundLayers;

	// Delete tool layers
	delete gridLayer;
	delete collisionLayer;
	delete transitionLayer;

	for (unsigned int i = 0; i < tileData.size(); i++)
		delete tileData[i];
	tileData.clear();
}

void Map::setLightInterval(const sf::Vector2u& interval)
{
	lightInterval = interval;
}

void Map::updateDrawList(Player* player, const sf::Time& currentTime, const pb::Time& currentInGameTime, bool animate)
{
	//LOCAL VARIABLES
	std::vector<std::vector<pb::Graphic_Entity*>> drawAtPos;
	bool playerDrawn = false;

	// Determine if lights should be on or off
	if (lightInterval == sf::Vector2u(0, 0) || (unsigned int)currentInGameTime.hours >= lightInterval.x || (unsigned int)currentInGameTime.hours <= lightInterval.y)
		Tile::lightsOn = true;
	else
		Tile::lightsOn = false;

	// Determine which column to start from
	unsigned int startColumn = unsigned int((Game::camera->getCenter().x - Game::camera->getSize().x) / TILE_SIZE);
	if (Game::camera->getSize().x / TILE_SIZE >= numColumns || Game::camera->getCenter().x < Game::camera->getSize().x)
		startColumn = 0;

	// Determine which row to start from
	unsigned int startRow = unsigned int((Game::camera->getCenter().y - Game::camera->getSize().y) / TILE_SIZE);
	if (Game::camera->getSize().y / TILE_SIZE >= numRows || Game::camera->getCenter().y < Game::camera->getSize().y)
		startRow = 0;

	// Cull out tiles based on view size
	unsigned int viewWidth = unsigned int((Game::camera->getCenter().x + Game::camera->getSize().x) / TILE_SIZE);
	unsigned int viewHeight = unsigned int((Game::camera->getCenter().y + Game::camera->getSize().y) / TILE_SIZE);

	// If the view extends past the borders of the map, clamp the width/height to numColumns - 1/numRows - 1
	if (viewWidth >= numColumns)
		viewWidth = numColumns - 1;
	if (viewHeight >= numRows)
		viewHeight = numRows - 1;

	// Resize the vector to include only what will be shown
	drawAtPos.resize(viewHeight + 1);

	// Clear verticies
	mapLayer->clearVertexArray();
	maskLayer->clearVertexArray();
	canopyLayer->clearVertexArray();
	collisionLayer->clearVertexArray();
	gridLayer->clearVertexArray();
	transitionLayer->clearVertexArray();
	for (unsigned int i = 0; i < numRows; i++)
		groundLayers[i]->clearVertexArray();

	// Draw ONLY the visible tiles
	for (unsigned int i = startRow; i <= viewHeight; i++)
	{
		for (unsigned int j = startColumn; j <= viewWidth; j++)
		{
			mapLayer->update(TILE_SIZE, i, j, currentTime, *Game::graphicManager);
			maskLayer->update(TILE_SIZE, i, j, currentTime, *Game::graphicManager);
			groundLayers[i]->update(TILE_SIZE, i, j, currentTime, *Game::graphicManager);
			canopyLayer->update(TILE_SIZE, i, j, currentTime, *Game::graphicManager);

			//TOOLS
			if (renderCollisionLayer)
				collisionLayer->update(i, j);
			if (renderGridLayer)
			{
				if (i % 2 == 0)
					gridLayer->update(i, j);
				else
					gridLayer->update(i, viewWidth - (j - startColumn));
			}
			if (renderTransitionLayer)
				transitionLayer->update(i, j);
		}
	}

	Game::graphicManager->addToDrawList(mapLayer);
	Game::graphicManager->addToDrawList(maskLayer);

	//Get the column and row the graphic object is in
	int row = (int)(player->getRect().top + player->getRect().height) / TILE_SIZE;
	int columnL = (int)player->getRect().left / TILE_SIZE;
	int columnR = (int)(player->getRect().left + player->getRect().width) / TILE_SIZE;
	
	// Make sure the row and columns calculated are not going out of bounds
	row = (row == numRows ? numRows - 1 : row);
	columnL = (columnL < 0 ? 0 : columnL);
	columnR = (columnR == numColumns ? numColumns - 1 : columnR);

	const Tile *t = groundLayers[row]->getTile(0, columnL), *t2 = groundLayers[row]->getTile(0, columnR);
	//Determine the order that graphic objects are drawn based on their immediate surroundings.
	if (t != 0 || t2 != 0)
	{
		row++;
		if (t != 0 && (row * TILE_SIZE) + t->bBY > player->getRect().top ||
			t2 != 0 && (row * TILE_SIZE) + t2->bBY > player->getRect().top)
			row--;		
	}

	drawAtPos[row].push_back(player);

	//Check each row for collision with the player or with npc's
	for (unsigned int i = startRow; i <= viewHeight; i++)
	{
		for (unsigned int j = 0; j < drawAtPos[i].size(); j++)
			drawAtPos[i][j]->updateDrawList();

		Game::graphicManager->addToDrawList(groundLayers[i], true);
	}

	// Check the last position in the vector
	if (drawAtPos[drawAtPos.size() - 1].size() != 0)
	{
		for (unsigned int i = 0; i < drawAtPos[drawAtPos.size() - 1].size(); i++)
			drawAtPos[drawAtPos.size() - 1][i]->updateDrawList();
	}

	Game::graphicManager->addToDrawList(canopyLayer, true);

	//TOOLS
	if (renderCollisionLayer)
		Game::graphicManager->addToDrawList(collisionLayer);
	if (renderGridLayer)
		Game::graphicManager->addToDrawList(gridLayer);
	if (renderTransitionLayer)
		Game::graphicManager->addToDrawList(transitionLayer);
}


/*
moveToMap
Parameters:
	player: The player character will have its position set to the indicated spawning point
	camera: The camera will have its position updated to center on the player

This method moves from one map to another when a transition tile is collided with.
*/
const std::string Map::moveToMap(Player* player)
{
	//LOCAL VARIABLES
	int row = (int)player->getRect().top / TILE_SIZE;
	int column = (int)player->getRect().left / TILE_SIZE;
	
	std::string mapToLoad = mapLayer->getTile(row, column)->mapName;
	
	sf::Vector2f startPosition = mapLayer->getTile(row, column)->transitionCoords;
	startPosition.x += (TILE_SIZE / 2);
	startPosition.y += (TILE_SIZE / 2);

	loadMap(mapToLoad); //Load the next map

	player->setPosition(startPosition); 
	Game::camera->updatePosition(sf::Vector2f((float)startPosition.x, (float)startPosition.y));

	return mapToLoad;
}

const sf::Vector2i Map::getMapSize()
{
	return sf::Vector2i(numColumns * TILE_SIZE, numRows * TILE_SIZE);
}

/*
collisionDetected
Parameters:
	rect: This rectangle contains the boundaries of some graphic entity that needs to be checked.

This method checks for collision between a tile and some entity that exists within the map. Returns true 
if collision has been detected. False otherwise.
*/
bool Map::collisionDetected(const sf::IntRect& rect)
{
	//If the entity is at the maps edge along the x-axis, return true
	if ((unsigned int)rect.left < 0 || (unsigned int)rect.left + rect.width > numColumns * TILE_SIZE)
		return true;

	//If the entity is at the maps edge along the y-axis, return true
	if ((unsigned int)(rect.top + rect.height + 2) - TILE_SIZE < 0 || (unsigned int)(rect.top + rect.height + 2) > numRows * TILE_SIZE)
		return true;
	
	unsigned int row = (rect.top + rect.height) / TILE_SIZE, column = rect.left / TILE_SIZE;
	
	// Make sure the row and column calculated have not gone out of bounds
	row = (row == numRows ? numRows - 1 : row);
	column = (column == numColumns ? numColumns - 1 : column);
	
	bool collision = false;

	//Check collision of the player on the left side of player
	if (mapLayer->getTile(row, column)->collidable)
		collision = mapLayer->isColliding(rect, row, column, TILE_SIZE);
	else if (groundLayers[row]->getTile(0, column) != 0 && groundLayers[row]->getTile(0, column)->collidable)
		collision = groundLayers[row]->isColliding(rect, row, column, TILE_SIZE);
	else if (maskLayer->getTile(row, column) != 0 && maskLayer->getTile(row, column)->collidable)
		collision = maskLayer->isColliding(rect, row, column, TILE_SIZE);
	
	//Return true if collision has been detected
	if (collision)
		return collision;

	//Check collision of the player on the right side of the player
	column = (unsigned int)((rect.left + rect.width) / TILE_SIZE);
	if (mapLayer->getTile(row, column)->collidable)
		collision = mapLayer->isColliding(rect, row, column, TILE_SIZE);
	else if (groundLayers[row]->getTile(0, column) != 0 && groundLayers[row]->getTile(0, column)->collidable)
		collision = groundLayers[row]->isColliding(rect, row, column, TILE_SIZE);
	else if (maskLayer->getTile(row, column) != 0 && maskLayer->getTile(row, column)->collidable)
		collision = maskLayer->isColliding(rect, row, column, TILE_SIZE);

	return collision; //Return collision
}

/*
transitioning

This method is used to determine if the game state should switch to the transitioning state or not.
*/
bool Map::transitioning(Player* player)
{
	//This checks to see if the tile being moved to is a transition tile
	if (mapLayer->getTile((int)player->getRect().top / TILE_SIZE, (int)player->getRect().left / TILE_SIZE)->tileType == 'E')
		return true; 

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

void Map::displayTransitionLayer()
{
	if (!renderTransitionLayer)
		renderTransitionLayer = true;
	else
		renderTransitionLayer = false;
}

/*
~Map

This destructor frees up the memory that was dynamically allocated for the 
2d array that stores the map.
*/
Map::~Map()
{
	// Delete layers
	delete mapLayer;
	delete canopyLayer;
	delete maskLayer;

	for (unsigned int i = 0; i < numRows; i++)
		delete groundLayers[i];
	delete[] groundLayers;

	// Delete tool layers
	delete gridLayer;
	delete collisionLayer;
	delete transitionLayer;

	for (unsigned int i = 0; i < tileData.size(); i++)
		delete tileData[i];
	tileData.clear();
}