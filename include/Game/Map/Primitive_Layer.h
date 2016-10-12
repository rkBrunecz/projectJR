#ifndef Primitive_Layer_H
#define Primitive_Layer_H

#include "SFML\Graphics\Drawable.hpp"
#include "SFML\Graphics\Transformable.hpp"
#include "SFML\Graphics\VertexArray.hpp"
#include "PBE\Graphics\Graphic_Manager.h"

class Primitive_Layer : public sf::Drawable, public sf::Transformable
{
public:
	// Constructors
	Primitive_Layer(unsigned int rows, unsigned int columns, sf::PrimitiveType type, const sf::Color& c);

	// Destructor
	~Primitive_Layer();

	void update(unsigned int row, unsigned int column);

	void clearVertexArray();

	void addVerticies(unsigned int row, unsigned int column, float x, float y, unsigned int width, unsigned int height);

	void removeVerticies(unsigned int row, unsigned int column);

private:
	struct Rect
	{
		Rect(float x, float y, unsigned int width, unsigned int height) : x(x), y(y), width(width), height(height){ };

		float x, y;
		unsigned int width, height;
	};

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// No texture with absolute position
	void updateVerticies(Rect *r);

	sf::VertexArray verticies;
	sf::Color c;
	std::vector<Rect *> points;
	unsigned int numRows, numColumns;
};

#endif