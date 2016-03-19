/*
Authors: Randy Brunecz, Joshua Gray
UI.h
10/15/2015


*/

#ifndef UI_H
#define UI_H

#include <SFML\Graphics.hpp>
#include "Map.h"

class UI
{
public:
	//PUBLIC FUNCTIONS
	static sf::Vector2i getNewMapParams(std::string* str);
};

#endif