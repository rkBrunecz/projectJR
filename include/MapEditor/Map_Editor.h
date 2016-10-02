#ifndef Map_Editor_H
#define Map_Editor_H

// Headers
#include <string>
#include "SFML\System\NonCopyable.hpp"
#include "PBE\System\Camera.h"
#include "PBE\Graphics\Graphic_Manager.h"
#include "PBE\System\In_Game_Clock.h"
#include "Editable_Map.h"
#include "UI.h"

class Map_Editor : public sf::NonCopyable
{
public:
	// Static variables
	static pb::Camera *camera;
	static pb::Graphic_Manager *graphicManager, *tilePaneManager;

	// Constructor
	Map_Editor(const std::string versionNum);

	// Destructor
	~Map_Editor();

	void runEditor();

private:
	void processEvents();
	void update();
	void render();

	bool displayCursor();
	void moveCamera();

	// Private constants
	const enum Editor_States 
	{
		Build,
		New,
		Load,
		Save,
		TestMap,
		ForceUpdate
	};

	// Private variables
	float zoomFactor = 1;

	Editor_States state = Build;
	sf::Vector2i rightButtonPos;

	std::string versionNumber;
	sf::RenderWindow *window = 0;
	sf::VideoMode desktop;

	pb::In_Game_Clock *gameClock;

	Editable_Map *map = 0;

	sf::RectangleShape menuBar, tilePane;
};

#endif