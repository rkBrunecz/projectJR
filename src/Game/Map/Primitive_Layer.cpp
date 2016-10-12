#include "Game\Map\Primitive_Layer.h"

Primitive_Layer::Primitive_Layer(unsigned int rows, unsigned int columns, sf::PrimitiveType type, const sf::Color& c)
{
	numRows = rows;
	numColumns = columns;

	points.resize(rows * columns);

	verticies.setPrimitiveType(type);

	this->c = c;
}

Primitive_Layer::~Primitive_Layer()
{
	for (unsigned int i = 0; i < points.size(); i++)
		delete points[i];
}

void Primitive_Layer::addVerticies(unsigned int row, unsigned int column, float x, float y, unsigned int width, unsigned int height)
{
	if (points[row * numColumns + column] != 0)
		delete points[row * numColumns + column];

	points[row * numColumns + column] = new Rect(x, y, width, height);
}

void Primitive_Layer::removeVerticies(unsigned int row, unsigned int column)
{
	if (points[row * numColumns + column] != 0)
	{
		delete points[row * numColumns + column];
		points[row * numColumns + column] = 0;
	}
}

void Primitive_Layer::update(unsigned int row, unsigned int column)
{
	if (points[row * numColumns + column] != 0)
		updateVerticies(points[row * numColumns + column]);
}

void Primitive_Layer::clearVertexArray()
{
	verticies.clear();
}

void Primitive_Layer::updateVerticies(Rect *r)
{
	switch (verticies.getPrimitiveType())
	{
	case sf::LinesStrip:
	{
		sf::Vertex verts[5];

		// Define the quads four corners
		verts[1].position = sf::Vector2f(r->x, r->y);
		verts[2].position = sf::Vector2f(r->x + r->width, r->y);
		verts[3].position = sf::Vector2f(r->x + r->width, r->y + r->height);
		verts[4].position = sf::Vector2f(r->x, r->y + r->height);
		verts[0].position = sf::Vector2f(r->x, r->y + r->height);

		// Apply color to the verticies
		verts[0].color = c;
		verts[1].color = c;
		verts[2].color = c;
		verts[3].color = c;
		verts[4].color = c;

		// Append verticies
		verticies.append(verts[0]);
		verticies.append(verts[1]);
		verticies.append(verts[2]);
		verticies.append(verts[3]);
		verticies.append(verts[4]);

		break;
	}
	default:
	{
		sf::Vertex verts[4];

		// Define the quads four corners
		verts[0].position = sf::Vector2f(r->x, r->y);
		verts[1].position = sf::Vector2f(r->x + r->width, r->y);
		verts[2].position = sf::Vector2f(r->x + r->width, r->y + r->height);
		verts[3].position = sf::Vector2f(r->x, r->y + r->height);

		// Apply color to the verticies
		verts[0].color = c;
		verts[1].color = c;
		verts[2].color = c;
		verts[3].color = c;

		// Append verticies
		verticies.append(verts[0]);
		verticies.append(verts[1]);
		verticies.append(verts[2]);
		verticies.append(verts[3]);

		break;
	}
	}
}

void Primitive_Layer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(verticies, states);
}
