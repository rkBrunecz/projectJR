#include "MapEditor/Editable_Map.h"
#include "MapEditor/Map_Editor.h"
#include "MapEditor/UI.h"

Editable_Map::Editable_Map()
{
	pb::addToCollisionList(this);

	mousePos.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	mousePos.setFillColor(sf::Color(0, 0, 200, 125));

	selectedTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	selectedTile.setFillColor(sf::Color(200, 200, 0, 125));
}

Editable_Map::Editable_Map(pb::Graphic_Manager *tilePaneManager, const sf::Vector2i& tileSheetCoords)
{
	pb::addToCollisionList(this);

	mousePos.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	mousePos.setFillColor(sf::Color(0, 0, 200, 125));

	selectedTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	selectedTile.setFillColor(sf::Color(200, 200, 0, 125));

	// Set tile sizes
	deleteTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	deleteTransTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	transitionTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	rotationTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	mirrorTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	testMapTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));

	// Set tile textures
	deleteTile.setTexture(tilePaneManager->addTexture("MapEditorTiles.png"));
	deleteTransTile.setTexture(tilePaneManager->addTexture("MapEditorTiles.png"));
	transitionTile.setTexture(tilePaneManager->addTexture("MapEditorTiles.png"));
	rotationTile.setTexture(tilePaneManager->addTexture("MapEditorTiles.png"));
	mirrorTile.setTexture(tilePaneManager->addTexture("MapEditorTiles.png"));
	testMapTile.setTexture(tilePaneManager->addTexture("MapEditorTiles.png"));
	testMapTile.setTexture(tilePaneManager->addTexture("MapEditorTiles.png"));

	// Set texture rects
	deleteTile.setTextureRect(sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE));
	deleteTransTile.setTextureRect(sf::IntRect(TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
	transitionTile.setTextureRect(sf::IntRect(TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE));
	mirrorTile.setTextureRect(sf::IntRect(TILE_SIZE * 3, 0, TILE_SIZE, TILE_SIZE));
	rotationTile.setTextureRect(sf::IntRect(TILE_SIZE * 4, 0, TILE_SIZE, TILE_SIZE));
	testMapTile.setTextureRect(sf::IntRect(TILE_SIZE * 5, 0, TILE_SIZE, TILE_SIZE));

	// Load font, and set text properties
	pb::System::load(&font, "arial.ttf");
	currentRowColumn.setFont(font);
	currentRowColumn.setCharacterSize(12);
	currentRowColumn.setFillColor(sf::Color::Black);

	currentTime.setFont(font);
	currentTime.setCharacterSize(24);
	currentTime.setFillColor(sf::Color::Black);

	initializeTileSheetCoords(tileSheetCoords);
}

void Editable_Map::initializeTileSheetCoords(const sf::Vector2i& tileSheetCoords)
{
	this->tileSheetCoords = sf::Vector2i(tileSheetCoords.x + (TILE_SIZE / 2), tileSheetCoords.y + (TILE_SIZE * 2));

	tileSheetSprite.setPosition((float)this->tileSheetCoords.x, (float)this->tileSheetCoords.y);

	currentRowColumn.setPosition(sf::Vector2f(tileSheetCoords));

	testMapTile.setPosition((float)tileSheetCoords.x + (TILE_SIZE * 1.5f), (float)TILE_SIZE);
	mirrorTile.setPosition((float)tileSheetCoords.x + (TILE_SIZE * 3), (float)TILE_SIZE);
	rotationTile.setPosition((float)tileSheetCoords.x + (TILE_SIZE * 4.5f), (float)TILE_SIZE);
	transitionTile.setPosition((float)tileSheetCoords.x + (TILE_SIZE * 6), (float)TILE_SIZE);
	deleteTransTile.setPosition((float)tileSheetCoords.x + (TILE_SIZE * 7.5f), (float)TILE_SIZE);
	deleteTile.setPosition((float)tileSheetCoords.x + (TILE_SIZE * 9), (float)TILE_SIZE);
}

void Editable_Map::loadMap(std::string mapName, pb::Graphic_Manager* graphicManager, pb::Camera* camera)
{
	if (mapName.compare(".jrm") < 0)
		return;

	//LOCAL VARIABLES
	std::ifstream mapFile;

	//Reset all tile manipulations and debug layers
	renderCollisionLayer = false;
	renderGridLayer = false;
	renderTransitionLayer = false;

	//If the rows and columns of the map have been set, it is safe to clear out the map file.
	if (numRows != 0 && numColumns != 0)
		emptyMap(); //Empty out the map file to make room for a new map.

	//Open the mapFile file
	mapFile.open(mapName);
	nameOfFile = mapName;
	nameOfFile = std::strstr(mapName.c_str(), "res\\Maps\\");
	nameOfFile[3] = '/';
	nameOfFile[8] = '/';
	
	if (mapFile.is_open())
		initialize(mapFile, graphicManager, camera); //Dynamically create an array to hold the map	
	else
	{
		printf("\"%s\" could not be found!\n", mapName.c_str());
		system("pause");
		exit(EXIT_FAILURE); //Close the application with a failure code if the file does not open
	}

	mapFile.close(); //Close the file

	mapLoaded = true;
}

void Editable_Map::createMap(unsigned int rows, unsigned int columns, std::string mapName, std::string sheetFileName, pb::Graphic_Manager* graphicManager, pb::Camera* camera)
{
	std::ifstream tileFile;
	std::string input;

	//Clear out the transitions if they existed in a previously loaded map.
	if (numTransitionPoints != 0)
		transitions.clear();

	//Reset all tile manipulations and debug layers
	renderCollisionLayer = false;
	renderGridLayer = false;
	renderTransitionLayer = false;
	numTransitionPoints = 0;

	//If the rows and columns of the map have been set, it is safe to clear out the map file.
	if (numRows != 0 && numColumns != 0)
		emptyMap(); //Empty out the map file to make room for a new map.

	//LOCAL VARIABLES
	numRows = rows;
	numColumns = columns;

	nameOfFile = mapName;
	nameOfSheetFile = sheetFileName;

	sheetFileName.replace(sheetFileName.length() - 3, 3, "png");
	nameOfTileSheet = sheetFileName.substr(sheetFileName.find_last_of("\\") + 1, sheetFileName.size());

	//Get the tile data so that tiles can be added to the map
	initializeTileData(nameOfSheetFile);

	tileFile.close(); //Close the tile file

	//Get the path to the tile map and then open it.	
	tileSheetSprite.setTexture(*graphicManager->addTexture(nameOfTileSheet));;

	// Initialize layers
	mapLayer = new Layer(nameOfTileSheet, numRows, numColumns, graphicManager);
	canopyLayer = new Layer(nameOfTileSheet, numRows, numColumns, graphicManager);
	maskLayer = new Layer(nameOfTileSheet, numRows, numColumns, graphicManager);

	groundLayers = new Layer*[numRows];
	for (unsigned int i = 0; i < numRows; i++)
		groundLayers[i] = new Layer(nameOfTileSheet, 1, numColumns, graphicManager);

	// Tools
	collisionLayer = new Primitive_Layer(numRows, numColumns, sf::Quads, sf::Color(255, 0, 0, 150));
	transitionLayer = new Primitive_Layer(numRows, numColumns, sf::Quads, sf::Color(0, 255, 0, 255));
	gridLayer = new Primitive_Layer(numRows, numColumns, sf::LinesStrip, sf::Color::Black);

	camera->setBounds(float(numColumns * TILE_SIZE), float(numRows * TILE_SIZE));
	
	for (unsigned int i = 0; i < numRows; i++)
	{
		for (unsigned int j = 0; j < numColumns; j++)
		{
			mapLayer->addTile(*tileData[0]->getTile(), i, j);
			gridLayer->addVerticies(i, j, float(j * TILE_SIZE), float(i * TILE_SIZE), TILE_SIZE, TILE_SIZE);
		}
	}

	mapLoaded = true;
}

void Editable_Map::initialize(std::ifstream& mapFile, pb::Graphic_Manager* graphicManager, pb::Camera* camera)
{
	//LOCAL VARIABLES
	std::string input, //Used to get input from the getline method from the file
		mapRows,  //Used to store the total number of rows in the map that is read from the file.
		mapColumns; //Used to store the total number of columns in the map that is read from the file.
	std::ifstream tileFile;

	// Get jrs and initialize tile data
	std::getline(mapFile, nameOfSheetFile);
	initializeTileData(nameOfSheetFile);

	//Get the path to the tile map and then open it.
	std::getline(mapFile, nameOfTileSheet);
	tileSheetSprite.setTexture(*graphicManager->addTexture(nameOfTileSheet));;

	//Get the total number of rows and columns from the file and store them
	std::getline(mapFile, mapRows, 'x');
	std::getline(mapFile, mapColumns);

	//Convert the parsed strings to ints
	numRows = atoi(mapRows.c_str());
	numColumns = atoi(mapColumns.c_str());

	// Initialize layers
	mapLayer = new Layer(nameOfTileSheet, numRows, numColumns, graphicManager);
	canopyLayer = new Layer(nameOfTileSheet, numRows, numColumns, graphicManager);
	maskLayer = new Layer(nameOfTileSheet, numRows, numColumns, graphicManager);

	groundLayers = new Layer*[numRows];
	for (unsigned int i = 0; i < numRows; i++)
		groundLayers[i] = new Layer(nameOfTileSheet, 1, numColumns, graphicManager);

	// Tools
	collisionLayer = new Primitive_Layer(numRows, numColumns, sf::Quads, sf::Color(255, 0, 0, 150));
	transitionLayer = new Primitive_Layer(numRows, numColumns, sf::Quads, sf::Color(0, 255, 0, 255));
	gridLayer = new Primitive_Layer(numRows, numColumns, sf::LinesStrip, sf::Color::Black);

	camera->setBounds(float(numColumns * TILE_SIZE), float(numRows * TILE_SIZE));

	initializeTransitionPoints(mapFile); //Intialize all transition points in the map

	populateMap(mapFile); //Fill the array with the map data
}

void Editable_Map::initializeTransitionPoints(std::ifstream& mapFile)
{
	//LOCAL VARIABLES
	std::string input;
	int numCoords;
	sf::Vector2f coords;

	if (numTransitionPoints != 0)
		transitions.clear();

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
		coords.y = static_cast<float>(atoi(input.c_str()) * TILE_SIZE);
		std::getline(mapFile, input, '-');
		coords.x = static_cast<float>(atoi(input.c_str()) * TILE_SIZE);

		p.startingCoords = sf::Vector2i(static_cast<int>(coords.x / TILE_SIZE), static_cast<int>(coords.y / TILE_SIZE));

		//Get the total number of map tiles that move to the same map. Allows for an area to be defined to allow for more map transition flexibility
		std::getline(mapFile, input, '-');
		numCoords = atoi(input.c_str());

		p.numCoords = numCoords;

		//Loop to find all possible map transition points and store the coordinates for the next map, and the map name to the file.
		for (int i = 0; i < numCoords; i++)
		{
			sf::Vector2i tp;

			//Get the coordinates of a tile that is a transition point
			std::getline(mapFile, input, 'x');
			tp.y = atoi(input.c_str());
			std::getline(mapFile, input, ',');
			tp.x = atoi(input.c_str());

			// Store the file name and coordinates for the spawning position in the next map
			mapLayer->addTransitionPoint(tp.y, tp.x, p.transitionMapName, coords);

			p.transitionPoints.push_back(tp);
		}

		transitions.push_back(p);
		std::getline(mapFile, input);
	}
}

void Editable_Map::addTileToMap(Tile_Data& tD, unsigned int row, unsigned int column)
{
	// Local variables
	unsigned short realRow = row;
	Layer *L = 0;

	// Determine what layer the tile should be added to
	if (tD.layer == 0)
		L = mapLayer;
	else if (tD.layer == 1)
		L = maskLayer;
	else if (tD.layer == 2)
	{
		L = groundLayers[row];
		row = 0; // Set row to zero since the ground layer is split into multiple layer strips
	}
	else if (tD.layer == 3)
		L = canopyLayer;

	// Determine what kind of tile to add to the layer
	if (tD.type == Tile_Data::Animated)
	{
		Animated_Tile at(*tD.at);

		if (at.collidable)
		{
			if (tD.layer == 1)
				mapLayer->changeTileCollision(row, column, false);

			collisionLayer->addVerticies(realRow, column, float(column * TILE_SIZE + at.bBX), float(realRow * TILE_SIZE + at.bBY), at.width, at.height);
		}
		else if (L->getTile(row, column) != 0 && L->getTile(row, column)->collidable && !at.collidable)
			collisionLayer->removeVerticies(row, column);

		// Update light position
		if (at.light != 0)
			at.light->lightPos = sf::Vector2f(float(column * TILE_SIZE + (TILE_SIZE * 0.5f)), float(row * TILE_SIZE + TILE_SIZE));


		L->addTile(at, row, column);
	}
	else
	{
		Tile t(*tD.t);

		if (t.collidable)
		{
			if (tD.layer == 1)
				mapLayer->changeTileCollision(row, column, false);

			collisionLayer->addVerticies(realRow, column, float(column * TILE_SIZE + t.bBX), float(realRow * TILE_SIZE + t.bBY), t.width, t.height);
		}
		else if (L->getTile(row, column) != 0 && L->getTile(row, column)->collidable && !t.collidable)
			collisionLayer->removeVerticies(row, column);

		// Update light position
		if (t.light != 0)
			t.light->lightPos = sf::Vector2f(float(column * TILE_SIZE + (TILE_SIZE * 0.5f)), float(row * TILE_SIZE + TILE_SIZE));

		L->addTile(t, row, column);
	}
}

void Editable_Map::updateDrawList(sf::RenderWindow *window, pb::Graphic_Manager *graphicManager, const pb::Camera& camera, const sf::Vector2f& mouseCoords, const sf::Time& currentTime, const pb::Time& inGameTime)
{
	// Determine if lights should be on or off
	if (lightInterval == sf::Vector2u(0, 0) || (unsigned int)inGameTime.hours >= lightInterval.x || (unsigned int)inGameTime.hours < lightInterval.y)
		Tile::lightsOn = true;
	else
		Tile::lightsOn = false;

	// Cull out unnecessary tiles
	unsigned int startColumn = unsigned int((camera.getCenter().x - camera.getSize().x) / TILE_SIZE);
	if (camera.getSize().x / TILE_SIZE >= numColumns || camera.getCenter().x < camera.getSize().x)
		startColumn = 0;

	unsigned int startRow = unsigned int((camera.getCenter().y - camera.getSize().y) / TILE_SIZE);
	if (camera.getSize().y / TILE_SIZE >= numRows || camera.getCenter().y < camera.getSize().y)
		startRow = 0;

	unsigned int viewWidth = unsigned int((camera.getCenter().x + camera.getSize().x) / TILE_SIZE);
	unsigned int viewHeight = unsigned int((camera.getCenter().y + camera.getSize().y) / TILE_SIZE);

	// If the view extends past the borders of the map, clamp the width/height to numColumns - 1/numRows - 1
	if (viewWidth >= numColumns)
		viewWidth = numColumns - 1;
	if (viewHeight >= numRows)
		viewHeight = numRows - 1;

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
			mapLayer->update(TILE_SIZE, i, j, currentTime, *graphicManager);
			maskLayer->update(TILE_SIZE, i, j, currentTime, *graphicManager);
			groundLayers[i]->update(TILE_SIZE, i, j, currentTime, *graphicManager);
			canopyLayer->update(TILE_SIZE, i, j, currentTime, *graphicManager);

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

	graphicManager->addToDrawList(mapLayer);
	graphicManager->addToDrawList(maskLayer);
	for (unsigned int i = 0; i < numRows; i++)
		graphicManager->addToDrawList(groundLayers[i], true);
	graphicManager->addToDrawList(canopyLayer, true); //Draw the canopy

	//TOOLS
	if (renderCollisionLayer)
		graphicManager->addToDrawList(collisionLayer);
	if (renderGridLayer)
		graphicManager->addToDrawList(gridLayer);
	if (renderTransitionLayer)
		graphicManager->addToDrawList(transitionLayer);

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		mousePos.setPosition(sf::Vector2f(floor(mouseCoords.x / TILE_SIZE) * TILE_SIZE,
			floor(mouseCoords.y / TILE_SIZE) * TILE_SIZE));

		if (mousePos.getPosition().x <= TILE_SIZE * (numColumns - 1) && mousePos.getPosition().y <= TILE_SIZE * (numRows - 1) && mousePos.getPosition().x >= 0 && mousePos.getPosition().y >= 0)
			graphicManager->addToDrawList(&mousePos);
		else
			window->setMouseCursorVisible(true);
	}
}


void Editable_Map::updateTileSheet(const sf::RenderWindow& window, pb::Graphic_Manager *tilePaneManager, const sf::Vector2f& mousePos, pb::In_Game_Clock& gameClock)
{
	tilePaneManager->addToDrawList(&tileSheetSprite);
	tilePaneManager->addToDrawList(&deleteTile);
	tilePaneManager->addToDrawList(&deleteTransTile);
	tilePaneManager->addToDrawList(&transitionTile);
	tilePaneManager->addToDrawList(&rotationTile);
	tilePaneManager->addToDrawList(&mirrorTile);
	tilePaneManager->addToDrawList(&testMapTile);

	// Get game time
	pb::Time t = gameClock.getTime();

	// Set the current time string
	if (t.hours < 10)
		currentTime.setString("0" + std::to_string(t.hours));
	else
		currentTime.setString(std::to_string(t.hours));

	if (t.minutes < 10)
		currentTime.setString(currentTime.getString() + ":0" + std::to_string(t.minutes));
	else
		currentTime.setString(currentTime.getString() + ":" + std::to_string(t.minutes));

	// Update position of current time string and add it to draw list
	currentTime.setPosition(sf::Vector2f((float)tileSheetCoords.x, float(window.getSize().y - currentTime.getCharacterSize() * 2)));
	tilePaneManager->addToDrawList(&currentTime);

	if (mousePos.x <= numColumns * TILE_SIZE && sf::Mouse::getPosition(window).x <= tileSheetCoords.x && mousePos.x >= 0 &&
		sf::Mouse::getPosition(window).y >= TILE_SIZE && mousePos.y <= numRows * TILE_SIZE && mousePos.y >= 0)
	{
		currentRowColumn.setString(std::to_string((int)(mousePos.y / TILE_SIZE)) + ", " + std::to_string((int)(mousePos.x / TILE_SIZE)));
		tilePaneManager->addToDrawList(&currentRowColumn);
	}

	if (currentTile.specialTile.compare("None") != 0 || currentTile.pos >= 0)
		tilePaneManager->addToDrawList(&selectedTile);
}

bool Editable_Map::isMapLoaded()
{
	return mapLoaded;
}

bool Editable_Map::inMapBounds(const sf::Vector2f& coords)
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (floor(coords.x / TILE_SIZE) <= (numColumns - 1) && floor(coords.y / TILE_SIZE) <= (numRows - 1) && coords.x >= 0 && coords.y >= 0)
			return true;
	}

	return false;
}

void Editable_Map::setTile(const sf::Vector2i& mouseCoords)
{
	if (mouseCoords.x > deleteTile.getPosition().x && mouseCoords.x < deleteTile.getPosition().x + TILE_SIZE &&
		mouseCoords.y > deleteTile.getPosition().y && mouseCoords.y < deleteTile.getPosition().y + TILE_SIZE)
	{
		currentTile.setSpecialTile("Delete");
		selectedTile.setPosition(deleteTile.getPosition());
		renderTransitionLayer = false; //Turn off the transition layer
		return;
	}
	else if (mouseCoords.x > deleteTransTile.getPosition().x && mouseCoords.x < deleteTransTile.getPosition().x + TILE_SIZE &&
		mouseCoords.y > deleteTransTile.getPosition().y && mouseCoords.y < deleteTransTile.getPosition().y + TILE_SIZE)
	{
		currentTile.setSpecialTile("DeleteTransition");
		selectedTile.setPosition(deleteTransTile.getPosition());
		renderTransitionLayer = true;
		return;
	}
	else if (mouseCoords.x > rotationTile.getPosition().x && mouseCoords.x < rotationTile.getPosition().x + TILE_SIZE &&
		mouseCoords.y > rotationTile.getPosition().y && mouseCoords.y < rotationTile.getPosition().y + TILE_SIZE)
	{
		currentTile.setSpecialTile("Rotate");
		selectedTile.setPosition(rotationTile.getPosition());
		renderTransitionLayer = false; //Turn off the transition layer
		return;
	}
	else if (mouseCoords.x > mirrorTile.getPosition().x && mouseCoords.x < mirrorTile.getPosition().x + TILE_SIZE &&
		mouseCoords.y > mirrorTile.getPosition().y && mouseCoords.y < mirrorTile.getPosition().y + TILE_SIZE)
	{
		currentTile.setSpecialTile("Mirror");
		selectedTile.setPosition(mirrorTile.getPosition());
		renderTransitionLayer = false; //Turn off the transition layer
		return;
	}
	else if (mouseCoords.x > transitionTile.getPosition().x && mouseCoords.x < transitionTile.getPosition().x + TILE_SIZE &&
		mouseCoords.y > transitionTile.getPosition().y && mouseCoords.y < transitionTile.getPosition().y + TILE_SIZE)
	{
		currentTile.setSpecialTile("Transition");
		selectedTile.setPosition(transitionTile.getPosition());
		renderTransitionLayer = true;
		return;
	}
	else if (mouseCoords.x > testMapTile.getPosition().x && mouseCoords.x < testMapTile.getPosition().x + TILE_SIZE &&
		mouseCoords.y > testMapTile.getPosition().y && mouseCoords.y < testMapTile.getPosition().y + TILE_SIZE)
	{
		currentTile.setSpecialTile("TestMap");
		selectedTile.setPosition(testMapTile.getPosition());
		return;
	}

	if (mouseCoords.x < tileSheetCoords.x || mouseCoords.y < tileSheetCoords.y)
		return;

	renderTransitionLayer = false; //Turn off the transition layer

	//LOCAL VARIABLES
	unsigned int column = (mouseCoords.x - tileSheetCoords.x) / TILE_SIZE;
	unsigned int row = (mouseCoords.y - tileSheetCoords.y) / TILE_SIZE;
	unsigned int pos = row * tileDataColumns + column;

	if (column > tileDataColumns || row > 19)
		return;

	if (tileData[pos] != 0)
	{
		selectedTile.setPosition(sf::Vector2f(float(tileSheetCoords.x + (column * TILE_SIZE)), float(tileSheetCoords.y + (row * TILE_SIZE))));
		currentTile.specialTile = "None";
		currentTile.pos = pos;
	}
}

void Editable_Map::addTileToPos(sf::RenderWindow *window, const pb::Time& t)
{
	if (currentTile.pos >= 0 && tileData[currentTile.pos] == 0)
		return;

	//LOCAL VARIABLES
	unsigned int column = unsigned int(mousePos.getPosition().x / TILE_SIZE);
	unsigned int row = unsigned int(mousePos.getPosition().y / TILE_SIZE);

	if (column > numColumns - 1 || row > numRows - 1 || column < 0 || row < 0)
		return;

	if (currentTile.specialTile.compare("Delete") == 0)
		deleteTileFromPos(row, column);
	else if (currentTile.specialTile.compare("DeleteTransition") == 0)
		deleteTransitionPoint(row, column);
	else if (currentTile.specialTile.compare("Rotate") == 0)
		rotateTile(row, column);
	else if (currentTile.specialTile.compare("Mirror") == 0)
		mirrorTileAtPos(row, column);
	else if (currentTile.specialTile.compare("Transition") == 0)
		setTransitionPoint(row, column, window);
	else if (currentTile.specialTile.compare("TestMap") == 0)
		testMap(row, column, t);
	else if (currentTile.pos != -1)
		addTileToMap(*tileData[currentTile.pos], row, column);
}

void Editable_Map::deleteTileFromPos(int row, int column)
{
	Layer *L = 0;

	if (canopyLayer->getTile(row, column) != 0)
		L = canopyLayer;
	else if (groundLayers[row]->getTile(row, column) != 0)
		L = groundLayers[row];
	else if (maskLayer->getTile(row, column) != 0)
	{
		Tile *t = mapLayer->getTile(row, column);

		if (maskLayer->getTile(row, column)->collidable && t != 0)
		{
			mapLayer->changeTileCollision(row, column, tileData[t->row * tileDataColumns + t->column]->getTile()->collidable);
			collisionLayer->addVerticies(row, column, float(column * TILE_SIZE + t->bBX), float(row * TILE_SIZE + t->bBY), t->width, t->height);
		}
		
		L = maskLayer;
	}

	if (L == 0)
		return;

	L->removeTile(row, column);

	// Remove collision block from collision layer if no layer has a collidable tile
	if (canopyLayer->getTile(row, column) != 0 && !canopyLayer->getTile(row, column)->collidable &&
		groundLayers[row]->getTile(row, column) != 0 && !groundLayers[row]->getTile(row, column)->collidable &&
		maskLayer->getTile(row, column) != 0 && !maskLayer->getTile(row, column)->collidable &&
		mapLayer->getTile(row, column) != 0 && !mapLayer->getTile(row, column)->collidable)
		collisionLayer->removeVerticies(row, column);
}

void Editable_Map::deleteTransitionPoint(int row, int column)
{
	if (mapLayer->getTile(row, column)->mapName.compare("") == 0)
		return;

	//LOCAL VARIABLES
	unsigned int i = 0;
	bool tileRemoved = false;
	sf::Vector2i transitionCoordsAtPos;

	transitionCoordsAtPos.x = (int)mapLayer->getTile(row, column)->transitionCoords.x / TILE_SIZE;
	transitionCoordsAtPos.y = (int)mapLayer->getTile(row, column)->transitionCoords.y / TILE_SIZE;

	while (i < transitions.size() && !tileRemoved)
	{
		if (transitions[i].transitionMapName.compare(mapLayer->getTile(row, column)->mapName) == 0 && transitions[i].startingCoords == transitionCoordsAtPos)
		{
			//ADDITIONAL LOCAL VARIABLES
			unsigned int j = 0;
			bool coordsFound = false;

			while (j < transitions[i].transitionPoints.size() && !coordsFound)
			{
				if (transitions[i].transitionPoints[j] == sf::Vector2i(column, row))
					coordsFound = true;
				else
					j++;
			}

			transitions[i].transitionPoints.erase(transitions[i].transitionPoints.begin() + j, transitions[i].transitionPoints.begin() + j + 1);
			transitions[i].numCoords--;

			if (transitions[i].transitionPoints.size() == 0)
			{
				transitions.erase(transitions.begin() + i, transitions.begin() + i + 1);
				numTransitionPoints--;
			}

			mapLayer->removeTransitionPoint(row, column);
			transitionLayer->removeVerticies(row, column);

			tileRemoved = true;
		}

		i++;
	}
}

void Editable_Map::rotateTile(int row, int column)
{
	Layer *L = 0;

	// Determine layer to apply rotation to
	if (canopyLayer->getTile(row, column) != 0)
		L = canopyLayer;
	else if (groundLayers[row]->getTile(row, column) != 0)
		L = groundLayers[row];
	else if (maskLayer->getTile(row, column) != 0)
		L = maskLayer;
	else if (mapLayer->getTile(row, column) != 0)
		L = mapLayer;
	else // No layer found
		return;

	Tile *t = L->getTile(row, column);
	
	// Roate tile. Rotations are as follows: 1 = 0 degrees, 1 = 90 degrees, 2 = 180 degrees, 3 = 270 degrees
	t->rotation += 1;
	if (t->rotation > 3)
		t->rotation = 0;

	updateBoundingBox(t);

	if (t->collidable)
		collisionLayer->addVerticies(row, column, float(column * TILE_SIZE + t->bBX), float(row * TILE_SIZE + t->bBY), t->width, t->height);
}

void Editable_Map::mirrorTileAtPos(int row, int column)
{
	Layer *L = 0;

	// Determine layer to apply the mirror to
	if (canopyLayer->getTile(row, column) != 0)
		L = canopyLayer;
	else if (groundLayers[row]->getTile(row, column) != 0)
		L = groundLayers[row];
	else if (maskLayer->getTile(row, column) != 0)
		L = maskLayer;
	else if (mapLayer->getTile(row, column) != 0)
		L = mapLayer;
	else // No layer found
		return;

	Tile *t = L->getTile(row, column);

	// Apply mirror
	if (!t->mirror)
		t->mirror = true;
	else
		t->mirror = false;

	updateBoundingBox(t);

	if (t->collidable)
		collisionLayer->addVerticies(row, column, float(column * TILE_SIZE + t->bBX), float(row * TILE_SIZE + t->bBY), t->width, t->height);
}

void Editable_Map::setTransitionPoint(int row, int column, sf::RenderWindow *window)
{
	//If a transition point already exists, exit the function
	if (mapLayer->getTile(row, column)->tileType == 'E')
		return;

	// LOCAL VARIABLES
	std::string mapFileName = UI::getMap("JRM");
	if (mapFileName.compare(".jrm") < 0) // If the user closes the file chooser, exit the function
		return;

	window->setActive(false);
	Map_Editor transitionPointerSelector("Select Transition Point", mapFileName);

	sf::Vector2i transitionToCoords = transitionPointerSelector.runForResult();
	sf::Vector2i transitionPoint = sf::Vector2i(column, row);

	window->setActive(true);

	// If the user closes the transition coordinates window, exit the function
	if (transitionToCoords == sf::Vector2i(-1, -1))
		return;

	mapFileName = std::strstr(mapFileName.c_str(), "res\\Maps\\");
	mapFileName[3] = '/';
	mapFileName[8] = '/';

	// Set-up tile to be a transition tile
	mapLayer->addTransitionPoint(row, column, mapFileName, sf::Vector2f(float(transitionToCoords.x * TILE_SIZE), float(transitionToCoords.y * TILE_SIZE)));
	transitionLayer->addVerticies(row, column, float(column * TILE_SIZE), float(row * TILE_SIZE), TILE_SIZE, TILE_SIZE);

	for (unsigned int i = 0; i < transitions.size(); i++)
	{
		if (transitions[i].startingCoords == transitionToCoords && transitions[i].transitionMapName.compare(mapFileName) == 0)
		{
			transitions[i].numCoords++;
			transitions[i].transitionPoints.push_back(transitionPoint);
			return;
		}
	}

	numTransitionPoints++;

	TransitionPoint tp;
	tp.transitionMapName = mapFileName;
	tp.startingCoords = transitionToCoords;
	tp.transitionPoints.push_back(transitionPoint);
	tp.numCoords++;

	transitions.push_back(tp);
}

void Editable_Map::testMap(unsigned int row, unsigned int column, const pb::Time& t)
{
	if (!mapLoaded)
		return;

	saveMap(); // Save map before running it

	sf::Vector2i testPos(column, row);

	std::string mapName = getMapName();

	std::string cmd = "ProjectJR.exe " + mapName + " " + std::to_string(testPos.y) + " " + std::to_string(testPos.x) + " " + std::to_string(t.hours) + " " + std::to_string(t.minutes);
	LPSTR cmdArgs = const_cast<char *>(cmd.c_str());

	PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter

	STARTUPINFO StartupInfo; //This is an [in] parameter
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof StartupInfo; //Only compulsory field

	CreateProcess("ProjectJR.exe", cmdArgs, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo);
}

unsigned short Editable_Map::getTileSize()
{
	return TILE_SIZE;
}

void Editable_Map::saveMap()
{
	//LOCAL VARIABLES
	std::ofstream mapFile;
	bool hasTile = false;

	mapFile.open(nameOfFile);

	mapFile << nameOfSheetFile << std::endl;
	mapFile << nameOfTileSheet << std::endl;
	mapFile << numRows << "x" << numColumns << std::endl;
	mapFile << numTransitionPoints << std::endl;

	// Add transitions to map file
	for (int i = 0; i < numTransitionPoints; i++)
	{
		mapFile << transitions[i].transitionMapName << "-" << transitions[i].startingCoords.y << "x" << transitions[i].startingCoords.x << "-" << transitions[i].numCoords << "-";

		for (int j = 0; j < transitions[i].numCoords; j++)
			mapFile << transitions[i].transitionPoints[j].y << "x" << transitions[i].transitionPoints[j].x << ",";

		mapFile << std::endl;
	}

	for (unsigned int row = 0; row < numRows; row++)
	{
		for (unsigned int column = 0; column < numColumns; column++)
		{
			if (mapLayer->getTile(row, column) != 0)
			{
				mapFile << mapLayer->getTile(row, column)->toString();
				hasTile = true;
			}
			if (maskLayer->getTile(row, column) != 0)
			{
				if (hasTile)
				{
					mapFile << ",";
					hasTile = false;
				}

				mapFile << maskLayer->getTile(row, column)->toString();
				hasTile = true;
			}
			if (groundLayers[row]->getTile(row, column) != 0)
			{
				if (hasTile)
				{
					mapFile << ",";
					hasTile = false;
				}

				mapFile << groundLayers[row]->getTile(row, column)->toString();
				hasTile = true;
			}
			if (canopyLayer->getTile(row, column) != 0)
			{
				if (hasTile)
				{
					mapFile << ",";
					hasTile = false;
				}

				mapFile << canopyLayer->getTile(row, column)->toString();
				hasTile = true;
			}

			mapFile << "_";
		}

		mapFile << std::endl;
	}

	mapFile.close();
}


unsigned int Editable_Map::getRows()
{
	return numRows;
}

unsigned int Editable_Map::getColumns()
{
	return numColumns;
}

std::string Editable_Map::getMapName()
{
	return nameOfFile;
}
