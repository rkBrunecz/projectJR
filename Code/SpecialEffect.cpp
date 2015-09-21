/*
Authors: Randy Brunecz, Joshua Gray
SpecialEffect.h
9/3/2015

The special effects object contains many methods for creating special effects such as fades, screen dims, etc.
*/

#include "SpecialEffect.h"
#include "Graphic.h"

/*
fadeOut
Parameters:
	window: The game window
	map: This object contains the core draw method needed to render the game properly
	player: Used in the map draw call and to set it's transparency.

This method performs a fade out effect.
*/
void SpecialEffect::fadeOut(sf::RenderWindow* window, Map* map, Player* player)
{
	//LOCAL VARIABLES
	sf::Clock clock;
	int alpha = 255;

	//Perform a fade out over a half second
	while (clock.getElapsedTime() <= sf::seconds(0.3f))
	{
		//Set the transparency
		map->setColor(255, 255, 255, alpha);
		player->setColor(255, 255, 255, alpha);

		if (alpha != 0)
			alpha -= 15;

		//Clear window
		window->clear();

		//Draw all of the graphics
		map->draw(window, player);

		//Display everything in the window
		window->display();
	}

	//If for some reason the fade did not finish, set the graphics to be fully transparent
	if (alpha != 0)
	{
		//Set the transparency
		map->setColor(255, 255, 255, 0);
		player->setColor(255, 255, 255, 0);
	}
}

/*
fadeIn
Parameters:
	window: The game window
	map: This object contains the core draw method needed to render the game properly
	player: Used in the map draw call and to set it's transparency.

This method performs a fade in effect.
*/
void SpecialEffect::fadeIn(sf::RenderWindow* window, Map* map, Player* player)
{
	//LOCAL VARIABLES
	sf::Clock clock;
	int alpha = 0;

	//Perform a fade in over a half second
	while (clock.getElapsedTime() <= sf::seconds(0.3f))
	{
		//Set the transparency
		map->setColor(255, 255, 255, alpha);
		player->setColor(255, 255, 255, alpha);

		if (alpha != 255)
			alpha += 15;

		//Clear window
		window->clear();

		//Draw all of the graphics
		map->draw(window, player);

		//Display everything in the window
		window->display();
	}

	//If for some reason the alpha is not 255, set all graphics objects to be have no transparency
	if (alpha != 255)
	{
		//Set the transparency
		map->setColor(255, 255, 255, 255);
		player->setColor(255, 255, 255, 255);
	}
}

/*
screenDim
Parameters:
	window: This is the window that will be drawn on

This method overlays a black rectangle over the window to create a "dim" effect
*/
void SpecialEffect::screenDim(sf::RenderWindow* window)
{
	//Create a rectangle to lay over the window
	sf::RectangleShape dim;
	dim.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	dim.setPosition(window->getView().getCenter().x - (window->getSize().x * 0.5), window->getView().getCenter().y -  (window->getSize().y * 0.5));
	dim.setFillColor(sf::Color(0, 0, 0, 150));

	window->draw(dim);
}