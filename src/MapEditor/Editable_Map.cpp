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

	// Set tile sizes
	deleteTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	deleteTransTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	transitionTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	rotationTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	mirrorTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));

	// load tile textures
	deleteTile.setTexture(Map_Editor::tilePaneManager->addTexture("Delete.png"));
	deleteTransTile.setTexture(Map_Editor::tilePaneManager->addTexture("DeleteTrans.png"));
	transitionTile.setTexture(Map_Editor::tilePaneManager->addTexture("Transition.png"));
	rotationTile.setTexture(Map_Editor::tilePaneManager->addTexture("Rotate.png"));
	mirrorTile.setTexture(Map_Editor::tilePaneManager->addTexture("Mirror.png"));

	// Load font, and set text properties
	pb::System::load(&font, "arial.ttf");
	currentRowColumn.setFont(font);
	currentRowColumn.setCharacterSize(12);
	currentRowColumn.setFillColor(sf::Color::Black);
}

void Editable_Map::initializeTileSheetCoords(sf::RenderWindow* window)
{
	tileSheetCoords.x = window->getSize().x - 352;
	tileSheetCoords.y = 64;

	tileSheetSprite.setPosition(tileSheetCoords.x, tileSheetCoords.y);

	currentRowColumn.setPosition(window->getSize().x - (TILE_SIZE * 8) - (TILE_SIZE * 3), TILE_SIZE);

	mirrorTile.setPosition(window->getSize().x - (TILE_SIZE * 6) - (TILE_SIZE * 2), TILE_SIZE);
	rotationTile.setPosition(window->getSize().x - (TILE_SIZE * 5) - (TILE_SIZE * 1.5), TILE_SIZE);
	transitionTile.setPosition(window->getSize().x - (TILE_SIZE * 4) - TILE_SIZE, TILE_SIZE);
	deleteTransTile.setPosition(window->getSize().x - (TILE_SIZE * 3) - (TILE_SIZE / 2), TILE_SIZE);
	deleteTile.setPosition(window->getSize().x - (TILE_SIZE * 2), TILE_SIZE);

}

void Editable_Map::loadMap(std::string mapName)
{
	if (mapName.compare(".jrm") < 0)
		return;

	//LOCAL VARIABLES
	std::ifstream mapFile;

	//Reset all tile manipulations and debug layers
	allowTileManipulation();
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
		initialize(mapFile); //Dynamically create an array to hold the map	
	else
	{
		printf("\"%s\" could not be found!\n", mapName.c_str());
		system("pause");
		exit(EXIT_FAILURE); //Close the application with a failure code if the file does not open
	}

	mapFile.close(); //Close the file

	mapLoaded = true;
}

void Editable_Map::createMap(unsigned int rows, unsigned int columns, std::string mapName, std::string sheetFileName)
{
	std::ifstream tileFile;
	std::string input;

	//Clear out the transitions if they existed in a previously loaded map.
	if (numTransitionPoints != 0)
		transitions.clear();

	//Reset all tile manipulations and debug layers
	allowTileManipulation();
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
	tileFile.open(nameOfSheetFile);

	if (!tileFile.is_open())
	{
		printf("\"%s\" could not be found!\n", nameOfSheetFile.c_str());
		system("pause");
		exit(EXIT_FAILURE); //Close the application with a failure code if the file does not open
	}

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
	tiles.setTexture(*Map_Editor::graphicManager->addTexture(nameOfTileSheet));
	tileSheetSprite.setTexture(*Map_Editor::graphicManager->addTexture(nameOfTileSheet));;

	waterAnimator = new Water(4, 0, 3, TILE_SIZE, TILE_SIZE, 0.3f);

	//Recreate the mapTexture, canopyTexture and groundTexture ONLY when needed
	if (mapTexture.getSize().x <= numColumns * TILE_SIZE && mapTexture.getSize().y <= numRows * TILE_SIZE)
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
		transitionTexture.create(numColumns * TILE_SIZE, numRows * TILE_SIZE);

		gridTexture.setSmooth(true);
	}

	createGrid();

	Map_Editor::camera->setBounds((numColumns + 4) * TILE_SIZE, numRows * TILE_SIZE);

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

	groundSprites = new sf::Sprite[numRows];

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numColumns; j++)
			addTileToMap(map, tileData[0], 2, i, j);
	}

	drawMap();

	mapLoaded = true;
}

void Editable_Map::createGrid()
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

void Editable_Map::initialize(std::ifstream& mapFile)
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

	tiles.setTexture(*Map_Editor::graphicManager->addTexture(nameOfTileSheet));
	tileSheetSprite.setTexture(*Map_Editor::graphicManager->addTexture(nameOfTileSheet));;

	//Get the total number of rows and columns from the file and store them
	std::getline(mapFile, mapRows, 'x');
	std::getline(mapFile, mapColumns);

	//Convert the parsed strings to ints
	numRows = atoi(mapRows.c_str());
	numColumns = atoi(mapColumns.c_str());

	waterAnimator = new Water(4, 0, 3, TILE_SIZE, TILE_SIZE, 0.3f);

	//Recreate the mapTexture, canopyTexture and groundTexture ONLY when needed
	if (mapTexture.getSize().x <= numColumns * TILE_SIZE && mapTexture.getSize().y <= numRows * TILE_SIZE)
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
		transitionTexture.create(numColumns * TILE_SIZE, numRows * TILE_SIZE);

		gridTexture.setSmooth(true);
	}

	createGrid();

	Map_Editor::camera->setBounds((numColumns + 4) * TILE_SIZE, numRows * TILE_SIZE);

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

	groundSprites = new sf::Sprite[numRows];

	initializeTransitionPoints(mapFile); //Intialize all transition points in the map

	populateMap(mapFile); //Fill the array with the map data

	drawMap(); //Draw the map
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
		coords.y = atoi(input.c_str()) * TILE_SIZE;
		std::getline(mapFile, input, '-');
		coords.x = atoi(input.c_str()) * TILE_SIZE;

		p.startingCoords = sf::Vector2i((int)(coords.x / TILE_SIZE), (int)(coords.y / TILE_SIZE));

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

			//Store the file name and coordinates for the spawning position in the next map
			map[tp.y][tp.x].mapName = p.transitionMapName;
			map[tp.y][tp.x].transitionCoords = coords;

			p.transitionPoints.push_back(tp);
		}

		transitions.push_back(p);
		std::getline(mapFile, input);
	}
}

void Editable_Map::updateDrawList(sf::RenderWindow* window, sf::Vector2f mouseCoords)
{
	waterAnimator->updateWaterCycle(&waterAniClock);
	waterAnimator->updateAnimation(&waterSprite, waterFrames); // Update the water sprite
	
	Map_Editor::graphicManager->addToDrawList(&waterSprite, false);
	Map_Editor::graphicManager->addToDrawList(&mapSprite, false);
	Map_Editor::graphicManager->addToDrawList(&maskSprite, false);
	for (int i = 0; i < numRows; i++)
		Map_Editor::graphicManager->addToDrawList(&groundSprites[i], true);
	Map_Editor::graphicManager->addToDrawList(&canopySprite, true); //Draw the canopy

	//TOOLS
	if (renderCollisionLayer)
		Map_Editor::graphicManager->addToDrawList(&collisionSprite, false);
	if (renderGridLayer)
		Map_Editor::graphicManager->addToDrawList(&gridSprite, false);
	if (renderTransitionLayer)
		Map_Editor::graphicManager->addToDrawList(&transitionSprite, false);

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		mousePos.setPosition(sf::Vector2f(floor(mouseCoords.x / TILE_SIZE) * TILE_SIZE,
			floor(mouseCoords.y / TILE_SIZE) * TILE_SIZE));

		if (mousePos.getPosition().x <= TILE_SIZE * (numColumns - 1) && mousePos.getPosition().y <= TILE_SIZE * (numRows - 1) && mousePos.getPosition().x >= 0 && mousePos.getPosition().y >= 0)
			Map_Editor::graphicManager->addToDrawList(&mousePos, false);
		else
			window->setMouseCursorVisible(true);
	}

	drawTileSheet(window, mouseCoords);
}


void Editable_Map::drawTileSheet(sf::RenderWindow *window, sf::Vector2f mousePos)
{
	Map_Editor::tilePaneManager->addToDrawList(&tileSheetSprite, false);
	Map_Editor::tilePaneManager->addToDrawList(&deleteTile, false);
	Map_Editor::tilePaneManager->addToDrawList(&deleteTransTile, false);
	Map_Editor::tilePaneManager->addToDrawList(&transitionTile, false);
	Map_Editor::tilePaneManager->addToDrawList(&rotationTile, false);
	Map_Editor::tilePaneManager->addToDrawList(&mirrorTile, false);

	if (mousePos.x <= numColumns * TILE_SIZE && sf::Mouse::getPosition(*window).x <= tileSheetCoords.x && mousePos.x >= 0 &&
		sf::Mouse::getPosition(*window).y >= TILE_SIZE && mousePos.y <= numRows * TILE_SIZE && mousePos.y >= 0)
	{
		currentRowColumn.setString(std::to_string((int)(mousePos.y / TILE_SIZE)) + ", " + std::to_string((int)(mousePos.x / TILE_SIZE)));
		Map_Editor::tilePaneManager->addToDrawList(&currentRowColumn, false);
	}

	if (currentTile.compare("No Tile") != 0)
		Map_Editor::tilePaneManager->addToDrawList(&selectedTile, false);
}

bool Editable_Map::isMapLoaded()
{
	return mapLoaded;
}

void Editable_Map::setTile(sf::Vector2i mouseCoords)
{
	if (mouseCoords.x > deleteTile.getPosition().x && mouseCoords.x < deleteTile.getPosition().x + TILE_SIZE &&
		mouseCoords.y > deleteTile.getPosition().y && mouseCoords.y < deleteTile.getPosition().y + TILE_SIZE)
	{
		currentTile = "Delete";
		selectedTile.setPosition(sf::Vector2f(deleteTile.getPosition().x, deleteTile.getPosition().y));
		renderTransitionLayer = false; //Turn off the transition layer
		return;
	}
	else if (mouseCoords.x > deleteTransTile.getPosition().x && mouseCoords.x < deleteTransTile.getPosition().x + TILE_SIZE &&
		mouseCoords.y > deleteTransTile.getPosition().y && mouseCoords.y < deleteTransTile.getPosition().y + TILE_SIZE)
	{
		currentTile = "DeleteTransition";
		selectedTile.setPosition(sf::Vector2f(deleteTransTile.getPosition().x, deleteTransTile.getPosition().y));
		renderTransitionLayer = true;
		return;
	}
	else if (mouseCoords.x > rotationTile.getPosition().x && mouseCoords.x < rotationTile.getPosition().x + TILE_SIZE &&
		mouseCoords.y > rotationTile.getPosition().y && mouseCoords.y < rotationTile.getPosition().y + TILE_SIZE)
	{
		currentTile = "Rotate";
		selectedTile.setPosition(sf::Vector2f(rotationTile.getPosition().x, rotationTile.getPosition().y));
		renderTransitionLayer = false; //Turn off the transition layer
		return;
	}
	else if (mouseCoords.x > mirrorTile.getPosition().x && mouseCoords.x < mirrorTile.getPosition().x + TILE_SIZE &&
		mouseCoords.y > mirrorTile.getPosition().y && mouseCoords.y < mirrorTile.getPosition().y + TILE_SIZE)
	{
		currentTile = "Mirror";
		selectedTile.setPosition(sf::Vector2f(mirrorTile.getPosition().x, mirrorTile.getPosition().y));
		renderTransitionLayer = false; //Turn off the transition layer
		return;
	}
	else if (mouseCoords.x > transitionTile.getPosition().x && mouseCoords.x < transitionTile.getPosition().x + TILE_SIZE &&
		mouseCoords.y > transitionTile.getPosition().y && mouseCoords.y < transitionTile.getPosition().y + TILE_SIZE)
	{
		currentTile = "Transition";
		selectedTile.setPosition(sf::Vector2f(transitionTile.getPosition().x, transitionTile.getPosition().y));
		renderTransitionLayer = true;
		return;
	}

	if (mouseCoords.x < tileSheetCoords.x || mouseCoords.y < tileSheetCoords.y)
		return;

	renderTransitionLayer = false; //Turn off the transition layer

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

void Editable_Map::addTileToPos()
{
	if (currentTile.compare("No Tile") == 0)
		return;

	//LOCAL VARIABLES
	int column = mousePos.getPosition().x / TILE_SIZE;
	int row = mousePos.getPosition().y / TILE_SIZE;

	if (column > numColumns - 1 || row > numRows - 1 || column < 0 || row < 0)
		return;

	if (currentTile.compare("Delete") == 0)
		deleteTileFromPos(row, column);
	else if (currentTile.compare("DeleteTransition") == 0)
		deleteTransitionPoint(row, column);
	else if (currentTile.compare("Rotate") == 0)
		rotateTile(row, column);
	else if (currentTile.compare("Mirror") == 0)
		mirrorTileAtPos(row, column);
	else if (currentTile.compare("Transition") == 0)
		setTransitionPoint(row, column);
	else if (currentTile[0] == '0' && !isSameTile(map, row, column))
	{
		addTileToMap(map, currentTile, 2, row, column);
		drawToTexture(mapTexture, map, row, column);
		mapTexture.display();
	}
	else if (currentTile[0] == '1')
	{
		addTileToMap(mask, currentTile, 2, row, column);
		
		if (currentTile[6] == '1' && map[row][column].collidable)
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

void Editable_Map::deleteTileFromPos(int row, int column)
{
	if (canopy[row][column].hasTile)
	{
		canopy[row][column].hasTile = false;
		updateMap(canopyTexture, canopy);
	}
	else if (ground[row][column].hasTile)
	{
		ground[row][column].hasTile = false;
		updateMap(groundTexture, ground);
	}
	else if (mask[row][column].hasTile)
	{
		mask[row][column].hasTile = false;

		if (mask[row][column].collidable && map[row][column].tileType == 'W')
			map[row][column].collidable = true;

		updateMap(maskTexture, mask);
	}

	tileDeletedRecently = true;
}

void Editable_Map::deleteTransitionPoint(int row, int column)
{
	if (transitionRemovedRecently || map[row][column].mapName.compare("") == 0)
		return;

	//LOCAL VARIABLES
	int i = 0;
	bool tileRemoved = false;
	sf::Vector2i transitionCoordsAtPos;

	transitionCoordsAtPos.x = map[row][column].transitionCoords.x / TILE_SIZE;
	transitionCoordsAtPos.y = map[row][column].transitionCoords.y / TILE_SIZE;

	transitionRemovedRecently = true;

	while (i < transitions.size() && !tileRemoved)
	{
		if (transitions[i].transitionMapName.compare(map[row][column].mapName) == 0 && transitions[i].startingCoords == transitionCoordsAtPos)
		{
			//ADDITIONAL LOCAL VARIABLES
			int j = 0;
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

			map[row][column].tileType = tileData[map[row][column].row * 10 + map[row][column].column][7];

			tileRemoved = true;
		}

		i++;
	}
}

void Editable_Map::rotateTile(int row, int column)
{
	if (tileRotatedRecently)
		return;

	//Rotations are as follows: 1 = 0 degrees, 1 = 90 degrees, 2 = 180 degrees, 3 = 270 degrees
	if (canopy[row][column].hasTile)
	{
		canopy[row][column].rotation += 1;
		if (canopy[row][column].rotation > 3)
			canopy[row][column].rotation = 0;

		addTileToMap(canopy, canopy[row][column].toString(), 0, row, column);
		updateMap(canopyTexture, canopy);
	}
	else if (ground[row][column].hasTile)
	{
		ground[row][column].rotation += 1;
		if (ground[row][column].rotation > 3)
			ground[row][column].rotation = 0;

		addTileToMap(ground, ground[row][column].toString(), 0, row, column);
		updateMap(groundTexture, ground);
	}
	else if (mask[row][column].hasTile)
	{
		mask[row][column].rotation += 1;
		if (mask[row][column].rotation > 3)
			mask[row][column].rotation = 0;

		addTileToMap(mask, mask[row][column].toString(), 0, row, column);
		updateMap(maskTexture, mask);
	}
	else if (map[row][column].hasTile)
	{
		map[row][column].rotation += 1;
		if (map[row][column].rotation > 3)
			map[row][column].rotation = 0;

		addTileToMap(map, map[row][column].toString(), 0, row, column);
		drawToTexture(mapTexture, map, row, column);
		mapTexture.display();
	}

	tileRotatedRecently = true;
}

void Editable_Map::mirrorTileAtPos(int row, int column)
{
	if (tileMirroredRecently)
		return;

	if (canopy[row][column].hasTile)
	{
		if (!canopy[row][column].mirror)
			canopy[row][column].mirror = true;
		else
			canopy[row][column].mirror = false;

		addTileToMap(canopy, canopy[row][column].toString(), 0, row, column);
		updateMap(canopyTexture, canopy);
	}
	else if (ground[row][column].hasTile)
	{
		if (!ground[row][column].mirror)
			ground[row][column].mirror = true;
		else
			ground[row][column].mirror = false;

		addTileToMap(ground, ground[row][column].toString(), 0, row, column);
		updateMap(groundTexture, ground);
	}
	else if (mask[row][column].hasTile)
	{
		if (!mask[row][column].mirror)
			mask[row][column].mirror = true;
		else
			mask[row][column].mirror = false;

		addTileToMap(mask, mask[row][column].toString(), 0, row, column);
		updateMap(maskTexture, mask);
	}
	else if (map[row][column].hasTile)
	{
		if (!map[row][column].mirror)
			map[row][column].mirror = true;
		else
			map[row][column].mirror = false;

		addTileToMap(map, map[row][column].toString(), 0, row, column);
		drawToTexture(mapTexture, map, row, column);
		mapTexture.display();
	}

	tileMirroredRecently = true;
}

void Editable_Map::setTransitionPoint(int row, int column)
{
	//If a transition point already exists, exit the function
	if (map[row][column].tileType == 'E')
		return;

	//LOCAL VARIABLES
	std::string mapFileName = UI::getMap("JRM");
	if (mapFileName.compare(".jrm") < 0) // If the user closes the file chooser, exit the function
		return;

	sf::Vector2i transitionToCoords = UI::getCoordinates("Transition Map Coordinates");
	sf::Vector2i transitionPoint = sf::Vector2i(column, row);

	//If the user closes the transition coordinates window, exit the function
	if (transitionToCoords == sf::Vector2i(-1, -1))
		return;

	mapFileName = std::strstr(mapFileName.c_str(), "res\\Maps\\");
	mapFileName[3] = '/';
	mapFileName[8] = '/';

	//Set-up tile to be a transition tile
	map[row][column].tileType = 'E';
	map[row][column].transitionCoords = (sf::Vector2f)transitionToCoords;
	map[row][column].transitionCoords.x = map[row][column].transitionCoords.x * TILE_SIZE;
	map[row][column].transitionCoords.y = map[row][column].transitionCoords.y * TILE_SIZE;
	map[row][column].mapName = mapFileName;

	sf::RectangleShape r;
	r.setPosition(column * TILE_SIZE, row * TILE_SIZE);
	r.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
	r.setFillColor(sf::Color(0, 255, 0, 255));

	transitionTexture.draw(r);

	transitionPlacedRecently = true;

	for (int i = 0; i < transitions.size(); i++)
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

void Editable_Map::allowTileManipulation()
{
	tileRotatedRecently = false;
	tileMirroredRecently = false;
	tileDeletedRecently = false;
	transitionPlacedRecently = false;
	transitionRemovedRecently = false;
}

bool Editable_Map::isSameTile(Tile**& layer, int row, int column)
{
	Tile t;
	t.row = currentTile[2] - '0';
	t.column = currentTile[3] - '0';
	t.rotation = currentTile[4] - '0';
	t.collidable = currentTile[5] - '0'; //0 = false, 1 = true
	t.tileType = currentTile[6];

	if (layer[row][column].row == t.row &&
		layer[row][column].column == t.column &&
		layer[row][column].rotation == t.rotation &&
		layer[row][column].collidable == t.collidable &&
		layer[row][column].tileType == t.tileType)
		return true;

	return false;
}

void Editable_Map::updateMap(sf::RenderTexture& texture, Tile**& layer)
{
	texture.clear(sf::Color(0, 0, 0, 0));

	//Step through each row in the maps array.
	for (int i = 0; i <= numRows - 1; i++)
	{
		//Step through each column in the maps row and add a tile where needed
		for (int j = 0; j <= numColumns - 1; j++)
		{
			if (layer[i][j].hasTile)
				drawToTexture(texture, layer, i, j);
		}
	}

	texture.display();
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

	for (int i = 0; i < numTransitionPoints; i++)
	{
		mapFile << transitions[i].transitionMapName << "-" << transitions[i].startingCoords.y << "x" << transitions[i].startingCoords.x << "-" << transitions[i].numCoords << "-";

		for (int j = 0; j < transitions[i].numCoords; j++)
			mapFile << transitions[i].transitionPoints[j].y << "x" << transitions[i].transitionPoints[j].x << ",";

		mapFile << std::endl;
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

		mapFile << std::endl;
	}

	mapFile.close();
}

std::string Editable_Map::tileToString(Tile**& layer, int row, int column)
{
	std::string s = std::to_string(layer[row][column].row) + std::to_string(layer[row][column].column) + std::to_string(layer[row][column].rotation) + std::to_string(boolToString(layer[row][column].mirror));

	s = s + std::to_string(boolToString(layer[row][column].collidable)) + layer[row][column].tileType;

	if (layer[row][column].boundingBox.compare("none") != 0 && layer[row][column].collidable)
		s = s + ":" + layer[row][column].boundingBox;

	return s;
}

int Editable_Map::boolToString(bool b)
{
	if (b == true)
		return 1;
	else
		return 0;
}

int Editable_Map::getRows()
{
	return numRows;
}

int Editable_Map::getColumns()
{
	return numColumns;
}

std::string Editable_Map::getMapName()
{
	return nameOfFile;
}

void Editable_Map::forceUpdate()
{
	drawMap();
}