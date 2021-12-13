/*
 * This is the Asteroid class and handles
 *		- Initilaising a single asteroid.
 *		- Moving / updating the asteroid.
 *		- Checking oob from bottom of screen.
 *		- Packing asteroid data into a struct for network data transfer.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES
#pragma once
#include <SFML/Graphics.hpp>
#include "AsteroidDataMsg.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Asteroid
{
public:
	Asteroid(sf::RenderWindow* hwnd);
	~Asteroid();

	AsteroidDataMsg* packAsteroidData(float dt, int asteroidID);
	bool update(float dt);
	void render(sf::RenderWindow* window);
	sf::Sprite* getAsteroidSprite();
	sf::FloatRect getCollisionBox();
	void setCollisionBox(float x, float y, float width, float height);

private:
	void initAsteroid();
	void loadTexture();
	bool checkScreenBounds();
	void move();
	
	sf::RenderWindow* window;
	sf::Texture asteroidTexture;
	sf::Sprite asteroidSprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2u size;
	sf::FloatRect collisionBox;

	AsteroidDataMsg* asteroidMsg;

	int randXPos;
	int randYPos;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////