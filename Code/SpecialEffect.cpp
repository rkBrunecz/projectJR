#include "SpecialEffect.h"
#include "Graphic.h"


void SpecialEffect::fadeOut(sf::RenderWindow* window, Graphic* graphics[], int arraySize)
{
	sf::Clock clock;
	int alpha = 255;

	while (clock.getElapsedTime() <= sf::seconds(0.5f))
	{
		for (int i = 0; i < arraySize; i++)
			graphics[i]->setColor(0, 0, 0, alpha);

		alpha -= 15;
	}
}

void SpecialEffect::fadeIn(sf::RenderWindow* window, Graphic* graphics[], int arraySize)
{
	sf::Clock clock;
	int alpha = 0;

	while (clock.getElapsedTime() <= sf::seconds(0.5f))
	{
		for (int i = 0; i < arraySize; i++)
			graphics[i]->setColor(0, 0, 0, alpha);

		alpha += 15;
	}
}