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
#pragma once
#include "GameState.h"
#include "Screen.h"
#include "Player.h"
#include "UI.h"
#include "Asteroid.h"
#include "Projectile.h"
#include "NetworkManager.h"
#include "UIDataMsg.h"
#include "Player_UI_Data_Packet.h"
#include "Asteroids_Data_Packet.h"
#include "Projectiles_Data_Packet.h"
#include <list>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// GLOBALS
extern bool printDataToConsole;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Level : public Screen
{
public:
	//Level() {}
	Level(sf::RenderWindow* hwnd, Input* in, GameState* gs);
	~Level();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	// INIT STUFF
	void initConnection();
	void initDebugMode();
	void initBackground();
	void initUI();
	void initLevel();
	void initPlayer();
	void initAsteroids();

	// UPDATE STUFF
	void updateAsteroids(float dt);
	void updateProjectiles(float dt);
	void updateDebugMode();

	// RENDER STUFF
	void renderAsteroids();
	void renderProjectiles();
	void renderDebugMode();

	// SPAWN STUFF
	void spawnNewAsteroid();
	void spawnNewProjectile();

	// CHECK STUFF
	void checkCollisions();
	
	// FOR DEBUG
	void createNewAsteroidColBox();
	void createNewProjectileColBox();

	// FOR RENDERING
	void beginDraw();
	void endDraw();

	// MISC.
	void loadTexture();

	UI* ui;
	Player* player1;
	std::vector<Asteroid*> asteroids;
	std::vector<Projectile*> projectiles;
	sf::Texture bgTexture;
	sf::Sprite bgSprite;
	sf::RectangleShape player1ColBox;
	sf::RectangleShape player2ColBox;
	std::vector<sf::RectangleShape> asteroidColBoxes;
	std::vector<sf::RectangleShape> projectileColBoxes;

	// ###################################### NETWORK STUFF ######################################
	NetworkManager* network;
	// For sending player data
	PlayerDataMsg* pdMsg;
	// For sending UI data
	UIDataMsg* uidMsg;
	// List used for ease of controlling amount of asteroid data using pop
	std::list<AsteroidDataMsg*> asteroidMsgsList;
	std::list<ProjectileDataMsg*> projectileMsgsList;

	Player_UI_Data_Packet playerUIpckt;
	Asteroids_Data_Packet asteroidsPckt;
	Projectiles_Data_Packet projectilesPckt;
	// ###################################### NETWORK STUFF END ##################################
	
	float networkUpdateTimer;
	float asteroidSpawnTime;
	float totalGameTime;

	bool isDebugMode;
	bool hasCollided;
	bool echoComplete;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////