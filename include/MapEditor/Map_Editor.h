#ifndef Map_Editor_H
#define Map_Editor_H

// Headers
#include <string>
#include "SFML\System\NonCopyable.hpp"
#include "PBE\System\Camera.h"
#include "PBE\Graphics\Graphic_Manager.h"
#include "PBE\System\In_Game_Clock.h"
#include "Menu_Bar.h"
#include "Editable_Map.h"
#include "UI.h"

class Map_Editor : public sf::NonCopyable
{
public:
	// Constructor
	Map_Editor(const std::string& versionNum);
	Map_Editor(const std::string& title, const std::string& mapToLoad);

	// Destructor
	~Map_Editor();

	void runEditor();

	sf::Vector2i runForResult();

private:
	// Public constants
	const enum Editor_States
	{
		Build,
		New,
		Load,
		Save
	};

	void processEvents();
	void update();
	void render();

	void parseMenuBarString(const std::string& s);

	bool displayCursor();
	void moveCamera();

	// Private variables
	float zoomFactor = 0.5f;

	Editor_States state = Build;
	sf::Vector2i rightButtonPos;

	std::string versionNumber;
	sf::RenderWindow *window = 0;
	sf::VideoMode desktop;

	pb::In_Game_Clock *gameClock;

	Editable_Map *map = 0;

	sf::Clock clock;
	sf::Time currentTime;

	Menu_Bar *menuBar;
	sf::RectangleShape tilePane;

	pb::Camera *camera = 0;
	pb::Graphic_Manager *graphicManager = 0, *tilePaneManager = 0;

	bool leftMouseBtnClicked = false;
};

#endif