#ifndef Editable_Map_H
#define Editable_Map_H

#include "Game\Map.h"

class Editable_Map : public Map
{
public:
	//PUBLIC FUNCTIONS
	Editable_Map();

	void updateDrawList(sf::RenderWindow *window, sf::Vector2f mouseCoords);

	void loadMap(std::string mapName);
	void initializeTileSheetCoords(sf::RenderWindow* window);
	void createMap(unsigned int rows, unsigned int columns, std::string mapName, std::string sheetFileName);
	void setTile(sf::Vector2i mouseCoords);
	void addTileToPos();
	void saveMap();
	void allowTileManipulation();
	void forceUpdate();

	void createGrid(); // Remove?

	static unsigned short getTileSize();

	bool isMapLoaded();

	std::string getMapName();

	int getRows();
	int getColumns();

private:
	struct TransitionPoint
	{
		std::string transitionMapName;
		sf::Vector2i startingCoords;
		int numCoords = 0;
		std::vector<sf::Vector2i> transitionPoints;
	};

	// Private methods for initializing a map
	void initialize(std::ifstream& mapFile);
	void initializeTransitionPoints(std::ifstream& mapFile);

	// Private methods
	void drawTileSheet(sf::RenderWindow *window, sf::Vector2f mousePos);

	void updateMap(sf::RenderTexture& texture, Tile**& layer);
	void deleteTileFromPos(int row, int column);
	void deleteTransitionPoint(int row, int column);
	void rotateTile(int row, int column);
	void mirrorTileAtPos(int row, int column);
	void setTransitionPoint(int row, int column);

	bool isSameTile(Tile**& layer, int row, int column);

	int boolToString(bool b);

	std::string tileToString(Tile**& layer, int row, int column);

	// Private constants
	const unsigned int NUM_TILES_IN_SHEET = 200;

	// Private variables
	sf::Text currentRowColumn;
	sf::Font font;
	sf::Sprite tileSheetSprite;
	sf::RectangleShape mousePos, selectedTile, deleteTile, transitionTile, rotationTile, mirrorTile, deleteTransTile;

	int numTransitionPoints = 0;
	bool mapLoaded = false, tileRotatedRecently = false, tileMirroredRecently = false, tileDeletedRecently = false, transitionPlacedRecently = false, transitionRemovedRecently = false;
	sf::Vector2i tileSheetCoords;
	std::string tileData[200];
	std::string currentTile = "No Tile", nameOfFile = "NULL", nameOfTileSheet = "NULL", nameOfSheetFile = "NULL"; //Options are tile data, "No Tile", "Rotate", "Transition", "Mirror", "DeleteTransition", and "Delete"
	std::vector<TransitionPoint> transitions; //Vector containing information about all transition points in a map

};

#endif