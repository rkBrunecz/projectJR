#ifndef Layer_H
#define Layer_H

#include "SFML\Graphics\Drawable.hpp"
#include "SFML\Graphics\Transformable.hpp"
#include "SFML\Graphics\VertexArray.hpp"
#include "PBE\Graphics\Graphic_Manager.h"
#include "Tile.h"

class Layer : public sf::Drawable, public sf::Transformable
{
public:
	// Constructors
	Layer(const std::string& tileset, unsigned int numRows, unsigned int numColumns, pb::Graphic_Manager *manager);

	// Destructor
	~Layer();

	void update(unsigned int tileSize, unsigned int row, unsigned int column, const sf::Time& currentTime, pb::Graphic_Manager& graphicManager);

	void clearVertexArray();

	Tile *getTile(unsigned int row, unsigned int column);

	void addTile(const Tile& t, unsigned int row, unsigned int column);

	void addTile(const Animated_Tile& t, unsigned int row, unsigned int column);

	void removeTile(unsigned int row, unsigned int column);

	void addTransitionPoint(unsigned int row, unsigned int column, const std::string& transitionMapName, const sf::Vector2f& coords);

	void removeTransitionPoint(unsigned int row, unsigned int column);

	void changeTileCollision(unsigned int row, unsigned int column, bool collidable);

	bool isColliding(const sf::IntRect& rect, unsigned int row, unsigned int column, unsigned int tileSize);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void updateVerticies(unsigned int row, unsigned int column, unsigned int tileSize, const Tile& t);

	void rotateTile(sf::Vertex *quad, unsigned int tileSize, const Tile& t);

	void mirrorTile(sf::Vertex *quad, unsigned int tileSize, const Tile& t);

	const sf::Texture *tileset;
	std::vector<Tile *>layer;
	sf::VertexArray verticies;

	unsigned int numRows, numColumns;
};

#endif