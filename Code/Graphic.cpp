/*
Graphic.cpp
Parent class that defines common methods that child objects will share.

@author Randall Brunecz
@version 1.0 9/1/2015
*/

#include "Graphic.h"
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>

const sf::Color Graphic::MORNING(255, 255, 255, 255), Graphic::DAWN(150, 150, 150, 255), Graphic::AFTERNOON(200, 130, 130, 255), Graphic::EVENING(80, 80, 100, 255);

std::vector<Graphic::Texture *> Graphic::textures;
std::vector<Graphic::Graphic_Obj *> Graphic::drawList;
sf::Clock Graphic::clock;
sf::Color Graphic::dayShift(255, 255, 255, 255), Graphic::colorIncrement(1, 1, 1, 0);
sf::RectangleShape *Graphic::fade = NULL;
Graphic::TimeOfDay Graphic::shiftTo = Afternoon; 
float Graphic::dayUpdateInterval = 7;
short Graphic::shadowAlpha = SHADOW_ALPHA_MAX;
bool Graphic::updateTime = true;
sf::Shader *Graphic::alpha;
sf::RenderTexture *Graphic::tex;

sf::Texture *Graphic::addTexture(std::string fileName)
{
	if (!textures.empty())
	{
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			if (textures[i]->isSameFile(fileName))
				return textures[i]->getTexture();
		}

	}

	Texture *t = new Texture();
	t->initialize(fileName);
	textures.push_back(t);

	return textures.back()->getTexture();
}

void Graphic::initializeData()
{
	tex = new sf::RenderTexture();

	alpha = new sf::Shader();
	if (!alpha->loadFromFile("res/Shaders/alpha.frag", sf::Shader::Fragment))
		exit(EXIT_FAILURE);
}

void Graphic::freeMemory()
{
	delete alpha;
	delete tex;
	clearTextureList();
}

void Graphic::clearTextureList()
{
	for (unsigned int i = 0; i < textures.size(); i++)
		delete textures[i];

	textures.clear();
}

void Graphic::addToDrawList(sf::Sprite *s, bool isShadow)
{
	drawList.push_back(new Graphic_Obj(s, isShadow));
}

void Graphic::draw(sf::RenderWindow* window)
{
	if (drawList.size() != 0 && 
		tex->getSize().x < drawList[0]->sprite->getTexture()->getSize().x && 
		tex->getSize().y < drawList[0]->sprite->getTexture()->getSize().y)
		tex->create(drawList[0]->sprite->getTexture()->getSize().x, drawList[0]->sprite->getTexture()->getSize().y);

	tex->clear();

	if (updateTime && clock.getElapsedTime().asSeconds() > dayUpdateInterval)
	{
		updateDayTime();

		clock.restart();
	}

	for (unsigned int i = 0; i < drawList.size(); i++)
	{
		if (drawList[i]->isShadow)
		{
			alpha->setParameter("texture", sf::Shader::CurrentTexture);
			alpha->setParameter("alpha", shadowAlpha / 255.0f);

			tex->draw(*drawList[i]->sprite, alpha);
		}
		else
			tex->draw(*drawList[i]->sprite);

		delete drawList[i];
	}

	tex->display();

	sf::Sprite s(tex->getTexture());
	s.setColor(dayShift);

	window->draw(s);

	drawList.clear();
}

void Graphic::updateDayTime()
{
	dayUpdateInterval = 0.10f;

	switch (shiftTo)
	{
	case Morning:

		dayShift += colorIncrement;

		if (shadowAlpha + std::ceil(SHADOW_ALPHA_MAX / 30) < SHADOW_ALPHA_MAX)
			shadowAlpha += (short)std::ceil(SHADOW_ALPHA_MAX / 30);
		else
			shadowAlpha = SHADOW_ALPHA_MAX;

		if (dayShift == MORNING)
		{
			shiftTo = Afternoon;
			colorIncrement = sf::Color(1, 2, 2, 0);

			dayUpdateInterval = 7;
		}

		break;

	case Afternoon:

		dayShift -= colorIncrement;

		if (dayShift.r - colorIncrement.r < AFTERNOON.r)
			colorIncrement.r = dayShift.r - AFTERNOON.r;
		if (dayShift.g - colorIncrement.g < AFTERNOON.g)
			colorIncrement.g = dayShift.g - AFTERNOON.g;
		if (dayShift.b - colorIncrement.b < AFTERNOON.b)
			colorIncrement.b = dayShift.b - AFTERNOON.b;

		if (dayShift == AFTERNOON)
		{
			shiftTo = Evening;
			colorIncrement = sf::Color(4, 2, 1, 0);

			dayUpdateInterval = 7;
		}

		break;

	case Evening:

		dayShift -= colorIncrement;

		if (dayShift.r - colorIncrement.r < EVENING.r)
			colorIncrement.r = dayShift.r - EVENING.r;
		if (dayShift.g - colorIncrement.g < EVENING.g)
			colorIncrement.g = dayShift.g - EVENING.g;
		if (dayShift.b - colorIncrement.b < EVENING.b)
			colorIncrement.b = dayShift.b - EVENING.b;

		if (shadowAlpha - std::ceil(SHADOW_ALPHA_MAX / 30) > 0)
			shadowAlpha -= (short)std::ceil(SHADOW_ALPHA_MAX / 30);
		else 
			shadowAlpha = 0;

		if (dayShift == EVENING)
		{
			shiftTo = Dawn;
			colorIncrement = sf::Color(2, 2, 1, 0);

			dayUpdateInterval = 7;
		}

		break;

	case Dawn:

		dayShift += colorIncrement;

		if (dayShift.r + colorIncrement.r > DAWN.r)
			colorIncrement.r = abs(dayShift.r - DAWN.r);
		if (dayShift.g + colorIncrement.g > DAWN.g)
			colorIncrement.g = abs(dayShift.g - DAWN.g);
		if (dayShift.b + colorIncrement.b > DAWN.b)
			colorIncrement.b = abs(dayShift.b - DAWN.b);

		if (shadowAlpha + std::ceil(SHADOW_ALPHA_MAX / 30) < SHADOW_ALPHA_MAX - 75)
			shadowAlpha += (short)std::ceil(SHADOW_ALPHA_MAX / 30);
		else
			shadowAlpha = SHADOW_ALPHA_MAX - 75;

		if (dayShift == DAWN)
		{
			shiftTo = Morning;
			colorIncrement = sf::Color(1, 1, 1, 0);

			dayUpdateInterval = 7;
		}

		break;
	}

	

}

void Graphic::dimScreen(sf::RenderWindow* window)
{
	//Create a rectangle to lay over the window
	sf::RectangleShape dim;

	dim.setSize(sf::Vector2f((float)window->getSize().x, (float)window->getSize().y));
	dim.setPosition(window->getView().getCenter().x - (window->getSize().x * 0.5f), window->getView().getCenter().y - (window->getSize().y * 0.5f));
	dim.setFillColor(sf::Color(0, 0, 0, 150));

	window->draw(dim);
}

bool Graphic::fadingIn(sf::RenderWindow* window)
{
	updateTime = false;
	if (fade == NULL)
	{
		fade = new sf::RectangleShape(sf::Vector2f((float)window->getSize().x, (float)window->getSize().y));
		fade->setFillColor(sf::Color(0, 0, 0, 255));
		fade->setPosition(window->getView().getCenter().x - (window->getSize().x * 0.5f), window->getView().getCenter().y - (window->getSize().y * 0.5f));

		clock.restart();
	}

	int alpha = fade->getFillColor().a;

	if (alpha > 0)
	{
		//Set the transparency
		if (clock.getElapsedTime().asMilliseconds() > 20)
		{
			alpha -= 15;
			fade->setFillColor(sf::Color(0, 0, 0, alpha));

			clock.restart();
		}

		window->clear();

		draw(window);
		window->draw(*fade);

		window->display();
	}

	if (alpha != 0)
		return true;
	else
	{
		updateTime = true;
		
		delete fade;
		fade = NULL;

		return false;
	}
}

bool Graphic::fadingOut(sf::RenderWindow* window)
{
	updateTime = false;
	if (fade == NULL)
	{
		fade = new sf::RectangleShape(sf::Vector2f((float)window->getSize().x, (float)window->getSize().y));
		fade->setFillColor(sf::Color(0, 0, 0, 0));
		fade->setPosition(window->getView().getCenter().x - (window->getSize().x * 0.5f), window->getView().getCenter().y - (window->getSize().y * 0.5f));

		clock.restart();
	}

	int alpha = fade->getFillColor().a;

	if (alpha < 255)
	{
		//Set the transparency
		if (clock.getElapsedTime().asMilliseconds() > 20)
		{
			alpha += 15;
			fade->setFillColor(sf::Color(0, 0, 0, alpha));

			clock.restart();
		}

		window->clear();

		draw(window);
		window->draw(*fade);

		window->display();
	}

	if (alpha != 255)
		return true;
	else
	{
		updateTime = true;

		delete fade;
		fade = NULL;

		return false;
	}
}

/*
draw
Parameters:
	window: This is the window where all graphics will be drawn

This method is shell. Every child of graphic will have it's own defined
draw method that will be specific to that object.
*/
void Graphic::updateDrawList(bool animate)
{
	
}

/*
updatePosition
Parameters:
	window: Used to get information about the window and to poll events, if needed.

This method is shell. Every child of graphic will have it's own defined
updatePosition method which will dictate how positions of graphical objects
will move around. Return is the default use of this method. This is specifically
used for graphic elements that do not need their positions updated.
*/
void Graphic::updatePosition()
{
	return;
}

/*
setColor
Parameters:
	r: This value determines the red scale for a graphic
	g: This value determines the green scale for a graphic
	b: This value determines the blue scale for a graphic
	a: This value determines the transparency of a graphic

This method sets the color and transparency for a given graphic object
*/
void Graphic::setColor(int r, int g, int b, int a)
{
	return;
}

bool Graphic::collisionDetected(sf::IntRect* tmp)
{
	return false;
}

sf::IntRect Graphic::getRect()
{
	return sf::IntRect(-1, -1, -1, -1);
}
