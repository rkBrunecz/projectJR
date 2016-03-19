/*
Authors: Randy Brunecz, Joshua Gray
MapEditor.cpp
10/12/2015


*/

#include <SFML\Graphics.hpp>
#include "Map.h"
#include "Camera.h"
#include "Collision.h"
#include "UI.h"
#include <iostream>
#include <Windows.h>

const enum Editor_States //Dictates the state the game is in
{
	Build,
	New,
	Load,
	Save,
	Quit
};

//GLOBAL VARIABLES
Editor_States state = Build;
sf::Vector2i rightButtonPos;

bool displayCursor(sf::RenderWindow& window, sf::Vector2i mouseCoords)
{
	if (mouseCoords.x <= window.getSize().x - 384 && mouseCoords.y >= window.getSize().y * window.getView().getViewport().top)
		return false;

	return true;
}

void moveCamera(Camera& camera, sf::RenderWindow& window, Map& map)
{
	if (camera.getSize().x >= map.mapSize().x && camera.getSize().y >= map.mapSize().y)
		return;

	sf::Vector2f pos(0,0);

	if (camera.getSize().x <= map.mapSize().x && camera.getSize().y <= map.mapSize().y)
		pos = 0.1f * sf::Vector2f(sf::Mouse::getPosition(window) - rightButtonPos) + camera.getCenter();
	else if (camera.getSize().x >= map.mapSize().x)
		pos = 0.1f * sf::Vector2f(0, (sf::Mouse::getPosition(window) - rightButtonPos).y) + sf::Vector2f(0, camera.getCenter().y);
	else if (camera.getSize().y >= map.mapSize().y)
		pos = 0.1f * sf::Vector2f((sf::Mouse::getPosition(window) - rightButtonPos).x, 0) + sf::Vector2f(camera.getCenter().x, 0);

	camera.updatePosition(sf::Vector2i(pos));
}

std::string loadMap(sf::RenderWindow& window)
{
	OPENFILENAME ofn;

	char currentDirectory[MAX_PATH];
	GetModuleFileName(NULL, currentDirectory, MAX_PATH);
	std::string::size_type pos = std::string(currentDirectory).find_last_of("\\/");

	// a another memory buffer to contain the file name	
	char szFile[100];

	// open a file name
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = window.getSystemHandle();
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "JRM\0*.jrm\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	GetOpenFileName(&ofn);

	std::string s = ofn.lpstrFile;

	SetCurrentDirectory(std::string(currentDirectory).substr(0, pos).c_str());

	return s;
}

/*
runGame
Parameters:
	window     - The game window where graphics are drawn
	map        - This object contains information about the map

This method is where all game related work is done.
*/
void runEditor(sf::RenderWindow& window, Camera& camera, Map& map, sf::RectangleShape& menuBar, sf::RectangleShape& tilePane)
{
	//Clear the window
	window.clear(sf::Color(255,250,250,255));

	switch (state)
	{
	case Build:
		if (map.isMapLoaded())
		{
			window.setView(camera);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && sf::Mouse::getPosition().x < window.getSize().x - 382 && sf::Mouse::getPosition().y > 32)
				moveCamera(camera, window, map);
			else
				window.setMouseCursorVisible(displayCursor(window, sf::Mouse::getPosition(window)));

			sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			map.draw(&window, mouse_pos);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Mouse::getPosition(window).x < window.getSize().x - tilePane.getSize().x && sf::Mouse::getPosition(window).y > menuBar.getSize().y)
				map.addTileToPos();
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				map.setTile(sf::Mouse::getPosition(window));

			window.setView(window.getDefaultView());
			window.draw(tilePane);
			map.drawTileSheet(&window);
		}

		break;
	case Load:
		map.loadMap(loadMap(window), &camera);
		camera.setCenter(window.getSize().x / 2, window.getSize().y / 2);

		state = Build;

		break;

	case Save:
		map.saveMap();

		state = Build;

		break;

	case New:
	{
		std::string fileName;

		sf::Vector2i v = UI::getNewMapParams(&fileName);

		fileName = "bin/Graphics/" + fileName + ".jrm";

		if (v != sf::Vector2i(-1, -1))
			map.createMap(v.x, v.y, &camera, "name");

		state = Build;

		break;
	}
	case Quit:
		return;
		break;
	}

	//Redisplay everything in the window
	window.setView(window.getDefaultView());
	window.draw(menuBar);
	window.display();
}

int main()
{
	//LOCAL VARIABLES
	Camera camera;
	Map map;
	float zoomFactor = 1;

	//Create a fullscreen window with same pixel depth (a.k.a bit depth/color depth) as the desktop
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(desktop.width - (desktop.width * 0.1), desktop.height - (desktop.height * 0.25), desktop.bitsPerPixel), "Map Editor", sf::Style::Default);

	//Set up camera properties
	camera.setSize(desktop.width - 384, desktop.height - (desktop.height * 0.25));
	camera.setCenter(desktop.width / 2, desktop.height / 2);
	camera.setViewport(sf::FloatRect(0, 0.03f, 0.85f, 0.97f));

	//Set up a menu bar
	sf::RectangleShape menuBar;
	menuBar.setPosition(0, 0);
	menuBar.setSize(sf::Vector2f(desktop.width - (desktop.width * 0.1), (desktop.height - (desktop.height * 0.25)) * 0.03));
	menuBar.setFillColor(sf::Color(238, 233, 233, 255));
	menuBar.setOutlineColor(sf::Color(0, 0, 0, 255));
	menuBar.setOutlineThickness(1);

	//Set up a tile pane
	sf::RectangleShape tilePane;
	tilePane.setPosition(window.getSize().x - 384, menuBar.getSize().y);
	tilePane.setSize(sf::Vector2f(384, window.getSize().y - menuBar.getSize().y));
	tilePane.setFillColor(sf::Color(238, 233, 233, 255));
	tilePane.setOutlineColor(sf::Color(0, 0, 0, 255));
	tilePane.setOutlineThickness(1);

	//Intialize additional parameters
	window.setVerticalSyncEnabled(true);
	map.initializeTileSheetCoords(&window);

	//GAME LOOP
	while (state != Quit)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				state = Quit;
				break;

			case sf::Event::KeyPressed: 
				if (event.key.code == sf::Keyboard::Escape)
					state = Quit;
				else if (event.key.code == sf::Keyboard::Tab)
					map.displayCollsionLayer();
				else if (event.key.code == sf::Keyboard::G)
					map.displayGridLayer();
				else if (event.key.code == sf::Keyboard::L)
					state = Load;
				else if (event.key.code == sf::Keyboard::N)
					state = New;
				else if (event.key.code == sf::Keyboard::S)
					state = Save;
				break;

			case sf::Event::MouseButtonPressed: 
				if(event.mouseButton.button == sf::Mouse::Right)
					rightButtonPos = sf::Mouse::getPosition(window);
				break;

			case sf::Event::MouseWheelScrolled: 
				if(zoomFactor - (0.05 *event.mouseWheelScroll.delta) > 0.2 && zoomFactor - (0.05 *event.mouseWheelScroll.delta) < 1)
				{
					camera.zoom(1 - 0.05f * event.mouseWheelScroll.delta);
					zoomFactor -= (0.05 * event.mouseWheelScroll.delta);
				}
				break;
			}
		}

		runEditor(window, camera, map, menuBar, tilePane); //Run the game based on the current game state
	}

	window.close(); //Close the game window

	return 0; //Close the game
}