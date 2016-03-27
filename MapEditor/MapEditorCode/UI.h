/*
UI.h
UI handles several user interface components. Primarily used to create additional windows and open file selectors.

@author Randall Brunecz
@version 1.0 10/15/2015
*/

#ifndef UI_H
#define UI_H

#define NOMINMAX

#include <Windows.h>
#include <SFML\Graphics.hpp>
#include "Map.h"

class UI
{
public:
	//PUBLIC FUNCTIONS
	static void intializeMainWindow(sf::RenderWindow* window);
	static sf::Vector2i getNewMapParams(std::string* str);
	static std::string getMap(std::string filter);
	static sf::Vector2i getCoordinates(std::string windowName);


private:
	static sf::RenderWindow* mainWindow;
};

#endif