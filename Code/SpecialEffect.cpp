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
		for (int i = 0; i < arraySize; i++)
			graphics[i]->setColor(0, 0, 0, alpha);

		alpha -= 15;
	}
}

/*
fadeIn
Parameters:
window: The game window
graphics[]: The array that holds all of the graphics
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
		for (int i = 0; i < arraySize; i++)
			graphics[i]->setColor(0, 0, 0, alpha);

		alpha += 15;
	}
}