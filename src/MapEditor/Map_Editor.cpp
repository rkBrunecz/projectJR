/*
MapEditor.cpp
This file is the driving force behind the map editor. It directs user input in various ways and uses a state based system to determine how it will process requests.

@author Randall Brunecz
@version 1.0 10/12/2015
*/

// Headers
//#include "vld.h" // Check for memory leaks. Comment out if not being used
#include "MapEditor/Map_Editor.h"

// Global variables
pb::Camera *Map_Editor::camera;
pb::Graphic_Manager *Map_Editor::graphicManager, *Map_Editor::tilePaneManager;

Map_Editor::Map_Editor(const std::string versionNum)
{
	//Create a fullscreen window with same pixel depth (a.k.a bit depth/color depth) as the desktop
	desktop = sf::VideoMode::getDesktopMode();
	window = new sf::RenderWindow(sf::VideoMode(desktop.width - (desktop.width * 0.1), desktop.height - (desktop.height * 0.25), desktop.bitsPerPixel), "Map Editor " + versionNum, sf::Style::Default);
	window->setVerticalSyncEnabled(true);

	//Set up camera properties
	camera = new pb::Camera(desktop.width - 384, desktop.height - (desktop.height * 0.25), zoomFactor);
	camera->setCenter(desktop.width / 2, desktop.height / 2);
	camera->setViewport(sf::FloatRect(0, 0.03f, 0.85f, 0.97f));

	gameClock = new pb::In_Game_Clock(2, 8, 0, 24, 8, 8, 4, 4);

	graphicManager = new pb::Graphic_Manager(*gameClock);
	tilePaneManager = new pb::Graphic_Manager();

	tilePaneManager->updateBufferSize(sf::Vector2i(1920, 1080));

	//Set up a menu bar
	menuBar.setPosition(0, 0);
	menuBar.setSize(sf::Vector2f(desktop.width - (desktop.width * 0.1), (desktop.height - (desktop.height * 0.25)) * 0.03));
	menuBar.setFillColor(sf::Color(238, 233, 233, 255));
	menuBar.setOutlineColor(sf::Color(0, 0, 0, 255));
	menuBar.setOutlineThickness(1);

	//Set up a tile pane
	tilePane.setPosition(window->getSize().x - 384, menuBar.getSize().y);
	tilePane.setSize(sf::Vector2f(384, window->getSize().y - menuBar.getSize().y));
	tilePane.setFillColor(sf::Color(238, 233, 233, 255));
	tilePane.setOutlineColor(sf::Color(0, 0, 0, 255));
	tilePane.setOutlineThickness(1);

	UI::intializeMainWindow(window);
}

Map_Editor::~Map_Editor()
{
	delete graphicManager;
	delete tilePaneManager;
	delete camera;
	delete window;

	delete map;
	delete gameClock;
}

bool Map_Editor::displayCursor()
{
	sf::Vector2i mouseCoords = sf::Mouse::getPosition(*window);

	return (mouseCoords.x <= window->getSize().x - 384 && mouseCoords.y >= window->getSize().y * window->getView().getViewport().top) ? false : true;
}

void Map_Editor::moveCamera()
{
	if (camera->getSize().x >= map->getMapSize().x && camera->getSize().y >= map->getMapSize().y)
		return;

	sf::Vector2f pos(0,0);

	if (camera->getSize().x <= map->getMapSize().x && camera->getSize().y <= map->getMapSize().y)
		pos = 0.1f * sf::Vector2f(sf::Mouse::getPosition(*window) - rightButtonPos) + camera->getCenter();
	else if (camera->getSize().x >= map->getMapSize().x)
		pos = 0.1f * sf::Vector2f(0, (sf::Mouse::getPosition(*window) - rightButtonPos).y) + sf::Vector2f(0, camera->getCenter().y);
	else if (camera->getSize().y >= map->getMapSize().y)
		pos = 0.1f * sf::Vector2f((sf::Mouse::getPosition(*window) - rightButtonPos).x, 0) + sf::Vector2f(camera->getCenter().x, 0);

	camera->updatePosition(pos);
}

void Map_Editor::processEvents()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();

			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				window->close();
			else if (event.key.code == sf::Keyboard::Tab)
				map->displayCollsionLayer();
			else if (event.key.code == sf::Keyboard::G)
				map->displayGridLayer();
			else if (event.key.code == sf::Keyboard::T)
				map->displayTransitionLayer();
			else if (event.key.code == sf::Keyboard::L)
				state = Load;
			else if (event.key.code == sf::Keyboard::N)
				state = New;
			else if (event.key.code == sf::Keyboard::S)
				state = Save;
			else if (event.key.code == sf::Keyboard::U)
				state = ForceUpdate;
			else if (event.key.code == sf::Keyboard::R)
				state = TestMap;
			
			break;

		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Right)
				rightButtonPos = sf::Mouse::getPosition(*window);
			
			break;

		case sf::Event::MouseWheelScrolled:
			if (zoomFactor - (0.05 *event.mouseWheelScroll.delta) > 0.2 && zoomFactor - (0.05 *event.mouseWheelScroll.delta) < 1)
			{
				camera->zoom(1 - 0.05f * event.mouseWheelScroll.delta);
				zoomFactor -= (0.05 * event.mouseWheelScroll.delta);
			}
			
			break;
		}
	}
}

/*
update

This method is where all editor related work is done.
*/
void Map_Editor::update()
{
	switch (state)
	{
	case Build:
		if (map != 0 && map->isMapLoaded())
		{
			window->setView(*camera);

			gameClock->updateClock();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && sf::Mouse::getPosition().x < window->getSize().x - 382 && sf::Mouse::getPosition().y > 32)
				moveCamera();
			else
				window->setMouseCursorVisible(displayCursor());

			sf::Vector2f mouse_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
			tilePaneManager->addToDrawList(&tilePane, false);
			map->updateDrawList(window, mouse_pos);

			sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mousePos.x < window->getSize().x - tilePane.getSize().x && mousePos.y > menuBar.getSize().y)
				map->addTileToPos();
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				map->setTile(mousePos);
			
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
				map->allowTileManipulation();
		}

		break;
	case Load:
	{
		state = Build;

		std::string loadedMapName = UI::getMap("JRM");
		// If no map file was chosen, break
		if (loadedMapName.compare(".jrm") < 0)
			break;

		// Clean up memory
		graphicManager->clearTextureList();
		tilePaneManager->clearTextureList();
		delete map;
		map = 0;

		// Create a new map
		map = new Editable_Map();
		map->initializeTileSheetCoords(window);

		map->loadMap(loadedMapName);
		camera->setCenter(window->getSize().x / 2, window->getSize().y / 2);
		graphicManager->updateBufferSize(map->getMapSize());

		sf::Vector2i moveCamera = sf::Vector2i(0, 0);
		if (map->getColumns() * Editable_Map::getTileSize() < window->getSize().x)
			moveCamera.x = (window->getSize().x / 2) - tilePane.getSize().x;
		if (map->getRows() * Editable_Map::getTileSize() < window->getSize().y)
			moveCamera.y = (window->getSize().y / 2) - ((map->getColumns() * Editable_Map::getTileSize()) / 2) - menuBar.getSize().y;

		camera->move(-moveCamera.x, -moveCamera.y);

		break;
	}
	case Save:
		state = Build;

		if (map->isMapLoaded())
			map->saveMap();

		break;

	case ForceUpdate:
		state = Build;

		if (map->isMapLoaded())
			map->forceUpdate();

		break;

	case New:
	{
		state = Build;

		std::string fileName;
		sf::Vector2i v = UI::getNewMapParams(&fileName);
		// If the user closes the map parameters window, break
		if (v == sf::Vector2i(-1, -1))
			break;

		std::string sheetFileName = UI::getMap("JRS");
		// If the user closes the file chooser, exit the function
		if (sheetFileName.compare(".jrs") < 0) 
			break;

		// Clean up memory
		graphicManager->clearTextureList();
		tilePaneManager->clearTextureList();
		delete map;
		map = 0;

		// Create a new map
		map = new Editable_Map();
		map->initializeTileSheetCoords(window);

		sheetFileName = std::strstr(sheetFileName.c_str(), "res\\Maps\\jrs\\");
		fileName = "res/Maps/" + fileName + ".jrm";

		if (v != sf::Vector2i(-1, -1))
			map->createMap(v.y, v.x, fileName, sheetFileName);
		else
			break;

		camera->setCenter(window->getSize().x / 2, window->getSize().y / 2);
		graphicManager->updateBufferSize(map->getMapSize());

		sf::Vector2i moveCamera = sf::Vector2i(0, 0);
		if (v.x * Editable_Map::getTileSize() < window->getSize().x)
			moveCamera.x = (window->getSize().x / 2) - tilePane.getSize().x;
		if (v.y * Editable_Map::getTileSize() < window->getSize().y)
			moveCamera.y = (window->getSize().y / 2) - ((v.y * Editable_Map::getTileSize()) / 2) - menuBar.getSize().y;

		camera->move(-moveCamera.x, -moveCamera.y);

		break;
	}
	case TestMap:
	{
		state = Build;

		if (!map->isMapLoaded())
			return;

		sf::Vector2i getTestPos = UI::getCoordinates("Starting Coordinates");
		if (getTestPos == sf::Vector2i(-1, -1))
			return;

		std::string mapName = map->getMapName();

		std::string cmd = "ProjectJR.exe " + mapName + " " + std::to_string(getTestPos.y) + " " + std::to_string(getTestPos.x);
		LPSTR cmdArgs = const_cast<char *>(cmd.c_str());

		PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter

		STARTUPINFO StartupInfo; //This is an [in] parameter
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
		StartupInfo.cb = sizeof StartupInfo; //Only compulsory field

		CreateProcess("ProjectJR.exe", cmdArgs, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo);

		break;
	}
	}

	tilePaneManager->addToDrawList(&menuBar, false);
}

void Map_Editor::render()
{
	//Clear the window
	window->clear(sf::Color(255, 250, 250, 255));

	// Draw the map
	window->setView(*camera);
	graphicManager->draw(window, gameClock->getTime());

	// Draw the tile pane
	window->setView(window->getDefaultView());
	tilePaneManager->draw(window);

	window->display();
}

void Map_Editor::runEditor()
{
	while (window->isOpen())
	{
		window->setView(*camera);
		processEvents();

		update();

		render();
	}
}

int main()
{
	// Local variables
	std::string versionNum = "v.1.0a";
	Map_Editor editor(versionNum);

	editor.runEditor();

	return 0; //Close the game
}