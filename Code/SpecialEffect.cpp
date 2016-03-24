/*
SpecialEffect.cpp
The special effects object contains many methods for creating special effects such as fades, screen dims, etc.

@author Randall Brunecz
@version 1.0 9/3/2015
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
	int alpha = 0;
	sf::RectangleShape fade;

	fade.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	fade.setPosition(window->getView().getCenter().x - (window->getSize().x * 0.5), window->getView().getCenter().y - (window->getSize().y * 0.5));
	fade.setFillColor(sf::Color(0, 0, 0, alpha));

	//Perform a fade out over a half second
	while (alpha <= 255)
	{
		//Set the transparency
		if (clock.getElapsedTime() > sf::seconds(0.01f))
		{
			fade.setFillColor(sf::Color(0, 0, 0, alpha));
			alpha += 15;
			
			clock.restart();
		}

		//Clear window
		window->clear();

		//Draw all of the graphics
		map->draw(window, player, true);
		window->draw(fade);

		//Display everything in the window
		window->display();
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
	int alpha = 255;
	sf::RectangleShape fade;

	fade.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	fade.setPosition(window->getView().getCenter().x - (window->getSize().x * 0.5), window->getView().getCenter().y - (window->getSize().y * 0.5));
	fade.setFillColor(sf::Color(0, 0, 0, alpha));

	//Perform a fade in over a half second
	while (alpha >= 0)
	{
		//Set the transparency
		if (clock.getElapsedTime() > sf::seconds(0.01f))
		{
			fade.setFillColor(sf::Color(0, 0, 0, alpha));
			alpha -= 15;

			clock.restart();
		}

		//Clear window
		window->clear();

		//Draw all of the graphics
		map->draw(window, player, true);
		window->draw(fade);

		//Display everything in the window
		window->display();
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