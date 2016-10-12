/*
main.cpp
The main.cpp file is the entry point for the game.

@author Randall Brunecz
@version 1.0 8/31/2015

@version 1.1 9/9/2016
	- Added battle system to game

@version 1.2 9/20/2016
	- Moved all game code to a game class
*/

// Headers
//#include <vld.h> // Check for memory leaks. Comment out if not being used
#include "Game\Game.h"

std::vector<int> v;

int main(int argc, char* argv[])
{
	// Local variables
	Game *game = 0;
	std::string version = "v.1.2a"; // Version number

	if (argc == 4)
		game = new Game(version, argc, argv);
	else
		game = new Game(version);

	// Run game
	game->runGame();

	// CLEAN UP MEMORY
	delete game;

	return 0; //Close the game
}