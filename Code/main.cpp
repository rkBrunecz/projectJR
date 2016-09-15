/*
main.cpp
The main.cpp file is the entry point for the game. This is where the window is created
and displays the graphics for the game. The main.cpp file contains a state variable which
keeps track of the games current state and makes decisions based on the state the game is
in.

@author Randall Brunecz
@version 1.0 8/31/2015

@version 2.0 9/9/2016
	- Added battle system to game
*/

//#include <vld.h> // Check for memory leaks

#include <SFML\Graphics.hpp>
#include "Player.h"
#include "Map.h"
#include "Camera.h"
#include "Collision.h"
#include "Battle_Engine.h"
#include "Player.h"
#include <iostream>

//CONSTANTS
const unsigned short GRAPHICS_ARRAY_SIZE = 2;

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
	Quit
};
const enum Window_States //Determines the state the window is in
{
	Fullscreen,
	Windowed
};

//GLOBAL VARIABLES
Graphic* graphics[GRAPHICS_ARRAY_SIZE];
Game_States state = Play, returnState = Play;

/*
runGame
Parameters:
	window     - The game window where graphics are drawn
	state      - The state the game is currently in
	mapTexture - The texture that stores the map for drawing.
	map        - This object contains information about the map

This method is where all game related work is done.
*/
void runGame(sf::RenderWindow& window, float elapsedTime, Map& map, Battle_Engine& battle, Player& player, sf::Event& lastKeyPressed)
{
	//Clear the window
	window.clear();

	//Perform game actions based on the current game state.
	switch (state)
	{
	case Play:
	{
		//Update positions
		player.updatePosition(elapsedTime);

		window.setView(Camera::getCamera()); //Update the windows view

		//Draw all graphics
		map.updateDrawList(&player, true);

		//Check to see if a map transition is needed
		if (map.transitioning(&player))
			state = Transition;

		break;

	}
	case Battle:
	{
		battle.startTurn(lastKeyPressed, elapsedTime);

		Camera::animateCamera();
		window.setView(Camera::getCamera());

		battle.updateDrawList(true);

		lastKeyPressed.key.code = sf::Keyboard::Unknown;

		break;
	}
	case Pause:
	{
		//Draw all graphics
		if (returnState == Play)
			map.updateDrawList(&player, false);
		else
			battle.updateDrawList(false);

		break;
	}
	case Transition:
	{
		while (Graphic::fadingOut(&window))
			map.updateDrawList(&player, true);
			
		map.moveToMap(&player); //Transition to the next map

		window.setView(Camera::getCamera()); //Update the window view

		while (Graphic::fadingIn(&window))
			map.updateDrawList(&player, true);

		state = Play; //Continue playing the game
		returnState = Play;

		break;
	}
	case InitiateBattle:
	{
		do {
			map.updateDrawList(&player, true);
		} while (Graphic::fadingOut(&window));

		// Clear graphics and audio out and replace with game data
		Graphic::clearTextureList();
		Audio_Engine::clearSoundList();

		Camera::setCameraState(Camera::Battle);
		Camera::initialize(window.getSize().x, window.getSize().y);
		window.setView(Camera::getCamera());

		player.changePlayerState();

		Battle_Object* players[1];
		players[0] = &player;

		battle.initialize(players, 1, Camera::getCamera().getSize().x, Camera::getCamera().getSize().y);
		
		do {
			battle.updateDrawList(true);
		} while (Graphic::fadingIn(&window));

		Graphic::enableDayShift(false);

		state = Battle;
		returnState = Battle;

		break;
	}
	case InitiateOverworld:
	{
		do {
			battle.updateDrawList(true);
		} while (Graphic::fadingOut(&window));

		Graphic::clearTextureList();
		Audio_Engine::clearSoundList();

		Camera::setCameraState(Camera::Overworld);

		map.loadMap(); //Load the map

		player.changePlayerState();
		player.initialize();

		window.setView(Camera::getCamera());

		do {
			map.updateDrawList(&player, true);
		} while ((Graphic::fadingIn(&window)));

		Graphic::enableDayShift(true);

		state = Play;
		returnState = Play;

		break;
	}
	case Quit:
		return; //If the game state is set to Quit, return to the game loop and close down the game.
		
		break;
	}

	Graphic::draw(&window);
	if (state == Pause)
		Graphic::dimScreen(&window);

	//Redisplay everything in the window
	window.display();
}

/*
populateGraphicsArray
Parameters:
	window: This is the game window. Used to determine how to place certain graphics in relation to the windows resolution.

This method simply creates and poplulates the graphics array
*/
void populateGraphicsArray(Player* player, Map* map)
{
	graphics[0] = map;
	graphics[1] = player;

	Collision::intializeGraphicObjects(graphics, GRAPHICS_ARRAY_SIZE);
}

void game(int argc, char* argv[])
{
	//LOCAL VARIABLES
	Window_States windowState = Fullscreen; //Set window state to fullscreen
	sf::Clock clock; // Used to obtain the time between frame updates. This ensures updates to the game is consistent across different hardware.
	float elapsedTime; // In seconds
	bool verticalSyncEnabled = true;

	Map map;
	Battle_Engine battle;
	Player player;
	sf::Event lastKeyPressed;
	std::string mapName = "res/Maps/TestMap.jrm";

	//Create a fullscreen window with same pixel depth (a.k.a bit depth/color depth) as the desktop
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "Project JR", sf::Style::Fullscreen);

	// Set up properties of the window
	window.setVerticalSyncEnabled(verticalSyncEnabled);
	window.setMouseCursorVisible(false);
	window.setKeyRepeatEnabled(false);

	sf::Vector2f startPos(6 * 32, 6 * 32);

	if (argc == 4)
	{
		mapName = argv[1];
		startPos.y = (float)atoi(argv[2]) * 32 + 16;
		startPos.x = (float)atoi(argv[3]) * 32 + 16;
	}

	player.setPlayerPosition(startPos); //TESTING START SPOT. WILL CHANGE

	lastKeyPressed.key.code = sf::Keyboard::Unknown;

	//Set up camera properties
	Camera::setSize((float)desktop.width, (float)desktop.height);
	Camera::zoom(0.5);

	populateGraphicsArray(&player, &map); //Populate the graphics array

	player.initialize();

	map.loadMap(mapName);
	
	// GAME LOOP
	while (state != Quit)
	{
		// Get elapsed time since last update
		elapsedTime = clock.restart().asSeconds();;

		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				state = Quit;
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) //Pause the game if the spacebar is pressed
			{
				if (state == Play || state == Battle)
				{
					state = Pause;
					Graphic::enableDayShift(false);
				}
				else
				{
					state = returnState;

					if (returnState != Battle)
						Graphic::enableDayShift(true);
				}
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
				map.displayCollsionLayer();
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G)
				map.displayGridLayer();
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T)
				map.displayTransitionLayer();
			//Resize window if the F11 key is pressed
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F11)
			{
				window.close(); //Close the current window

				//Create a window in Windowed mode
				if (windowState == Fullscreen)
				{
					window.create(sf::VideoMode(desktop.width - 150, desktop.height - 150, desktop.bitsPerPixel), "Project JR", sf::Style::Titlebar);
					windowState = Windowed;
				}
				else //Create a window in fullscreen mode
				{
					window.create(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "Project JR", sf::Style::Fullscreen);
					windowState = Fullscreen;
				}

				window.setVerticalSyncEnabled(true);
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B)
			{
				if (state == Play)
					state = InitiateBattle;
				else
					state = InitiateOverworld;
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V)
			{
				verticalSyncEnabled = !verticalSyncEnabled;

				window.setVerticalSyncEnabled(verticalSyncEnabled);
			}
			//If the window loses focus, pause the game
			else if (event.type == sf::Event::LostFocus)
				state = Pause;
			//When the window regains focus, resume the game
			else if (event.type == sf::Event::GainedFocus)
				state = returnState;

			//Store the last key pressed if in the battle state
			if (state == Battle && event.type == sf::Event::KeyPressed)
				lastKeyPressed = event;
		}

		runGame(window, elapsedTime, map, battle, player, lastKeyPressed); //Run the game based on the current game state
	}

	window.close(); //Close the game window
}

int main(int argc, char* argv[])
{
	game(argc, argv);

	// CLEAN UP MEMORY
	Audio_Engine::clearSoundList();
	Graphic::clearTextureList();

	return 0; //Close the game
}