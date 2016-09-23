/*
Graphic.cpp
Parent class that defines common methods that child objects will share.

@author Randall Brunecz
@version 1.0 9/1/2015
*/

#include "PBE\Graphic\Graphic_Manager.h"

namespace pb
{
	Graphic_Manager::Graphic_Manager()
	{
		tex = new sf::RenderTexture();

		alpha = new pb::Alpha();
		alpha->load();
	}

	Graphic_Manager::Graphic_Manager(In_Game_Clock& clock) : Graphic_Manager()
	{
		dayShift = new Day_Shift_Animation(110, clock);
	}

	Graphic_Manager::~Graphic_Manager()
	{
		delete alpha;
		delete tex;
		delete dayShift;

		clearTextureList();
	}

	const sf::Texture *Graphic_Manager::addTexture(std::string fileName)
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

	void Graphic_Manager::enableDayShift(bool enable)
	{
		updateTime = enable;
	}


	void Graphic_Manager::clearTextureList()
	{
		for (unsigned int i = 0; i < textures.size(); i++)
			delete textures[i];

		textures.clear();
	}

	void Graphic_Manager::addToDrawList(sf::Sprite *s, bool hasShadow)
	{
		drawList.push_back(new Graphic_Obj(s, hasShadow));
	}

	void Graphic_Manager::draw(sf::RenderWindow* window)
	{
		for (unsigned int i = 0; i < drawList.size(); i++)
		{
			window->draw(*drawList[i]->sprite);
			delete drawList[i];
		}

		drawList.clear();
	}

	void Graphic_Manager::draw(sf::RenderWindow* window, Time& t)
	{
		// Local variables
		sf::Color dayColor = dayShift->updateDayTime(t);

		// Create a larger render texture if needed
		if (drawList.size() != 0 &&
			tex->getSize().x < drawList[0]->sprite->getTexture()->getSize().x &&
			tex->getSize().y < drawList[0]->sprite->getTexture()->getSize().y)
			tex->create(drawList[0]->sprite->getTexture()->getSize().x, drawList[0]->sprite->getTexture()->getSize().y);

		tex->clear(); // clear render texture for drawing

		for (unsigned int i = 0; i < drawList.size(); i++)
		{
			if (drawList[i]->hasShadow)
			{
				alpha->update(dayShift->getShadowAlpha());

				tex->draw(*drawList[i]->sprite, alpha->getShader());
			}
			else
				tex->draw(*drawList[i]->sprite);

			delete drawList[i];
		}

		tex->display();

		sf::Sprite s(tex->getTexture());
		s.setColor(dayColor);

		window->draw(s);

		drawList.clear();
	}

	void Graphic_Manager::dimScreen(sf::RenderWindow* window)
	{
		//Create a rectangle to lay over the window
		sf::RectangleShape dim;

		dim.setSize(sf::Vector2f((float)window->getSize().x, (float)window->getSize().y));
		dim.setPosition(window->getView().getCenter().x - (window->getSize().x * 0.5f), window->getView().getCenter().y - (window->getSize().y * 0.5f));
		dim.setFillColor(sf::Color(0, 0, 0, 150));

		window->draw(dim);
	}

	bool Graphic_Manager::fadingIn(sf::RenderWindow* window)
	{
		updateTime = false;
		if (fade == NULL)
		{
			fade = new sf::RectangleShape(sf::Vector2f((float)window->getSize().x, (float)window->getSize().y));
			fade->setFillColor(sf::Color(0, 0, 0, 255));
			fade->setPosition(window->getView().getCenter().x - (window->getSize().x * 0.5f), window->getView().getCenter().y - (window->getSize().y * 0.5f));

			aniClock.restart();
		}

		int alpha = fade->getFillColor().a;

		if (alpha > 0)
		{
			//Set the transparency
			if (aniClock.getElapsedTime().asMilliseconds() > 20)
			{
				alpha -= 15;
				fade->setFillColor(sf::Color(0, 0, 0, alpha));

				aniClock.restart();
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

	bool Graphic_Manager::fadingOut(sf::RenderWindow* window)
	{
		updateTime = false;
		if (fade == NULL)
		{
			fade = new sf::RectangleShape(sf::Vector2f((float)window->getSize().x, (float)window->getSize().y));
			fade->setFillColor(sf::Color(0, 0, 0, 0));
			fade->setPosition(window->getView().getCenter().x - (window->getSize().x * 0.5f), window->getView().getCenter().y - (window->getSize().y * 0.5f));

			aniClock.restart();
		}

		int alpha = fade->getFillColor().a;

		if (alpha < 255)
		{
			//Set the transparency
			if (aniClock.getElapsedTime().asMilliseconds() > 20)
			{
				alpha += 15;
				fade->setFillColor(sf::Color(0, 0, 0, alpha));

				aniClock.restart();
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
}