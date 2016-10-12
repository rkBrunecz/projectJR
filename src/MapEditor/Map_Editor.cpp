/*
MapEditor.cpp
This file is the driving force behind the map editor. It directs user input in various ways and uses a state based system to determine how it will process requests.

@author Randall Brunecz
@version 1.0 10/12/2015
*/

// Headers
#include "MapEditor/Map_Editor.h"

Map_Editor::Map_Editor(const std::string& versionNum)
{
	desktop = sf::VideoMode::getDesktopMode();
	window = new sf::RenderWindow(sf::VideoMode(unsigned int(desktop.width - (desktop.width * 0.1f)), unsigned int(desktop.height - (desktop.height * 0.25f)), desktop.bitsPerPixel), "Map Editor " + versionNum, sf::Style::Default);
	window->setVerticalSyncEnabled(true);

	//Set up camera properties
	camera = new pb::Camera(int(window->getSize().x - (window->getSize().x * 0.20f)), int(desktop.height - (desktop.height * 0.25f)), zoomFactor);
	camera->setCenter(float(desktop.width / 2), float(desktop.height / 2));
	camera->setViewport(sf::FloatRect(0, 0.03f, (window->getSize().x - (window->getSize().x * 0.20f)) / window->getSize().x, 0.97f));

	gameClock = new pb::In_Game_Clock(2, 8, 0, 24, 8, 8, 4, 4);

	graphicManager = new pb::Graphic_Manager(*gameClock);
	tilePaneManager = new pb::Graphic_Manager();

	// Set up menu menuBar
	menuBar = new Menu_Bar(sf::Vector2f(desktop.width - (desktop.width * 0.1f), (desktop.height - (desktop.height * 0.25f)) * 0.03f), sf::Vector2f(0, 0), sf::Color(238, 233, 233, 255), sf::Color(0, 0, 0, 255), 1);
	menuBar->addDropDownItem("File", "New");
	menuBar->addDropDownItem("File", "Save");
	menuBar->addDropDownItem("File", "Load");
	menuBar->addDropDownItem("View", "Display Grid");
	menuBar->addDropDownItem("View", "Display Collisions");
	menuBar->addDropDownItem("View", "Display Transitions");
	menuBar->addDropDownItem("Time", "1x");
	menuBar->addDropDownItem("Time", "5x");
	menuBar->addDropDownItem("Time", "10x");
	menuBar->addDropDownItem("Time", "20x");
	menuBar->addDropDownItem("Time", "40x");
	menuBar->addDropDownItem("Time", "Real Time");

	//Set up a tile pane
	tilePane.setPosition(window->getSize().x - (window->getSize().x * 0.20f), menuBar->getSize().y);
	tilePane.setSize(sf::Vector2f((window->getSize().x * 0.20f), window->getSize().y - menuBar->getSize().y));
	tilePane.setFillColor(sf::Color(238, 233, 233, 255));
	tilePane.setOutlineColor(sf::Color(0, 0, 0, 255));
	tilePane.setOutlineThickness(1);

	UI::intializeMainWindow(window);
}

Map_Editor::Map_Editor(const std::string& title, const std::string& mapToLoad)
{
	desktop = sf::VideoMode::getDesktopMode();
	window = new sf::RenderWindow(sf::VideoMode(unsigned int(desktop.width - (desktop.width * 0.1f)), unsigned int(desktop.height - (desktop.height * 0.25f)), desktop.bitsPerPixel), title, sf::Style::Close);
	window->setVerticalSyncEnabled(true);

	//Set up camera properties
	camera = new pb::Camera(window->getSize().x, window->getSize().y, zoomFactor);
	camera->setCenter(float(desktop.width / 2), float(desktop.height / 2));
	camera->setViewport(sf::FloatRect(0, 0.03f, 1.f, 0.97f));

	gameClock = new pb::In_Game_Clock(2, 8, 0, 24, 8, 8, 4, 4);

	graphicManager = new pb::Graphic_Manager(*gameClock);
	tilePaneManager = new pb::Graphic_Manager();

	map = new Editable_Map();
	map->loadMap(mapToLoad, graphicManager, camera);

	// Set up menu menuBar
	menuBar = new Menu_Bar(sf::Vector2f(desktop.width - (desktop.width * 0.1f), (desktop.height - (desktop.height * 0.25f)) * 0.03f), sf::Vector2f(0, 0), sf::Color(238, 233, 233, 255), sf::Color(0, 0, 0, 255), 1);
	menuBar->addDropDownItem("View", "Display Grid");
	menuBar->addDropDownItem("View", "Display Collisions");
	menuBar->addDropDownItem("View", "Display Transitions");

	camera->setCenter(float(window->getSize().x / 2), float(window->getSize().y / 2));

	sf::Vector2i moveCamera = sf::Vector2i(0, 0);
	if (map->getColumns() * Editable_Map::getTileSize() < window->getSize().x)
		moveCamera.x = (window->getSize().x / 2) - (map->getMapSize().x / 2);
	if (map->getRows() * Editable_Map::getTileSize() < window->getSize().y)
		moveCamera.y = (window->getSize().y / 2) - ((map->getColumns() * Editable_Map::getTileSize()) / 2);

	camera->move((float)-moveCamera.x, (float)-moveCamera.y);
}

Map_Editor::~Map_Editor()
{
	delete graphicManager;
	delete tilePaneManager;
	delete camera;
	delete window;

	delete map;
	delete gameClock;
	delete menuBar;
}

bool Map_Editor::displayCursor()
{
	sf::Vector2i mouseCoords = sf::Mouse::getPosition(*window);

	if (menuBar->isContainerShowing())
		return true;
	else if (mouseCoords.x <= window->getSize().x - tilePane.getSize().x && mouseCoords.y >= window->getSize().y * window->getView().getViewport().top)
		return false;

	return true;
}

void Map_Editor::moveCamera()
{
	if (camera->getSize().x >= map->getMapSize().x && camera->getSize().y >= map->getMapSize().y)
		return;

	sf::Vector2f pos(0,0);

	if (camera->getSize().x <= map->getMapSize().x && camera->getSize().y <= map->getMapSize().y)
		pos = 0.1f * sf::Vector2f(sf::Mouse::getPosition(*window) - rightButtonPos) + camera->getCenter();
	else if (camera->getSize().x >= map->getMapSize().x)
		pos = 0.1f * sf::Vector2f(0.f, (float)(sf::Mouse::getPosition(*window) - rightButtonPos).y) + sf::Vector2f(0.f, camera->getCenter().y);
	else if (camera->getSize().y >= map->getMapSize().y)
		pos = 0.1f * sf::Vector2f((float)(sf::Mouse::getPosition(*window) - rightButtonPos).x, 0.f) + sf::Vector2f(camera->getCenter().x, 0.f);

	camera->updatePosition(pos);
}

void Map_Editor::parseMenuBarString(const std::string& s)
{
	if (s.compare("Display Grid") == 0 && map != 0)
		map->displayGridLayer();
	else if (s.compare("Display Collisions") == 0 && map != 0)
		map->displayCollsionLayer();
	else if (s.compare("Display Transitions") == 0 && map != 0)
		map->displayTransitionLayer();
	else if (s.compare("1x") == 0)
		gameClock->changeFactor(1);
	else if (s.compare("5x") == 0)
		gameClock->changeFactor(5);
	else if (s.compare("10x") == 0)
		gameClock->changeFactor(10);
	else if (s.compare("20x") == 0)
		gameClock->changeFactor(20);
	else if (s.compare("40x") == 0)
		gameClock->changeFactor(40);
	else if (s.compare("Real Time") == 0)
		gameClock->changeFactor(1 / 60);
	else if (s.compare("New") == 0)
		state = New;
	else if (s.compare("Save") == 0)
		state = Save;
	else if (s.compare("Load") == 0)
		state = Load;
	else
		return;

	menuBar->hideContainers();
}

void Map_Editor::processEvents()
{
	leftMouseBtnClicked = false;

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
			
			break;

		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Right)
				rightButtonPos = sf::Mouse::getPosition(*window);
			else if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::string s = "Nothing";

				if (menuBar != 0)
					parseMenuBarString(s = menuBar->itemClicked(sf::Mouse::getPosition(*window)));
				
				if (s.compare("Nothing") == 0)
					leftMouseBtnClicked = true;
			}

			break;

		case sf::Event::MouseWheelScrolled:
			if (zoomFactor - (0.05 *event.mouseWheelScroll.delta) > 0.2 && zoomFactor - (0.05 *event.mouseWheelScroll.delta) < 1)
			{
				camera->zoom(1.f - 0.05f * event.mouseWheelScroll.delta);
				zoomFactor -= (0.05f * event.mouseWheelScroll.delta);

				camera->moveCameraInBounds();
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
		menuBar->update(sf::Mouse::getPosition(*window));

		if (map != 0 && map->isMapLoaded())
		{
			window->setView(*camera);

			gameClock->updateClock();

			if (!menuBar->isContainerShowing() && sf::Mouse::isButtonPressed(sf::Mouse::Right) && sf::Mouse::getPosition().x < window->getSize().x - tilePane.getSize().x && sf::Mouse::getPosition().y > menuBar->getSize().y)
				moveCamera();
			else
				window->setMouseCursorVisible(displayCursor());

			sf::Vector2f mouse_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

			tilePaneManager->addToDrawList(&tilePane, false);

			map->updateDrawList(window, graphicManager, *camera, mouse_pos, currentTime);
			map->updateTileSheet(*window, tilePaneManager, mouse_pos, *gameClock);

			sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
			if (leftMouseBtnClicked && mousePos.x < window->getSize().x - tilePane.getSize().x && mousePos.y > menuBar->getSize().y)
				map->addTileToPos(window);
			else if (leftMouseBtnClicked)
				map->setTile(mousePos);
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
		map = new Editable_Map(tilePaneManager, (sf::Vector2i)tilePane.getPosition());

		map->loadMap(loadedMapName, graphicManager, camera);
		camera->setCenter(float(window->getSize().x / 2), float(window->getSize().y / 2));

		sf::Vector2f moveCamera = sf::Vector2f(0, 0);
		if (map->getColumns() * Editable_Map::getTileSize() < window->getSize().x)
			moveCamera.x = float((window->getSize().x / 2) - (map->getMapSize().x / 2));
		if (map->getRows() * Editable_Map::getTileSize() < window->getSize().y)
			moveCamera.y = float((window->getSize().y / 2) - ((map->getColumns() * Editable_Map::getTileSize()) / 2) - menuBar->getSize().y);

		camera->move(-moveCamera.x, -moveCamera.y);

		break;
	}
	case Save:
		state = Build;

		if (map->isMapLoaded())
			map->saveMap();

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
		map = new Editable_Map(tilePaneManager, (sf::Vector2i)tilePane.getPosition());

		sheetFileName = std::strstr(sheetFileName.c_str(), "res\\Maps\\jrs\\");
		fileName = "res/Maps/" + fileName + ".jrm";

		if (v != sf::Vector2i(-1, -1))
			map->createMap(v.y, v.x, fileName, sheetFileName, graphicManager, camera);
		else
			break;

		camera->setCenter(float(window->getSize().x / 2), float(window->getSize().y / 2));

		sf::Vector2f moveCamera = sf::Vector2f(0, 0);
		if ((unsigned int)v.x * Editable_Map::getTileSize() < window->getSize().x)
			moveCamera.x = float((window->getSize().x / 2) - (map->getMapSize().x / 2));
		if ((unsigned int)v.y * Editable_Map::getTileSize() < window->getSize().y)
			moveCamera.y = float((window->getSize().y / 2) - ((v.y * Editable_Map::getTileSize()) / 2) - menuBar->getSize().y);

		camera->move(-moveCamera.x, -moveCamera.y);

		break;
	}
	}

	if (menuBar != 0)
		tilePaneManager->addToDrawList(menuBar, false);
}

void Map_Editor::render()
{
	//Clear the window
	window->clear(sf::Color(255, 250, 250, 255));

	// Draw the map
	window->setView(*camera);
	graphicManager->draw(window, gameClock->getTime());

	// Draw the tile pane
	if (tilePaneManager != 0)
	{
		window->setView(window->getDefaultView());
		tilePaneManager->draw(window);
	}

	window->display();
}

void Map_Editor::runEditor()
{
	clock.restart();

	while (window->isOpen())
	{
		currentTime = clock.getElapsedTime();

		processEvents();

		window->setView(*camera);
		update();

		render();
	}
}

sf::Vector2i Map_Editor::runForResult()
{
	sf::Vector2i returnCoords(-1, -1);

	while (window->isOpen())
	{
		currentTime = clock.getElapsedTime();

		processEvents();

		window->setView(*camera);
		menuBar->update(sf::Mouse::getPosition(*window));

		window->setView(*camera);

		gameClock->updateClock();

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && sf::Mouse::getPosition().x < (int)window->getSize().x && sf::Mouse::getPosition().y > 0)
			moveCamera();
		else
			window->setMouseCursorVisible(displayCursor());

		sf::Vector2f mouse_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

		map->updateDrawList(window, graphicManager, *camera, mouse_pos, currentTime);
		tilePaneManager->addToDrawList(menuBar, false);

		sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
		if (leftMouseBtnClicked && mousePos.x < (int)window->getSize().x && mousePos.y < (int)window->getSize().y)
		{
			if (map->inMapBounds(mouse_pos))
			{
				sf::Vector2f coords = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
				returnCoords = sf::Vector2i((int)coords.x / map->getTileSize(), (int)coords.y / map->getTileSize());
			}
		}

		render();

		if (returnCoords.x >= 0)
			window->close();
	}

	return returnCoords;
}