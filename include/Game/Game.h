#ifndef Game_H
#define Game_H

#include <SFML\OpenGL.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>
#include "PBE\System\Camera.h"
#include "PBE\Graphics\Graphic_Manager.h"
#include "PBE\Audio\Audio_Manager.h"
#include "PBE\System\In_Game_Clock.h"
#include "Game\Battle\Battle_Engine.h"
#include "Game\Map\Map.h"
#include "Player.h"


class Game : sf::NonCopyable
{
public:
	// Static variables
	static pb::Camera *camera;
	static pb::Graphic_Manager *graphicManager;
	static pb::Audio_Manager *audioManager;

	// Constructors
	Game(const std::string versionNum);
	Game(std::string versionNum, int argc, char* argv[]);

	// Destructor
	~Game();

	void runGame();

private:
	// Private methods
	void initialize();
	void processEvents();
	void update();
	void render(double alpha);

	// Private Constants
	const enum Game_States //Dictates the state the game is in
	{
		Play,
		Battle,
		Pause,
		MainMenu,
		GameMenu,
		Transition,
		InitiateBattle,
		InitiateOverworld,
		Fading
	};
	const enum Window_States //Determines the state the window is in
	{
		Fullscreen,
		Windowed
	};

	// Private variables
	Game_States state = Play, returnState = Play;
	Window_States windowState = Fullscreen; //Set window state to fullscreen

	float zoomLevelWorld = 0.5f;

	std::string versionNumber;
	sf::RenderWindow *window;
	sf::VideoMode desktop;
	sf::Event lastKeyPressed;
	
	// Map holds the map, and currentMap keeps the maps file name on hand incase it needs to be reloaded
	Map *map = 0;
	std::string currentMap = "res/Maps/TestMap.jrm"; // DEBUG VALUE. This will be changed
	
	Battle_Engine *battle = 0;
	Player *player = 0;

	// In game clock
	pb::In_Game_Clock *gameClock;

	// Used to track the elapsed time between frames
	const float dt = 0.01f; // In seconds
	sf::Time currentTime;

	// Tracks if V-Sync is available
	bool verticalSyncEnabled = true;
};

#endif