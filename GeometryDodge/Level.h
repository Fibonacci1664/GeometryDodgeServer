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

class Level : public Screen
{
public:
	Level() {}
	Level(sf::RenderWindow* hwnd, Input* in, GameState* gs);
	~Level();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	void initConnection();
	void initDebugMode();
	void initBackground();
	void initUI();
	void initLevel();
	void initPlayer();
	void initAsteroids();

	void updateAsteroids(float dt);
	void updateProjectiles(float dt);
	void updateDebugMode();

	void renderAsteroids();
	void renderProjectiles();
	void renderDebugMode();

	void spawnNewAsteroid();
	void spawnNewProjectile();

	void checkCollisions();
	
	void createNewAsteroidColBox();
	void createNewProjectileColBox();

	void beginDraw();
	void endDraw();

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

	// #### NETWORK STUFF ####
	NetworkManager* network;
	// For sending player data
	PlayerDataMsg* pdMsg;
	// For sending UI data
	UIDataMsg* uidMsg;
	// For ease of controlling amount of asteroid data
	std::list<AsteroidDataMsg*> asteroidMsgsList;
	std::list<ProjectileDataMsg*> projectileMsgsList;
	// For sending
	//std::vector<AsteroidDataMsg*> asteroidMsgsVec;

	Player_UI_Data_Packet playerUIpckt;
	Asteroids_Data_Packet asteroidsPckt;
	Projectiles_Data_Packet projectilesPckt;

	//int playerScore;
	bool isDebugMode;
	float networkUpdateTimer = 0.0f;
	float asteroidSpawnTime = 0.0f;
	float totalGameTime = 0.0f;
	bool accept = false;
	bool firstSend = true;
};