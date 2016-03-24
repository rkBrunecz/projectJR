/*
main.cpp
The main.cpp file is the entry point for the game. This is where the window is created
and displays the graphics for the game. The main.cpp file contains a state variable which
keeps track of the games current state and makes decisions based on the state the game is
in.

@author Randall Brunecz
@version 1.0 8/31/2015
*/

#include <SFML\Graphics.hpp>
#include "Player.h"
#include "SpecialEffect.h"
#include "Map.h"
#include "Camera.h"
#include "Collision.h"
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
	Quit
};
const enum Window_States //Determines the state the window is in
{
	Fullscreen,
	Windowed
};

//GLOBAL VARIABLES
Graphic* graphics[GRAPHICS_ARRAY_SIZE];
Game_States state = Play;

/*
runGame
Parameters:
	window     - The game window where graphics are drawn
	state      - The state the game is currently in
	mapTexture - The texture that stores the map for drawing.
	map        - This object contains information about the map

This method is where all game related work is done.
*/
void runGame(sf::RenderWindow& window, Camera& camera, Map& map, Player& player)
{
	//Clear the window
	window.clear();

	//Perform game actions based on the current game state.
	switch (state)
	{
	case Play:
	{
		//Update positions
		player.updatePosition(&window, &camera);

		window.setView(camera); //Update the windows view

		//Draw all graphics
		map.draw(&window, &player, true);

		//Check to see if a map transition is needed
		if (map.transitioning(&player))
			state = Transition;

		break;

	}
	case Pause:
	{
		//Draw all graphics
		map.draw(&window, &player, false);

		SpecialEffect::screenDim(&window);

		break;
	}
	case Transition:
	{
		SpecialEffect::fadeOut(&window, &map, &player);

		map.moveToMap(&player, &camera); //Transition to the next map

		window.setView(camera); //Update the window view

		SpecialEffect::fadeIn(&window, &map, &player);

		state = Play; //Continue playing the game
	}
	case Quit:
		return; //If the game state is set to Quit, return to the game loop and close down the game.
		
		break;
	}

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

int main()
{
	//LOCAL VARIABLES
	Window_States windowState = Fullscreen; //Set window state to fullscreen
	Camera camera;
	Map map;

	//Create a fullscreen window with same pixel depth (a.k.a bit depth/color depth) as the desktop
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "Project JR", sf::Style::Fullscreen);

	Player player(&camera);
	player.setPlayerPosition(sf::Vector2i( 6 * 32, 6 * 32)); //TESTING START SPOT. WILL CHANGE

	//Set up camera properties
	camera.setSize(desktop.width, desktop.height);
	camera.zoom(0.6);

	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);
	
	populateGraphicsArray(&player, &map); //Populate the graphics array

	map.loadMap("bin/Maps/TestMap.jrm", &camera); //Load the map

	//GAME LOOP
	while (state != Quit)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				state = Quit;
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1) //DEBUG TEST FADE IN
				SpecialEffect::fadeIn(&window, &map, &player);
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2) //DEBUG TEST FADE OUT
				SpecialEffect::fadeOut(&window, &map, &player);
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) //Pause the game if the spacebar is pressed
			{
				if (state == Play)
					state = Pause;
				else
					state = Play;
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
			//If the window loses focus, pause the game
			else if (event.type == sf::Event::LostFocus)
				state = Pause;			
			//When the window regains focus, resume the game
			else if (event.type == sf::Event::GainedFocus)
				state = Play;
		}

		runGame(window, camera, map, player); //Run the game based on the current game state
	}

	window.close(); //Close the game window

	return 0; //Close the game
}