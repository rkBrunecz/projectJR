#include "Game\Game.h"

// Global variables
pb::Camera *Game::camera;
pb::Graphic_Manager *Game::graphicManager;
pb::Audio_Manager *Game::audioManager;

Game::Game(const std::string versionNumber)
{
	// Create a fullscreen window with same pixel depth (a.k.a bit depth/color depth) as the desktop
	desktop = sf::VideoMode::getDesktopMode();

	// Set up window properties
	window = new sf::RenderWindow(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "Project JR " + versionNumber, sf::Style::Fullscreen);
	window->setVerticalSyncEnabled(verticalSyncEnabled);
	window->setMouseCursorVisible(false);
	window->setKeyRepeatEnabled(false);

	// Initialize camera, graphic manager, and audio manager
	camera = new pb::Camera(desktop.width, desktop.height, 0.5f);

	// Intialize a game clock with some default values
	gameClock = new pb::In_Game_Clock(2, 8, 0, 24, 8, 8, 4, 4);

	// Initialize graphic manager
	graphicManager = new pb::Graphic_Manager(*gameClock);

	// Initialize audio manager
	audioManager = new pb::Audio_Manager();

	// Initialize a player object
	player = new Player();
}

Game::Game(std::string versionNumber, int argc, char* argv[]) : Game(versionNumber)
{
	// Local variables
	float x = 0, y = 0;
	
	if (argc == 4)
	{
		currentMap = argv[1];
		y = (float)atoi(argv[2]) * 32 + 16;
		x = (float)atoi(argv[3]) * 32 + 16;
	}

	player->setPosition(sf::Vector2f(x, y));
}

Game::~Game()
{
	// Free memory
	delete window;
	delete battle;
	delete map;
	delete player;
	delete gameClock;

	// Free static variables
	delete camera;
	delete graphicManager;
	delete audioManager;
}

void Game::initialize()
{
	map = new Map(currentMap);

	// Default player position
	player->setPosition(sf::Vector2f(6 * 32, 6 * 32));
}

void Game::processEvents()
{
	// Handle events
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			window->close();
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) //Pause the game if the spacebar is pressed
		{
			if (state == Play || state == Battle)
			{
				state = Pause;
				graphicManager->enableDayShift(false);
			}
			else
			{
				state = returnState;

				if (returnState != Battle)
					graphicManager->enableDayShift(true);
			}
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num4) // DEBUG fade in
			graphicManager->fadeIn(sf::Color::Black, 0.020f);
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num3) // DEBUG fade out
			graphicManager->fadeOut(sf::Color::Black, 0.020f);
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab)
			map->displayCollsionLayer();
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G)
			map->displayGridLayer();
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T)
			map->displayTransitionLayer();
		//Resize window if the F11 key is pressed
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F11)
		{
			window->close(); //Close the current window

			//Create a window in Windowed mode
			if (windowState == Fullscreen)
			{
				window->create(sf::VideoMode(desktop.width - (desktop.width / 4), desktop.height - (desktop.height / 4), desktop.bitsPerPixel), "Project JR " + versionNumber, sf::Style::Titlebar);
				windowState = Windowed;
			}
			else //Create a window in fullscreen mode
			{
				window->create(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "Project JR " + versionNumber, sf::Style::Fullscreen);
				windowState = Fullscreen;
			}

			window->setVerticalSyncEnabled(true);
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B)
		{
			graphicManager->fadeOut(sf::Color::Black, 0.020f);

			if (state == Play)
				state = InitiateBattle;
			else
				state = InitiateOverworld;
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::V)
		{
			verticalSyncEnabled = !verticalSyncEnabled;

			window->setVerticalSyncEnabled(verticalSyncEnabled);
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
}

void Game::update()
{
	//Perform game actions based on the current game state.
	switch (state)
	{
	case Play:
	{
		// Wait for the fade out to finish
		if (!graphicManager->effectFinished())
		{
			map->updateDrawList(player, false);
			break;
		}

		//Update positions
		player->updatePosition(elapsedTime);
		gameClock->updateClock();

		//Draw all graphics
		map->updateDrawList(player, true);

		//Check to see if a map transition is needed
		if (map->transitioning(player))
		{
			graphicManager->fadeOut(sf::Color::Black, 0.020f); // Start a fade out
			state = Transition;
		}

		break;

	}
	case Battle:
	{
		// Wait for the fade out to finish
		if (!graphicManager->effectFinished())
		{
			battle->updateDrawList(false);
			break;
		}

		battle->startTurn(lastKeyPressed, elapsedTime);

		battle->updateDrawList(true);

		lastKeyPressed.key.code = sf::Keyboard::Unknown;

		break;
	}
	case Pause:
	{
		//Draw all graphics
		if (returnState == Play)
			map->updateDrawList(player, false);
		else
			battle->updateDrawList(false);

		// Pause the day shift and dim the screen
		graphicManager->enableDayShift(false);
		graphicManager->dimScreen(sf::Color::Black, 130);

		break;
	}
	case Transition:
	{
		// Wait for the fade out finish
		if (!graphicManager->effectFinished())
		{
			map->updateDrawList(player, false);
			break;
		}

		currentMap = map->moveToMap(player); //Transition to the next map

		state = Play; //Continue playing the game
		returnState = Play;

		graphicManager->fadeIn(sf::Color::Black, false); // Fade back in after the transition finished

		break;
	}
	case InitiateBattle:
	{
		// Wait for the fade out finish
		graphicManager->enableDayShift(false);
		if (!graphicManager->effectFinished())
		{
			map->updateDrawList(player, false);
			break;
		}

		// Clear graphics and audio out and replace with game data
		graphicManager->clearTextureList();
		audioManager->clearSoundList();

		// Delete previous game camera and create the battle camera
		delete camera;
		camera = new pb::Camera(window->getSize().x, window->getSize().y, 0.5f, "screenshake.wav");

		player->changePlayerState(Player::Battle); // change player state

		// Intialize a list of players to send to the battle engine
		Battle_Object* players[1];
		players[0] = player;

		// Instantiate a battle object
		battle = new Battle_Engine(players, 1, camera->getCamera().getSize().x, camera->getCamera().getSize().y);

		// Change game state
		state = Battle;
		returnState = Battle;

		graphicManager->fadeIn(sf::Color::Black, 0.020f); // Fade in

		break;
	}
	case InitiateOverworld:
	{
		// Wait for the fade in to finish
		graphicManager->enableDayShift(false);
		if (!graphicManager->effectFinished())
		{
			battle->updateDrawList(true);
			break;
		}

		// Clear textures and audio
		graphicManager->clearTextureList();
		audioManager->clearSoundList();

		// Free memory associated with battle
		delete battle;
		battle = 0; // 0 out battle to avoid double deletes

		// Delete previous game camera and create the world camera
		delete camera;
		camera = new pb::Camera(window->getSize().x, window->getSize().y, 0.5f);

		map->loadMap(currentMap); //Load the map

		player->changePlayerState(Player::World); // change player state

		graphicManager->enableDayShift(true); // re-enable day shift

		// Change states
		state = Play;
		returnState = Play;

		graphicManager->fadeIn(sf::Color::Black, 0.020f); // Fade in

		break;
	}
	}
}

void Game::render()
{
	//Clear the window
	window->clear();

	camera->animateCamera(); // Apply any camera animations
	window->setView(camera->getCamera()); //Update the windows view
	
	graphicManager->draw(window, gameClock->getTime()); // Use day/night version of graphic manager draw

	//Redisplay everything in the window
	window->display();
}

void Game::runGame()
{
	initialize();

	// GAME LOOP
	while (window->isOpen())
	{
		// Get elapsed time since last update
		elapsedTime = clock.restart().asSeconds();

		processEvents();

		update();

		render();
	}
}