/*
Authors: Randy Brunecz, Joshua Gray
main.cpp
8/31/2015

The main.cpp file is the entry point for the game. This is where the window is created
and displays the graphics for the game. The main.cpp file contains a state variable which
keeps track of the games current state and makes decisions based on the state the game is
in.
*/

#include <SFML\Graphics.hpp>
#include "Player.h"
#include "SpecialEffect.h"
#include "Map.h"

//CONSTANTS
const int GRAPHICS_ARRAY_SIZE = 2;
const enum Game_States //Dictates the state the game is in
{
	Play,
	Battle,
	Pause,
	MainMenu,
	GameMenu,
	Quit
};
const enum Window_States //Determines the state the window is in
{
	Fullscreen,
	Windowed
};

//GLOBAL VARIABLES
Graphic* graphics[GRAPHICS_ARRAY_SIZE]; //Contains pointers to all graphic objects in the game

/*
runGame
Parameters:
	window - The game window where graphics are drawn
	state  - The state the game is currently in
	dT	   - This is the delta time, or time elapsed, since the last update

This method is where all game related work is done.
*/
void runGame(sf::RenderWindow& window, Game_States& state)
{
	//Clear the window
	window.clear();

	//Perform game actions based on the current game state.
	switch (state)
	{
	case Play:
		//Update the position and draw all graphics
		for (int i = 0; i < GRAPHICS_ARRAY_SIZE; i++)
		{
			graphics[i]->updatePosition(&window);
			graphics[i]->draw(&window);
		}

		break;

	case Pause:
		SpecialEffect::screenDim(graphics, GRAPHICS_ARRAY_SIZE);

		for (int i = 0; i < GRAPHICS_ARRAY_SIZE; i++)
			graphics[i]->draw(&window);

		break;

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
void populateGraphicsArray(sf::RenderWindow& window)
{
	graphics[0] = new Map;
	graphics[1] = new Player(&window);
}

int main()
{
	//LOCAL VARIABLES
	Game_States state = Play; //Set the starting game state
	Window_States windowState = Fullscreen; //Set window state to fullscreen

	//Create a fullscreen window with same pixel depth (a.k.a bit depth/color depth) as the desktop
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "Project JR", sf::Style::Fullscreen);
	window.setFramerateLimit(60);
	
	populateGraphicsArray(window); //Populate the graphics array

	//GAME LOOP
	while (state != Quit)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				state = Quit;
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1) //DEBUG TEST FADE IN
				SpecialEffect::fadeIn(&window, graphics, GRAPHICS_ARRAY_SIZE);
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2) //DEBUG TEST FADE OUT
				SpecialEffect::fadeOut(&window, graphics, GRAPHICS_ARRAY_SIZE);
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				if (state == Play)
					state = Pause;
				else
				{
					state = Play;
					SpecialEffect::resetScreenDim(graphics, GRAPHICS_ARRAY_SIZE);
				}
			}
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
				
				window.setFramerateLimit(60); //Set the framerate to 60
			}
			//If the window loses focus, pause the game
			else if (event.type == sf::Event::LostFocus)
				state = Pause;			
			//When the window regains focus, resume the game
			else if (event.type == sf::Event::GainedFocus)
			{
				SpecialEffect::resetScreenDim(graphics, GRAPHICS_ARRAY_SIZE);
				state = Play;
			}
		}

		runGame(window, state); //Run the game based on the current game state
	}

	//Free up the memory used by the graphics library
	for (int i = 0; i < GRAPHICS_ARRAY_SIZE; i++)
		delete	graphics[i];

	window.close(); //Close the game window

	return 0; //Close the game
}