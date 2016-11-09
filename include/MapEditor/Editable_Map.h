#ifndef Editable_Map_H
#define Editable_Map_H

#include "Game\Map\Map.h"
#include "PBE\System\Camera.h"
#include "PBE\Graphics\Graphic_Manager.h"

class Editable_Map : public Map
{
public:
	//PUBLIC FUNCTIONS
	Editable_Map();
	Editable_Map(pb::Graphic_Manager *tilePaneManager, const sf::Vector2i& tileSheetCoords);

	void updateDrawList(sf::RenderWindow *window, pb::Graphic_Manager *graphicManager, const pb::Camera& camera, const sf::Vector2f& mouseCoords, const sf::Time& currentTime, const pb::Time& inGameTime);
	void updateTileSheet(const sf::RenderWindow& window, pb::Graphic_Manager *tilePaneManager, const sf::Vector2f& mousePos, pb::In_Game_Clock& gameClock);

	void loadMap(std::string mapName, pb::Graphic_Manager* graphicManager, pb::Camera* camera);
	void createMap(unsigned int rows, unsigned int columns, std::string mapName, std::string sheetFileName, pb::Graphic_Manager* graphicManager, pb::Camera* camera);
	void setTile(const sf::Vector2i& mouseCoords);
	void addTileToPos(sf::RenderWindow *window, const pb::Time& t);
	void saveMap();

	static unsigned short getTileSize();

	bool isMapLoaded();
	bool inMapBounds(const sf::Vector2f& mouseCoords);

	std::string getMapName();

	unsigned int getRows();
	unsigned int getColumns();

private:
	struct TransitionPoint
	{
		std::string transitionMapName;
		sf::Vector2i startingCoords;
		int numCoords = 0;
		std::vector<sf::Vector2i> transitionPoints;
	};

	struct Selected_Tile
	{
		std::string specialTile = "None";
		int pos = -1;

		void setSpecialTile(const std::string& tile)
		{
			pos = -1;
			specialTile = tile;
		}
	};

	// Private methods for initializing a map
	void initialize(std::ifstream& mapFile, pb::Graphic_Manager* graphicManager, pb::Camera* camera);
	void initializeTransitionPoints(std::ifstream& mapFile);
	void initializeTileSheetCoords(const sf::Vector2i& tileSheetCoords);

	void addTileToMap(Tile_Data& tD, unsigned int row, unsigned int column);

	void deleteTileFromPos(int row, int column);
	void deleteTransitionPoint(int row, int column);
	void rotateTile(int row, int column);
	void mirrorTileAtPos(int row, int column);
	void setTransitionPoint(int row, int column, sf::RenderWindow *window);
	void testMap(unsigned int row, unsigned int column, const pb::Time& t);

	// Private constants
	const unsigned int NUM_TILES_IN_SHEET = 200;

	// Private variables
	sf::Text currentRowColumn, currentTime;
	sf::Font font;
	sf::Sprite tileSheetSprite;
	sf::RectangleShape mousePos, selectedTile, deleteTile, transitionTile, rotationTile, mirrorTile, deleteTransTile, testMapTile;

	int numTransitionPoints = 0;
	bool mapLoaded = false;
	sf::Vector2i tileSheetCoords;
	Selected_Tile currentTile;
	std::string nameOfFile = "NULL", nameOfTileSheet = "NULL", nameOfSheetFile = "NULL"; 
	std::vector<TransitionPoint> transitions; //Vector containing information about all transition points in a map

};

#endif