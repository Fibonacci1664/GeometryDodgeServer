/*
 * This is the Player class and handles
 *		- Initialisation of player including:
 *				*	Loading textures.
 *				*	Setting up sprite and collision boxes.
 *		- Handling input.
 *		- Moving/updating player.
 *		- Carrying out collision checks with screen bounds.
 *		- Packing player data into a struct for network data transfer.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES
#pragma once
#include <SFML/Graphics.hpp>
#include "Input.h"
#include "NetworkManager.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Player : sf::Sprite
{
public:
	//Player() {}
	Player(int playerNum, sf::RenderWindow* hwnd, Input* in);
	~Player();

	bool handleInput(float dt);
	PlayerDataMsg* packPlayerData(float totalGameTime);
	void update(float dt);

	sf::Sprite* getPlayerSprite();
	sf::Vector2u* getPlayerSize();
	sf::FloatRect getCollisionBox();
	void setCollisionBox(float x, float y, float width, float height);
	void setPlayerScore(int newScore);
	int getPlayerScore();

private:
	void initPlayer();
	void loadTextures();
	void checkScreenBounds();

	Input* input;
	sf::RenderWindow* window;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2u size;
	sf::FloatRect playerCollisionBox;
	PlayerDataMsg* playerData;

	int playerNum;
	int playerScore;

	float fireButtonSpamTimer;
	float speed;
	float normX;
	float normY;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////