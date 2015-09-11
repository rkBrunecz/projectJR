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
	graphics[]: The array that holds all of the graphics
	arraySize: Size of the graphics array

This method performs a fade out effect.
*/
void SpecialEffect::fadeOut(sf::RenderWindow* window, Graphic* graphics[], int arraySize)
{
	//LOCAL VARIABLES
	sf::Clock clock;
	int alpha = 255;

	//Perform a fade out over a half second
	while (clock.getElapsedTime() <= sf::seconds(0.5f))
	{
		//Set the transparency
		for (int i = 0; i < arraySize; i++)
			graphics[i]->setColor(255, 255, 255, alpha);

		if (alpha != 0)
			alpha -= 15;

		//Clear window
		window->clear();

		//Draw all of the graphics
		for (int i = 0; i < arraySize; i++)
			graphics[i]->draw(window);

		//Display everything in the window
		window->display();
	}
}

/*
fadeIn
Parameters:
	window: The game window
	graphics[]: The array that holds all of the graphics
	map: The sprite of the map to be drawn 
	arraySize: Size of the graphics array

This method performs a fade in effect.
*/
void SpecialEffect::fadeIn(sf::RenderWindow* window, Graphic* graphics[], int arraySize)
{
	//LOCAL VARIABLES
	sf::Clock clock;
	int alpha = 0;

	//Perform a fade in over a half second
	while (clock.getElapsedTime() <= sf::seconds(0.5f))
	{
		//Set the transparency
		for (int i = 0; i < arraySize; i++)
			graphics[i]->setColor(255, 255, 255, alpha);

		if (alpha != 255)
			alpha += 15;

		//Clear window
		window->clear();

		//Draw all of the graphics
		for (int i = 0; i < arraySize; i++)
			graphics[i]->draw(window);

		//Display everything in the window
		window->display();
	}
}

/*
screenDim
Parameters:
	graphics[]: An array containing all of the graphics
	arraySize: Size of the graphics array

This method dims all of the graphics in the graphics array
*/
void SpecialEffect::screenDim(Graphic* graphics[], int arraySize)
{
	//Go through the list and dim all of the graphics in the array
	for (int i = 0; i < arraySize; i++)
		graphics[i]->setColor(255, 255, 255, 150);
}

/*
resetScreenDim
Parameters:
	graphics[]: An array containing all of the graphics
	arraySize: Size of the graphics array

This method resets all graphics back to their original transparency.
*/
void SpecialEffect::resetScreenDim(Graphic* graphics[], int arraySize)
{
	//Go through the list and dim all of the graphics in the array
	for (int i = 0; i < arraySize; i++)
		graphics[i]->setColor(255, 255, 255, 255);
}