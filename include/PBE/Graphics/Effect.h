#ifndef Effect_H
#define Effect_H

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Color.hpp>
#include <SFML\System\Clock.hpp>
#include <stdlib.h>
#include <string>

namespace pb
{
	class Effect : public sf::Drawable
	{
	public:
		void update(sf::RenderTarget& target)
		{
			updateParameters(target);
		}

		bool finished()
		{
			return isFinished();
		}

	protected:
		Effect(const std::string name) : name(name) { };

		// PROTECTED VARIABLES
		std::string name;

	private:
		// VIRTUAL METHODS
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
		virtual void updateParameters(sf::RenderTarget& target) = 0;
		virtual bool isFinished() = 0;
	};

	class Dim : public Effect
	{
	public:
		Dim(sf::Color dimColor, unsigned short alpha) : Effect("Dim")
		{
			this->dimColor = dimColor;
			this->dimColor.a = (sf::Uint8)alpha;

			rect.setFillColor(this->dimColor);
		}

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(rect, states);
		}

		void updateParameters(sf::RenderTarget& target)
		{
			rect.setPosition(target.getView().getCenter().x - (target.getSize().x * 0.5f), target.getView().getCenter().y - (target.getSize().y * 0.5f));
			rect.setSize(sf::Vector2f((float)target.getSize().x, (float)target.getSize().y));
		}

		bool isFinished()
		{
			return true;
		}


		// Private variables
		sf::RectangleShape rect;
		sf::Color dimColor;
	};

	class Fade : public Effect
	{
	public:
		const enum States
		{
			FadeIn,
			FadeOut,
			Finished
		};

		Fade(States state, sf::Color fadeColor, float updateInterval) : Effect("Fade"), UPDATE_INTERVAL(updateInterval)
		{
				// Set up properties of the fade rectangle
				this->fadeColor = fadeColor;		
				this->state = state;

				switch (state)
				{
				case FadeIn:
					this->fadeColor.a = ALPHA_MAX;

					break;

				case FadeOut:
					this->fadeColor.a = 0;

					break;
				}

				rect.setFillColor(this->fadeColor);
		};

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(rect, states);
		}

		void updateParameters(sf::RenderTarget& target)
		{
			rect.setPosition(target.getView().getCenter().x - (target.getSize().x * 0.5f), target.getView().getCenter().y - (target.getSize().y * 0.5f));
			rect.setSize(sf::Vector2f((float)target.getSize().x, (float)target.getSize().y));

			switch (state)
			{
			case FadeIn:
				fadeIn();

				break;

			case FadeOut:
				fadeOut();

				break;
			}
		}

		void fadeIn()
		{
			if (fadeColor.a == 0 && aniClock.getElapsedTime().asSeconds() > UPDATE_INTERVAL)
				state = Finished;

			if (fadeColor.a > 0)
			{
				//Set the transparency
				if (aniClock.getElapsedTime().asSeconds() > UPDATE_INTERVAL)
				{
					fadeColor.a -= 15;
					rect.setFillColor(fadeColor);
					aniClock.restart();
				}
			}
		}
		
		void fadeOut()
		{
			if (fadeColor.a == ALPHA_MAX && aniClock.getElapsedTime().asSeconds() > UPDATE_INTERVAL)
				state = Finished;

			if (fadeColor.a < ALPHA_MAX)
			{
				//Set the transparency
				if (aniClock.getElapsedTime().asSeconds() > UPDATE_INTERVAL)
				{
					fadeColor.a += 15;
					rect.setFillColor(fadeColor);
					aniClock.restart();
				}
			}
		}

		bool isFinished()
		{
			return (state == Finished ? true : false);
		}

		// Private constants
		const sf::Uint8 ALPHA_MAX = 255;
		const float UPDATE_INTERVAL; // In seconds

		// Private variables
		sf::Clock aniClock;
		sf::RectangleShape rect;
		sf::Color fadeColor;
		States state;
	};
}

#endif