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
		buffer = new sf::RenderTexture();
		buffer2 = new sf::RenderTexture();
	}

	Graphic_Manager::Graphic_Manager(In_Game_Clock& clock) : Graphic_Manager()
	{
		dayShift = new Day_Shift_Animation(130, clock);

		alpha = new pb::Alpha(sf::Color::Black);
		alpha->load();

		lighting = new pb::Lighting();
		lighting->load();

		shadow = new pb::Shadow();
		shadow->load();
	}

	Graphic_Manager::~Graphic_Manager()
	{
		// Delete shaders
		delete alpha;
		delete lighting;
		delete shadow;

		// Delete buffers
		delete buffer;
		delete buffer2;

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

	void Graphic_Manager::updateDayShiftEffect(pb::In_Game_Clock& clock)
	{
		if (dayShift != 0)
		{
			delete dayShift;
			dayShift = 0;
		}

		dayShift = new Day_Shift_Animation(130, clock);
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

	void Graphic_Manager::addLight(pb::Light *l)
	{
		lights.push_back(l);
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

	void Graphic_Manager::draw(sf::RenderWindow* window, Time& t, bool clearLists)
	{
		// Local variables
		sf::Color dayColor = dayShift->updateDayTime(t);

		// Recreate the buffers to match the windows size if they do not already match
		if (buffer->getSize().x != window->getSize().x || buffer->getSize().y != window->getSize().y)
		{
			buffer->create(window->getSize().x, window->getSize().y);
			buffer2->create(window->getSize().x, window->getSize().y);
		}
		
		// Set views
		buffer->setView(window->getView());
		window->setView(window->getDefaultView());

		// Clear buffer for drawing
		buffer->clear();

		// Arrays used for the light and shadow shaders
		sf::Vector3f *lightPositions = new sf::Vector3f[lights.size()];
		sf::Glsl::Vec4 *colors = new sf::Glsl::Vec4[lights.size()];
		float *intensities = new float[lights.size()];

		sf::Vector2u originalViewSize = sf::Vector2u(unsigned int(buffer->getView().getViewport().width * window->getSize().x), unsigned int(buffer->getView().getViewport().height * window->getSize().y));
		// Convert light positions from world coordinates to screen coordinates. Populate arrays for the light and shadow shaders
		for (unsigned int i = 0; i < lights.size(); i++)
		{
			sf::Vector3f v = lights[i]->convertToScreenCoordinates(buffer->getView(), originalViewSize);
			lightPositions[i].x = v.x;
			lightPositions[i].y = v.y;
			lightPositions[i].z = v.z;

			colors[i] = lights[i]->lightColor;
			intensities[i] = lights[i]->intensity;
		}

		// Update shadow shader
		shadow->update(dayShift->getShadowAlpha(), dayShift->getShadowAlphaMax(), lightPositions, lights.size());

		// Draw everything to the buffer
		for (unsigned int i = 0; i < drawList.size(); i++)
		{
			if (drawList[i]->hasShadow)
				buffer->draw(*drawList[i]->drawable, shadow->getShader());
			else
				buffer->draw(*drawList[i]->drawable);
			
			if (clearLists)
				delete drawList[i];
		}

		// Display the buffer
		buffer->display();

		// Apply day and night shift effect
		sf::Sprite s(buffer->getTexture());
		s.setColor(dayColor);

		// Draw day and night shift effect to buffer 2
		buffer2->clear();
		buffer2->draw(s);
		buffer2->display();

		// Update lighting shader
		lighting->update(lightPositions, colors, intensities, lights.size());

		// Draw to window
		window->draw(sf::Sprite(buffer2->getTexture()), lighting->getShader());
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

		// Clear draw list and lights list
		if (clearLists)
		{
			drawList.clear();
			lights.clear();
		}

		// Free memory
		delete lightPositions;
		delete colors;
		delete intensities;
	}

	void Graphic_Manager::dimScreen(sf::Color dimColor, unsigned short alpha)
	{
		// Free memory if needed
		delete effect;
		effect = 0;

		effect = new pb::Dim(sf::Color::Black, alpha);
	}

	void Graphic_Manager::fadeIn(sf::Color fadeColor, float updateInterval, int increment)
	{
		// Free memory if needed
		delete effect;
		effect = 0;

		effect = new pb::Fade(pb::Fade::FadeIn, fadeColor, updateInterval, increment);
	}

	void Graphic_Manager::fadeOut(sf::Color fadeColor, float updateInterval, int increment)
	{
		// Free memory if needed
		delete effect;
		effect = 0;

		effect = new pb::Fade(pb::Fade::FadeOut, fadeColor, updateInterval, increment);
	}

	void Graphic_Manager::updateEffect(sf::RenderTarget& target)
	{
		if (effect != 0)
			effect->update(target);
	}

	bool Graphic_Manager::effectFinished()
	{
		return (effect == 0 ? true : effect->finished());
	}

	bool Graphic_Manager::drawListEmpty()
	{
		return (drawList.size() == 0 ? true : false);
	}
}