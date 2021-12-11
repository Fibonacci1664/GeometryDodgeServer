#pragma once
#include <SFML/Graphics.hpp>
#include "Input.h"
#include "NetworkManager.h"

class Player : sf::Sprite
{
public:
	Player() {}
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

	int playerNum;
	float fireButtonSpamTimer = 0.0f;
	float speed;
	float normX;
	float normY;
	int playerScore = 0;
	PlayerDataMsg* playerData;
};