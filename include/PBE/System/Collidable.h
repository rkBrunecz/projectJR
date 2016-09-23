#ifndef Collidable_H
#define Collidable_H

// HEADERS
#include "SFML\Graphics.hpp"

namespace pb
{
	class Collidable
	{
	public:
		// PURE VIRTUAL METHODS
		virtual bool collisionDetected(const sf::IntRect& rect) = 0;
	};
}

#endif