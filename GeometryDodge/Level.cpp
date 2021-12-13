/*
 * This is the Level class and handles
 *		- Creating a network manager.
 *		- Handling rendering of debug collision boxes.
 *		- Initialising all level components, including:
 *				*	Background
 *				*	UI
 *				*	Player
 *				*	Asteroids
 *		- Updating/Spawning level components.
 *		- Carrying our all collision checks.
 *		- Sending all game data to the network manager.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES
#include "Level.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRCUTOR / DESTRUCTOR
Level::Level(sf::RenderWindow* hwnd, Input* in, GameState* gs) : Screen(hwnd, in, gs)
{
	networkUpdateTimer = 0.0f;
	asteroidSpawnTime = 0.0f;
	totalGameTime = 0.0f;

	isDebugMode = true;

	uidMsg = new UIDataMsg;
	pdMsg = new PlayerDataMsg;
	
	initLevel();

	playerUIpckt.uiData = *uidMsg;
	playerUIpckt.playerData = *pdMsg;

	if (isDebugMode)
	{
		initDebugMode();
	}
}

Level::~Level()
{
	for (int i = 0; i < asteroids.size(); ++i)
	{
		if (asteroids[i])
		{
			delete asteroids[i];
			asteroids[i] = nullptr;
		}
	}

	for (int i = 0; i < projectiles.size(); ++i)
	{
		if (projectiles[i])
		{
			delete projectiles[i];
			projectiles[i] = nullptr;
		}
	}

	if (network)
	{
		delete network;
		network = nullptr;
	}

	if (player1)
	{
		delete player1;
		player1 = nullptr;
	}

	if (ui)
	{
		delete ui;
		ui = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void Level::initLevel()
{
	initConnection();	
	initBackground();
	initUI();
	initPlayer();
	initAsteroids();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initConnection()
{
	network = new NetworkManager();
	network->createTCPListner();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initDebugMode()
{
	// Player debug collision box
	player1ColBox.setFillColor(sf::Color(0, 0, 0, 0));
	player1ColBox.setOutlineColor(sf::Color::Red);
	player1ColBox.setOutlineThickness(1.0f);

	// Set the collision box origin to be the top left of the sprites local coords, this is
	// based on the already defined collision box which was defined during object creation
	player1ColBox.setPosition(sf::Vector2f(player1->getCollisionBox().left, player1->getCollisionBox().top));
	player1ColBox.setSize(sf::Vector2f(player1->getCollisionBox().width, player1->getCollisionBox().height));

	// Create a new asteroid collision box
	asteroidColBoxes.push_back(sf::RectangleShape());

	// Asteroids debug collision box for the initial asteroid
	asteroidColBoxes[0].setFillColor(sf::Color(0, 0, 0, 0));
	asteroidColBoxes[0].setOutlineColor(sf::Color::Magenta);
	asteroidColBoxes[0].setOutlineThickness(1.0f);

	asteroidColBoxes[0].setPosition(sf::Vector2f(asteroids[0]->getCollisionBox().left, asteroids[0]->getCollisionBox().top));
	asteroidColBoxes[0].setSize(sf::Vector2f(asteroids[0]->getCollisionBox().width, asteroids[0]->getCollisionBox().height));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initBackground()
{
	loadTexture();
	bgTexture.setSmooth(true);
	bgSprite.setTexture(bgTexture);
	bgSprite.setScale(5.0f, 2.8125f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initUI()
{
	ui = new UI;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initPlayer()
{
	player1 = new Player(1, window, input);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::initAsteroids()
{
	asteroids.push_back(new Asteroid(window));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::spawnNewAsteroid()
{
	asteroids.push_back(new Asteroid(window));
	createNewAsteroidColBox();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::spawnNewProjectile()
{
	projectiles.push_back(new Projectile(window, player1->getPlayerSprite()->getPosition()));
	createNewProjectileColBox();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::handleInput(float dt)
{
	bool shotFired = player1->handleInput(dt);

	if (shotFired)
	{
		// Spawn new projectile at player position
		spawnNewProjectile();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::updateAsteroids(float dt)
{
	// Update all the asteroids locally all the time
	for (int i = 0; i < asteroids.size(); ++i)
	{
		bool oob = asteroids[i]->update(dt);

		// If this is true then it means an asteroid went out the btm of the screen, so remove it from the vec
		if (oob)
		{
			if (printDataToConsole)
			{
				std::cout << "Update Call - Aseroid OOB - Erasing Asteroid - Vector size is currently: " << asteroids.size() << '\n';
			}

			// Create an iterator to the asteroid that has gone oob, as the asteroids all fall at the same speed
			// it will always be the first in the list
			auto iter1 = asteroids.begin();
			// Erase it from the vector
			asteroids.erase(iter1);

			// Ensure collision box for this asteroid is also removed from the collision box vector
			auto iter2 = asteroidColBoxes.begin();
			asteroidColBoxes.erase(iter2);

			// Decrement the player score
			player1->setPlayerScore(player1->getPlayerScore() - 1);

			if (printDataToConsole)
			{
				std::cout << "Update Call - Asteroid erased - Vector size is now: " << asteroids.size() << '\n';
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::updateProjectiles(float dt)
{
	// Update all the projectiles locally all the time
	for (int i = 0; i < projectiles.size(); ++i)
	{
		bool oob = projectiles[i]->update(dt);

		// If this is true then it means an projectile went out the top of the screen, so remove it from the vec
		if (oob)
		{
			if (printDataToConsole)
			{
				std::cout << "Update Call - Projectile OOB - Erasing Projectile - Vector size is currently: " << projectiles.size() << '\n';
			}

			// Create an iterator to the asteroid that has gone oob, as the asteroids all fall at the same speed
			// it will always be the first in the list
			auto iter1 = projectiles.begin();
			// Erase it from the vector
			projectiles.erase(iter1);

			// Ensure collision box for this asteroid is also removed from the collision box vector
			auto iter2 = projectileColBoxes.begin();
			projectileColBoxes.erase(iter2);

			if (printDataToConsole)
			{
				std::cout << "Update Call - Projectile erased - Vector size is now: " << projectiles.size() << '\n';
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::updateDebugMode()
{
	if (isDebugMode)
	{
		// Update the players collision box
		player1ColBox.setPosition(sf::Vector2f(player1->getCollisionBox().left, player1->getCollisionBox().top));

		// Update all the asteroids collision boxes
		for (int i = 0; i < asteroidColBoxes.size(); ++i)
		{
			asteroidColBoxes[i].setPosition(sf::Vector2f(asteroids[i]->getCollisionBox().left, asteroids[i]->getCollisionBox().top));
		}

		// Update all the asteroids collision boxes
		for (int i = 0; i < projectileColBoxes.size(); ++i)
		{
			projectileColBoxes[i].setPosition(sf::Vector2f(projectiles[i]->getCollisionBox().left, projectiles[i]->getCollisionBox().top));
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::update(float dt)
{
	network->acceptConnections();

	asteroidSpawnTime += dt;
	networkUpdateTimer += dt;
	totalGameTime += dt;

	// ################################################################### LOCAL UPDATE ###################################################################
	ui->update(dt, player1->getPlayerScore());
	player1->update(dt);
	updateAsteroids(dt);
	updateProjectiles(dt);
	updateDebugMode();

	if (asteroidSpawnTime >= 1.0f)
	{
		spawnNewAsteroid();
		asteroidSpawnTime = 0.0f;
	}

	checkCollisions();

	// ################################################################### LOCAL UPDATE END ###############################################################

	// ################################################################### NETWORK UPDATE 50ms ############################################################
	if (networkUpdateTimer >= 0.05f)
	{
		// Pack data into individual structs
		// ###################################################################### UI ######################################################################
		uidMsg = ui->packUIData();
		// ###################################################################### UI END ##################################################################

		// ###################################################################### PLAYER ##################################################################
		pdMsg = player1->packPlayerData(totalGameTime);
		// ###################################################################### PLAYER END ##############################################################

		// ###################################################################### ASTEROIDS ###############################################################
		// Clear the old data from the lsit and repopulate with latest data
		asteroidMsgsList.clear();

		for (int i = 0; i < asteroids.size(); i++)
		{
			// Create a unique mem addr for this data
			AsteroidDataMsg* asterDataMsg = new AsteroidDataMsg;
			// Fill this mem address with the latest asteroid data
			*asterDataMsg = *asteroids[i]->packAsteroidData(totalGameTime, i);

			// Add this to the vec of asteroid positional data
			asteroidMsgsList.push_back(asterDataMsg);
		}

		// If there are no asteroids in the world, this was needed during testing 1 asteroid only
		if (asteroids.size() == 0)
		{
			// Reset the asteroid vec in the asteroid data packet
			asteroidsPckt.asteroidDataMsgs.clear();
		}
		else
		{
			asteroidsPckt.asteroidDataMsgs.resize(asteroidMsgsList.size());
			std::copy(asteroidMsgsList.begin(), asteroidMsgsList.end(), asteroidsPckt.asteroidDataMsgs.begin());  // Copy list into asteroid data packet vec
		}

		asteroidsPckt.asteroidDataMsgSize = asteroidsPckt.asteroidDataMsgs.size();	// Keep track of how many elements are in this vec, needed for send/recv

		// ###################################################################### ASTEROIDS END ############################################################

		// ###################################################################### PROJECTILES ##############################################################
		// Clear the old data from the lISt and repopulate with latest data
		projectileMsgsList.clear();

		for (int i = 0; i < projectiles.size(); i++)
		{
			// Create a unique mem addr for this data
			ProjectileDataMsg* projDataMsg = new ProjectileDataMsg;
			// Fill this mem address with the latest projectile data
			*projDataMsg = *projectiles[i]->packProjectileData(totalGameTime, i);

			// Add this to the vec of projectile positional data
			projectileMsgsList.push_back(projDataMsg);
		}

		// If there are no projectiles in the world
		if (projectiles.size() == 0)
		{
			// Reset the projectile vec in the projcetile data packet
			projectilesPckt.projectileDataMsgs.clear();
		}
		else
		{
			projectilesPckt.projectileDataMsgs.resize(projectileMsgsList.size());
			std::copy(projectileMsgsList.begin(), projectileMsgsList.end(), projectilesPckt.projectileDataMsgs.begin());	// Copy list into projectile data packet vec
		}

		projectilesPckt.projectileDataMsgSize = projectilesPckt.projectileDataMsgs.size();	// Keep track of how many elements are in this vec, needed for send/recv

		// ###################################################################### PROJECTILES END ##########################################################

		playerUIpckt.uiData = *uidMsg;
		playerUIpckt.playerData = *pdMsg;

		// Send ALL data msgs
		network->sendPlayer_UI_Packet(playerUIpckt);

		// Check score here immendiately after we have send the UI (score) data, if we sent -1 as the score
		// then the game over state should be triggered and no further sends should occur
		// This will prevent being blocked on the client side waiting for data that will never arrive
		if (player1->getPlayerScore() < 0)
		{
			gameState->setCurrentState(State::GAMEOVER);
			return;
			// Transmit that it is game over here, as we will not get another chance as level::update will NOT run next frame
			//network->sendGameState(int(gameState->getCurrentState()));
		}

		network->sendAsteroidPacket(asteroidsPckt);
		network->sendProjectilesPacket(projectilesPckt);

		networkUpdateTimer = 0.0f;
	}

	// ################################################################### NETWORK UPDATE END ############################################################
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::renderAsteroids()
{
	// Draw all the asteroids
	for (int i = 0; i < asteroids.size(); i++)
	{
		window->draw(*asteroids[i]->getAsteroidSprite());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::renderProjectiles()
{
	// Draw all the projectiles
	for (int i = 0; i < projectiles.size(); i++)
	{
		window->draw(*projectiles[i]->getProjectileSprite());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::renderDebugMode()
{
	// Draw all the debug magenta collision boxes
	if (isDebugMode)
	{
		window->draw(player1ColBox);

		for (int i = 0; i < asteroidColBoxes.size(); ++i)
		{
			window->draw(asteroidColBoxes[i]);
		}

		for (int i = 0; i < projectileColBoxes.size(); ++i)
		{
			window->draw(projectileColBoxes[i]);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::render()
{
	beginDraw();

	// Render stuff here
	window->draw(bgSprite);
	window->draw(*ui->getScoreText());
	window->draw(*player1->getPlayerSprite());

	renderAsteroids();
	renderProjectiles();

	renderDebugMode();

	endDraw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::beginDraw()
{
	window->clear(sf::Color::Black);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::endDraw()
{
	window->display();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::loadTexture()
{
	if (!bgTexture.loadFromFile("res/gfx/bg/blue.png"))
	{
		std::cout << "Error loading background texture.\n";
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::checkCollisions()
{
	// If player collides with an asteroid, set GAME_OVER state
	for (int i = 0; i < asteroids.size(); ++i)
	{
		if (player1->getCollisionBox().intersects(asteroids[i]->getCollisionBox()))
		{
			gameState->setCurrentState(State::GAMEOVER);
			return;
			// Transmit that it is game over here, as we will not get another chance as level::update will NOT run next frame
			//network->sendGameState(int(gameState->getCurrentState()));
		}
	}

	bool asteroidDestroyed = false;

	// Check each projectile against each asteroid
	for (int i = 0; i < asteroids.size(); ++i)
	{
		for (int j = 0; j < projectiles.size(); ++j)
		{
			if (projectiles[j]->getCollisionBox().intersects(asteroids[i]->getCollisionBox()))
			{
				if (printDataToConsole)
				{
					std::cout << "Check Collision - Asteroid vector size is " << asteroids.size() << '\n';
				}

				// Destroy the asteroid that was hit
				// Create an iterator to the asteroid that has been hit
				auto iter1 = asteroids.begin() + i;
				// Erase it from the vector
				asteroids.erase(iter1);

				// Ensure collision box for this asteroid is also removed from the collision box vector
				auto iter2 = asteroidColBoxes.begin() + i;
				asteroidColBoxes.erase(iter2);

				if (printDataToConsole)
				{
					std::cout << "Check Collision - Asteroid vector size is now " << asteroids.size() << '\n';
				}

				// Increment the player score
				player1->setPlayerScore(player1->getPlayerScore() + 1);

				asteroidDestroyed = true;
			}

			// This is req. as erasing an asteroid invalidates the indexing
			if (asteroidDestroyed)
			{
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::createNewAsteroidColBox()
{
	// Create a new asteroid collision box
	asteroidColBoxes.push_back(sf::RectangleShape());

	// Access the last element in the vector, i.e. the one we just pushed back
	int index = asteroidColBoxes.size() - 1;

	asteroidColBoxes[index].setFillColor(sf::Color(0, 0, 0, 0));
	asteroidColBoxes[index].setOutlineColor(sf::Color::Magenta);
	asteroidColBoxes[index].setOutlineThickness(1.0f);

	// Set the collision box origin to be the top left of the sprites local coords, this is
	// based on the already defined collision box which was defined during object creation
	asteroidColBoxes[index].setPosition(sf::Vector2f(asteroids[index]->getCollisionBox().left, asteroids[index]->getCollisionBox().top));
	asteroidColBoxes[index].setSize(sf::Vector2f(asteroids[index]->getCollisionBox().width, asteroids[index]->getCollisionBox().height));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Level::createNewProjectileColBox()
{
	// Create a new asteroid collision box
	projectileColBoxes.push_back(sf::RectangleShape());

	// Access the last element in the vector, i.e. the one we just pushed back
	int index = projectileColBoxes.size() - 1;

	projectileColBoxes[index].setFillColor(sf::Color(0, 0, 0, 0));
	projectileColBoxes[index].setOutlineColor(sf::Color::Magenta);
	projectileColBoxes[index].setOutlineThickness(1.0f);

	// Set the collision box origin to be the top left of the sprites local coords, this is
	// based on the already defined collision box which was defined during object creation
	projectileColBoxes[index].setPosition(sf::Vector2f(projectiles[index]->getCollisionBox().left, projectiles[index]->getCollisionBox().top));
	projectileColBoxes[index].setSize(sf::Vector2f(projectiles[index]->getCollisionBox().width, projectiles[index]->getCollisionBox().height));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////