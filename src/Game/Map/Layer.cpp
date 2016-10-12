#include "Game\Map\Layer.h"

Layer::Layer(const std::string& tileset, unsigned int numRows, unsigned int numColumns, pb::Graphic_Manager *manager)
{
	this->tileset = manager->addTexture(tileset);

	this->verticies.setPrimitiveType(sf::Quads);

	layer.resize(numRows * numColumns);
	this->numRows = numRows;
	this->numColumns = numColumns;
}

Layer::~Layer()
{
	for (unsigned int i = 0; i < layer.size(); i++)
		delete layer[i];
}

void Layer::update(unsigned int tileSize, unsigned int row, unsigned int column, const sf::Time& currentTime)
{
	unsigned int layerRow = (numRows != 1 ? row : 0);

	if (layer[layerRow * numColumns + column] != 0)
	{
		layer[layerRow * numColumns + column]->update(currentTime);
		updateVerticies(row, column, tileSize, *layer[layerRow * numColumns + column]);
	}
}

void Layer::clearVertexArray()
{
	verticies.clear();
}

Tile *Layer::getTile(unsigned int row, unsigned int column)
{
	if (numRows == 1)
		row = 0;

	return layer[row * numColumns + column];
}

void Layer::changeTileCollision(unsigned int row, unsigned int column, bool collidable)
{
	if (numRows == 1)
		row = 0;

	if (layer[row * numColumns + column] != 0)
		layer[row * numColumns + column]->collidable = collidable;
}

void Layer::addTile(const Tile& t, unsigned int row, unsigned int column)
{
	if (numRows == 1)
		row = 0;

	if (layer[row * numColumns + column] == 0)
		layer[row * numColumns + column] = new Tile(t);
	else
	{
		sf::Vector2f coords = layer[row * numColumns + column]->transitionCoords;
		std::string mapName = layer[row * numColumns + column]->mapName;

		delete layer[row * numColumns + column];
		layer[row * numColumns + column] = new Tile(t);

		layer[row * numColumns + column]->transitionCoords = coords;
		layer[row * numColumns + column]->mapName = mapName;
	}
}

void Layer::addTile(const Animated_Tile& t, unsigned int row, unsigned int column)
{
	if (numRows == 1)
		row = 0;

	if (layer[row * numColumns + column] == 0)
		layer[row * numColumns + column] = new Animated_Tile(t);
	else
	{
		sf::Vector2f coords = layer[row * numColumns + column]->transitionCoords;
		std::string mapName = layer[row * numColumns + column]->mapName;

		delete layer[row * numColumns + column];
		layer[row * numColumns + column] = new Animated_Tile(t);

		layer[row * numColumns + column]->transitionCoords = coords;
		layer[row * numColumns + column]->mapName = mapName;
	}
}

void Layer::removeTile(unsigned int row, unsigned int column)
{
	if (numRows == 1)
		row = 0;

	delete layer[row * numColumns + column];
	layer[row * numColumns + column] = 0;
}

void Layer::updateVerticies(unsigned int row, unsigned int column, unsigned int tileSize, const Tile& t)
{
	sf::Vertex quad[4];
	
	// Define the quads four corners
	quad[0].position = sf::Vector2f(float(column * tileSize), float(row * tileSize));
	quad[1].position = sf::Vector2f(float((column + 1) * tileSize), float(row * tileSize));
	quad[2].position = sf::Vector2f(float((column + 1) * tileSize), float((row + 1) * tileSize));
	quad[3].position = sf::Vector2f(float(column * tileSize), float((row + 1) * tileSize));

	// Defines its four texture coordinates
	quad[0].texCoords = sf::Vector2f(float(t.column * tileSize), float(t.row * tileSize));
	quad[1].texCoords = sf::Vector2f(float((t.column + 1) * tileSize), float(t.row * tileSize));
	quad[2].texCoords = sf::Vector2f(float((t.column + 1) * tileSize), float((t.row + 1) * tileSize));
	quad[3].texCoords = sf::Vector2f(float(t.column * tileSize), float((t.row + 1) * tileSize));
	
	// Determine transformations
	if (t.rotation >= 1 && t.rotation <= 3)
		rotateTile(quad, tileSize, t);
	if (t.mirror)
		mirrorTile(quad, tileSize, t);

	verticies.append(quad[0]);
	verticies.append(quad[1]);
	verticies.append(quad[2]);
	verticies.append(quad[3]);
}

void Layer::rotateTile(sf::Vertex *quad, unsigned int tileSize, const Tile& t)
{
	if (t.rotation == 1) // 90 degree rotation
	{
		quad[0].texCoords = sf::Vector2f(float(t.column * tileSize), float((t.row + 1) * tileSize));
		quad[1].texCoords = sf::Vector2f(float(t.column * tileSize), float(t.row * tileSize));
		quad[2].texCoords = sf::Vector2f(float((t.column + 1) * tileSize), float(t.row * tileSize));
		quad[3].texCoords = sf::Vector2f(float((t.column + 1) * tileSize), float((t.row + 1) * tileSize));
	}
	else if (t.rotation == 2) // 180 degree rotation
	{
		quad[0].texCoords = sf::Vector2f(float((t.column + 1) * tileSize), float((t.row + 1) * tileSize));
		quad[1].texCoords = sf::Vector2f(float(t.column * tileSize), float((t.row + 1) * tileSize));
		quad[2].texCoords = sf::Vector2f(float(t.column * tileSize), float(t.row * tileSize));
		quad[3].texCoords = sf::Vector2f(float((t.column + 1) * tileSize), float(t.row * tileSize));
	}
	else if (t.rotation == 3) // 270 degree rotation
	{
		quad[0].texCoords = sf::Vector2f(float((t.column + 1) * tileSize), float(t.row * tileSize));
		quad[1].texCoords = sf::Vector2f(float((t.column + 1) * tileSize), float((t.row + 1) * tileSize));
		quad[2].texCoords = sf::Vector2f(float(t.column * tileSize), float((t.row + 1) * tileSize));
		quad[3].texCoords = sf::Vector2f(float(t.column * tileSize), float(t.row * tileSize));
	}
}

void Layer::mirrorTile(sf::Vertex *quad, unsigned int tileSize, const Tile& t)
{
	sf::Vector2f texCoords0 = quad[0].texCoords, 
		texCoords1 = quad[1].texCoords,
		texCoords2 = quad[2].texCoords,
		texCoords3 = quad[3].texCoords;

	// Mirror tile
	quad[1].texCoords = texCoords0;
	quad[0].texCoords = texCoords1;
	quad[3].texCoords = texCoords2;
	quad[2].texCoords = texCoords3;
}

void Layer::addTransitionPoint(unsigned int row, unsigned int column, const std::string& transitionMapName, const sf::Vector2f& coords)
{
	if (numRows == 1)
		row = 0;

	if (layer[row * numColumns + column] == 0)
		layer[row * numColumns + column] = new Tile();

	layer[row * numColumns + column]->tileType = 'E';
	layer[row * numColumns + column]->mapName = transitionMapName;
	layer[row * numColumns + column]->transitionCoords = coords;
}

void Layer::removeTransitionPoint(unsigned int row, unsigned int column)
{
	if (numRows == 1)
		row = 0;

	if (layer[row * numColumns + column] == 0)
		return;

	layer[row * numColumns + column]->mapName = "";
	layer[row * numColumns + column]->tileType = 'G';
}

void Layer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	states.texture = tileset;

	target.draw(verticies, states);
}

bool Layer::isColliding(const sf::IntRect& rect, unsigned int row, unsigned int column, unsigned int tileSize)
{
	unsigned int layerRow = (numRows != 1 ? row : 0);

	//Create a bounding box for the current tile.
	sf::IntRect boundingBox(column * tileSize + layer[layerRow * numColumns + column]->bBX,
		row * tileSize + layer[layerRow * numColumns + column]->bBY,
		layer[layerRow * numColumns + column]->width,
		layer[layerRow * numColumns + column]->height);

	if (boundingBox.top == rect.height + rect.top)
		return true;

	//Check to see if the entity is inside of a objects colliding point
	return rect.intersects(boundingBox);
}