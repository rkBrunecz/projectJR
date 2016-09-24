#ifndef Graphic_Entity_H
#define Graphic_Entity_H

// HEADERS
#include "SFML\Graphics.hpp"

namespace pb
{
	class Graphic_Entity
	{
	public:
		// PURE VIRTUAL METHODS
		virtual void updateDrawList() = 0;
		virtual void updatePosition(const float elapsedTime) = 0;
		virtual void setPosition(const sf::Vector2f coords) = 0;
		virtual const sf::IntRect getRect() = 0;			
	};
}

#endif