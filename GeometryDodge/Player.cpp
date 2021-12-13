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
#include "Player.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
Player::Player(int playerNum, sf::RenderWindow* hwnd, Input* in) : playerNum(playerNum), window(hwnd), input(in)
{
	position = sf::Vector2f(0.0f, 0.0f);
	velocity = sf::Vector2f(0.0f, 0.0f);

	playerScore = 0;

	fireButtonSpamTimer = 0.0f;
	speed = 700.0f;

	playerData = new PlayerDataMsg;

	initPlayer();
}

Player::~Player()
{
	if (playerData)
	{
		delete playerData;
		playerData = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
bool Player::handleInput(float dt)
{
	fireButtonSpamTimer += dt;

	// Handle player input
	// Normalise the thumbstick input, the range is [-100, 100]
	normX = input->getLeftStick().x / 100.0f;
	normY = input->getLeftStick().y / 100.0f;

	// Check if the X button has been pressed, and a quarter of a second has passed
	if (input->isButtonDown(1) && fireButtonSpamTimer >= 0.25f)
	{
		fireButtonSpamTimer = 0.0f;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PlayerDataMsg* Player::packPlayerData(float totalGameTime)
{
	playerData->playerID = playerNum;
	playerData->timeSent = totalGameTime;
	playerData->x = playerSprite.getPosition().x;
	playerData->y = playerSprite.getPosition().y;

	return playerData;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::update(float dt)
{
	playerSprite.move(sf::Vector2f(normX * dt * speed, normY * dt * speed));
	playerCollisionBox = sf::FloatRect(playerSprite.getPosition().x - size.x * 0.4f, playerSprite.getPosition().y - size.y * 0.4f, size.x * 0.8f, size.y * 0.8f);
	checkScreenBounds();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::initPlayer()
{
	loadTextures();
	playerTexture.setSmooth(true);
	playerSprite.setTexture(playerTexture);
	size = playerTexture.getSize();
	playerSprite.setOrigin(size.x * 0.5f, size.y * 0.5f);
	playerSprite.setPosition(sf::Vector2f(640.0f, 640.0f));
	playerSprite.setScale(0.75f, 0.75f);
	playerCollisionBox = sf::FloatRect(playerSprite.getPosition().x - size.x * 0.4f, playerSprite.getPosition().y - size.y * 0.4f, size.x * 0.8f, size.y * 0.8f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::loadTextures()
{
	if (!playerTexture.loadFromFile("res/gfx/ships/altShip_" + std::to_string(playerNum) + ".png"))
	{
		std::cout << "Error loading ship texture " << playerNum << '\n';
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::checkScreenBounds()
{
	// Check left edge of screen
	if (playerSprite.getPosition().x - (size.x * 0.5f) < 0)
	{
		playerSprite.setPosition(sf::Vector2f(size.x * 0.5f, playerSprite.getPosition().y));
	}

	// Check right edge of screen
	if (playerSprite.getPosition().x + (size.x * 0.5f) > window->getSize().x)
	{
		playerSprite.setPosition(sf::Vector2f(window->getSize().x - size.x * 0.5f, playerSprite.getPosition().y));
	}

	// Check top edge of screen
	if (playerSprite.getPosition().y - (size.y * 0.5f) < 0)
	{
		playerSprite.setPosition(sf::Vector2f(playerSprite.getPosition().x, size.y * 0.5f));
	}

	// Check btm edge of screen
	if (playerSprite.getPosition().y + (size.y * 0.5f) > window->getSize().y)
	{
		playerSprite.setPosition(sf::Vector2f(playerSprite.getPosition().x, window->getSize().y - size.y * 0.5f));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sf::Sprite* Player::getPlayerSprite()
{
	return &playerSprite;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sf::Vector2u* Player::getPlayerSize()
{
	return &size;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sf::FloatRect Player::getCollisionBox()
{
	return playerCollisionBox;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setCollisionBox(float x, float y, float width, float height)
{
	playerCollisionBox = sf::FloatRect(x, y, width, height);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::setPlayerScore(int newScore)
{
	playerScore = newScore;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Player::getPlayerScore()
{
	return playerScore;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////