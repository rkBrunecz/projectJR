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

//CONSTANTS
const int graphicArraySize = 1;
const enum Game_States //Dictates the state the game is in
{
	Play,
	Battle,
	Pause,
	MainMenu,
	GameMenu,
	Quit
};

//GLOBAL VARIABLES
Graphic* graphics[graphicArraySize];
sf::Clock gameClock;

/*
runGame
Parameters:
	window - The game window where graphics are drawn
	state  - The state the game is currently in

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
		graphics[0]->updatePosition(&window, gameClock.getElapsedTime().asSeconds());
		graphics[0]->draw(&window);

		break;

	case Quit:
		return; //If the game state is set to Quit, return to the game loop and close down the game.
		
		break;
	}

	//Redisplay everything in the window
	window.display();
}

int main()
{
	//LOCAL VARIABLES
	Game_States state = Play; //Set the starting game state
	Player player;

	//Create a fullscreen window with same pixel depth (a.k.a bit depth/color depth) as the desktop
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "Project JR", sf::Style::Fullscreen);
	window.setFramerateLimit(60); //Set the framerate to 60

	graphics[0] = &player;

	//GAME LOOP
	while (state != Quit)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				state = Quit;
		}

		runGame(window, state); //Run the game based on the current game state

		gameClock.restart();
	}

	window.close(); //Close the game window

	return 0; //Close the game
}