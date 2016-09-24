/*
Graphic.cpp
Parent class that defines common methods that child objects will share.

@author Randall Brunecz
@version 1.0 9/1/2015
*/

#include "PBE\Graphics\Graphic_Manager.h"

namespace pb
{
	Graphic_Manager::Graphic_Manager()
	{
		tex = new sf::RenderTexture();

		alpha = new pb::Alpha(sf::Color::Black);
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
		delete effect;

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

	void Graphic_Manager::addToDrawList(sf::Drawable *d, bool hasShadow)
	{
		drawList.push_back(new Graphic_Obj(d, hasShadow));
	}

	void Graphic_Manager::draw(sf::RenderWindow* window)
	{
		for (unsigned int i = 0; i < drawList.size(); i++)
		{
			window->draw(*drawList[i]->drawable);
			delete drawList[i];
		}

		if (effect != 0)
		{
			effect->update(*window);
			window->draw(*effect);
			
			if (effect->finished())
			{
				delete effect;
				effect = 0;
			}
		}

		drawList.clear();
	}

	void Graphic_Manager::draw(sf::RenderWindow* window, Time& t)
	{
		// Local variables
		sf::Color dayColor = dayShift->updateDayTime(t);

		// Create a larger render texture if needed
		if (drawList.size() != 0 &&
			tex->getSize().x < 3000 &&
			tex->getSize().y < 3000)
			tex->create(3000, 3000);
		
		tex->clear(); // clear render texture for drawing

		for (unsigned int i = 0; i < drawList.size(); i++)
		{
			if (drawList[i]->hasShadow)
			{
				alpha->update(dayShift->getShadowAlpha());

				tex->draw(*drawList[i]->drawable, alpha->getShader());
			}
			else
				tex->draw(*drawList[i]->drawable);

			delete drawList[i];
		}

		// Finished drawing to render texture
		tex->display();

		// Apply day and night shift
		sf::Sprite s(tex->getTexture());
		s.setColor(dayColor);

		// Draw to window
		window->draw(s);
		if (effect != 0)
		{
			effect->update(*window);
			window->draw(*effect);

			if (effect->finished())
			{
				delete effect;
				effect = 0;
			}
		}

		drawList.clear();
	}

	void Graphic_Manager::dimScreen(sf::Color dimColor, unsigned short alpha)
	{
		// Free memory if needed
		delete effect;
		effect = 0;

		effect = new pb::Dim(sf::Color::Black, alpha);
	}

	void Graphic_Manager::fadeIn(sf::Color fadeColor, float updateInterval)
	{
		// Free memory if needed
		delete effect;
		effect = 0;

		effect = new pb::Fade(pb::Fade::FadeIn, fadeColor, updateInterval);
	}

	void Graphic_Manager::fadeOut(sf::Color fadeColor, float updateInterval)
	{
		// Free memory if needed
		delete effect;
		effect = 0;

		effect = new pb::Fade(pb::Fade::FadeOut, fadeColor, updateInterval);
	}

	bool Graphic_Manager::effectFinished()
	{
		return (effect == 0 ? true : effect->finished());
	}
}