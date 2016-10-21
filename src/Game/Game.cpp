#include "Game\Game.h"

// Global variables
pb::Camera *Game::camera;
pb::Graphic_Manager *Game::graphicManager;
pb::Audio_Manager *Game::audioManager;

Game::Game(const std::string versionNumber)
{
	// Create a fullscreen window with same pixel depth (a.k.a bit depth/color depth) as the desktop
	desktop = sf::VideoMode::getDesktopMode();

	// Set version number
	this->versionNumber = versionNumber;

	// Set up window properties
	window = new sf::RenderWindow(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), "Project JR " + versionNumber, sf::Style::Fullscreen);
	window->setVerticalSyncEnabled(verticalSyncEnabled);
	window->setMouseCursorVisible(false);
	window->setKeyRepeatEnabled(false);

	// Initialize camera, graphic manager, and audio manager
	camera = new pb::Camera(window->getSize().x, window->getSize().y, zoomLevelWorld);

	// Intialize a game clock with some default values
	gameClock = new pb::In_Game_Clock(10, 10, 0, 24, 8, 8, 4, 4);

	// Initialize graphic manager
	graphicManager = new pb::Graphic_Manager(*gameClock);

	// Initialize audio manager
	audioManager = new pb::Audio_Manager();

	// Initialize a player object
	player = new Player();
	player->setPosition(sf::Vector2f(22 * 32, 22 * 32)); // Default player position
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
	map->setLightInterval(sf::Vector2u(22, 5));
}

void Game::processEvents()
{
	// Handle events
	sf::Event event;
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();

			break;

		// Keyboard key is pressed
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				window->close();
			else if (event.key.code == sf::Keyboard::Space) //Pause the game if the spacebar is pressed
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
					{
						gameClock->resume();
						graphicManager->enableDayShift(true);
					}
				}
			}
			else if (event.key.code == sf::Keyboard::Tab)
				map->displayCollsionLayer();
			else if (event.key.code == sf::Keyboard::G)
				map->displayGridLayer();
			else if (event.key.code == sf::Keyboard::T)
				map->displayTransitionLayer();
			//Resize window if the F11 key is pressed
			else if (event.key.code == sf::Keyboard::F11)
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

				if (verticalSyncEnabled && windowState != Windowed)
				{
					window->setVerticalSyncEnabled(verticalSyncEnabled);
					window->setFramerateLimit(0);
				}
				else
					window->setFramerateLimit(60);

				// Recreate the camera
				sf::Vector2f pos = camera->getCenter();
				sf::Vector2f cameraBounds = camera->getCameraBounds();

				// Delete old camera
				delete camera;

				// Initialize new camera
				camera = new pb::Camera(window->getSize().x, window->getSize().y, zoomLevelWorld);
				camera->setBounds(cameraBounds.x, cameraBounds.y);
				camera->setCenter(pos);	

				window->setMouseCursorVisible(false);
				window->setKeyRepeatEnabled(false);
			}
			else if (event.key.code == sf::Keyboard::B)
			{
				if (state == Play)
					returnState = InitiateBattle;
				else
					returnState = InitiateOverworld;

				graphicManager->fadeOut(sf::Color::Black, 0.02f, 15);
				state = Fading;
			}
			else if (event.key.code == sf::Keyboard::V)
			{
				verticalSyncEnabled = !verticalSyncEnabled;
				window->setVerticalSyncEnabled(verticalSyncEnabled);
					
				if (!verticalSyncEnabled)
					window->setFramerateLimit(30); // DEBUG
				else
					window->setFramerateLimit(0);
			}

			// If in a battle, store the last key pressed
			if (state == Battle)
				lastKeyPressed = event;

			break;

		//If the window loses focus, pause the game
		case sf::Event::LostFocus:
			state = Pause;

			break;

		//When the window regains focus, resume the game
		case sf::Event::GainedFocus:
			state = returnState;
			gameClock->resume();

			break;
		}
	}
}

void Game::update()
{
	//Perform game actions based on the current game state.
	switch (state)
	{
	case Play:
	{
		//Update positions
		player->updatePosition(dt);
		gameClock->updateClock();

		//Check to see if a map transition is needed
		if (map->transitioning(player))
		{
			graphicManager->fadeOut(sf::Color::Black, 0.02f, 15); // Start a fade out
			returnState = Transition;
			state = Fading;
		}

		break;
	}
	case Battle:
	{
		battle->startTurn(lastKeyPressed, dt);

		lastKeyPressed.key.code = sf::Keyboard::Unknown;

		break;
	}
	case Pause:
	{
		gameClock->pause();

		// Pause the day shift and dim the screen
		graphicManager->enableDayShift(false);
		graphicManager->dimScreen(sf::Color::Black, 130);

		break;
	}
	case Transition:
	{
		graphicManager->clearTextureList();
		audioManager->clearSoundList();
		player->loadState(Player::World);

		currentMap = map->moveToMap(player); //Transition to the next map

		state = Fading;
		returnState = Play; // Resume game after loading has finished

		graphicManager->fadeIn(sf::Color::Black, 0.02f, 15); // Fade back in after the transition finished

		break;
	}
	case InitiateBattle:
	{
		// Clear graphics and audio out and replace with game data
		graphicManager->clearTextureList();
		audioManager->clearSoundList();

		// Delete previous game camera and create the battle camera
		delete camera;
		camera = new pb::Camera(window->getSize().x, window->getSize().y, 0.5f, "screenshake.wav");

		player->loadState(Player::Battle); // change player state

		// Intialize a list of players to send to the battle engine
		Battle_Object* players[1];
		players[0] = player;

		// Instantiate a battle object
		battle = new Battle_Engine(players, 1, camera->getSize().x, camera->getSize().y);

		// Change game state
		state = Fading;
		returnState = Battle;

		graphicManager->fadeIn(sf::Color::Black, 0.02f, 15); // Start a Fade in

		break;
	}
	case InitiateOverworld:
	{
		// Clear textures and audio
		graphicManager->clearTextureList();
		audioManager->clearSoundList();

		// Free memory associated with battle
		delete battle;
		battle = 0; // 0 out battle to avoid double deletes

		// Delete previous game camera and create the world camera
		delete camera;
		camera = new pb::Camera(window->getSize().x, window->getSize().y, zoomLevelWorld);

		map->loadMap(currentMap); // Load the map

		player->loadState(Player::World); // change player state

		graphicManager->enableDayShift(true); // re-enable day shift

		// Change states
		state = Fading;
		returnState = Play;

		graphicManager->fadeIn(sf::Color::Black, 0.02f, 15); // Start a Fade in

		break;
	}
	case Fading:
		// If fade has finished, return to the proper state
		if (graphicManager->effectFinished())
			state = returnState;

		graphicManager->updateEffect(*window);

		break;
	}
}

void Game::render(double alpha)
{
	switch (state)
	{
	case Play:
		player->renderPosition(alpha);

		//Draw all graphics
		map->updateDrawList(player, currentTime, gameClock->getTime(), true);

		break;

	case Battle:
		battle->updateDrawList(true, alpha);

		break;

	case Pause:
		//Draw all graphics
		if (returnState == Play)
			map->updateDrawList(player, currentTime, gameClock->getTime(), false);
		else
			battle->updateDrawList(false, alpha);

		break;

	case Fading:
		// Add the games previous state to the draw list
		if (returnState == InitiateBattle || returnState == Transition || returnState == Play)
			map->updateDrawList(player, currentTime, gameClock->getTime(), false);
		else if (returnState == InitiateOverworld || returnState == Battle)
			battle->updateDrawList(true, alpha);

		break;
	}

	// Clear the window
	window->clear();

	camera->animateCamera(); // Apply any camera animations
	window->setView(*camera); // Update the windows view

	graphicManager->draw(window, gameClock->getTime()); // Use day/night version of graphic manager draw

	// Redisplay everything in the window
	window->display();
}

/*
runGame

This is where the game loop starts. The game loop for this application is based off of
Glenn Fiedler's game loop. It can be found here: http://gafferongames.com/game-physics/fix-your-timestep/

*/
void Game::runGame()
{
	initialize();

	// Local Variables
	sf::Clock clock;

	double currentTime = clock.getElapsedTime().asSeconds();
	double accumulator = 0.0;
	double frameTime;

	double alpha;

	// GAME LOOP
	while (window->isOpen())
	{
		// Process events
		processEvents();

		// Get time
		sf::Time t = clock.getElapsedTime();
		this->currentTime = t;

		frameTime = t.asSeconds() - currentTime;
		if (frameTime > 0.25)
			frameTime = 0.25;

		currentTime = t.asSeconds();

		accumulator += frameTime;
		// Consume time to obtain a steady frame rate using a fixed timestep
		while (accumulator >= dt)
		{
			// Update game logic
			update();

			accumulator -= dt;
		}

		// Calcuate alpha value used in interpolation calculations
		alpha = accumulator / dt;

		// Render the game
		render(alpha);
	}
}