#include "Asteroid.h"
#include <iostream>
#include <cstdlib>

Asteroid::Asteroid(sf::RenderWindow* hwnd) : window(hwnd)
{
	position = sf::Vector2f(0.0f, 0.0f);
	velocity = sf::Vector2f(0.0f, 0.0f);
	randXPos = rand() % 1180 + 50;
	randYPos = 20.0f;

	asteroidMsg = new AsteroidDataMsg;

	initAsteroid();
}

Asteroid::~Asteroid()
{
	if (asteroidMsg)
	{
		delete asteroidMsg;
		asteroidMsg = nullptr;
	}
}

AsteroidDataMsg* Asteroid::packAsteroidData(float totalGameTime, int asteroidID)
{
	asteroidMsg->asteroidID = asteroidID;
	asteroidMsg->timeSent = totalGameTime;
	asteroidMsg->x = asteroidSprite.getPosition().x;
	asteroidMsg->y = asteroidSprite.getPosition().y;

	return asteroidMsg;
}

bool Asteroid::update(float dt)
{
	move();
	collisionBox = sf::FloatRect(asteroidSprite.getPosition().x - size.x * 0.5f, asteroidSprite.getPosition().y - size.y * 0.5f, size.x, size.y);
	bool oob = checkScreenBounds();

	return oob;
}

void Asteroid::render(sf::RenderWindow* window)
{
	window->draw(asteroidSprite);
}

void Asteroid::initAsteroid()
{
	loadTexture();
	asteroidTexture.setSmooth(true);
	asteroidSprite.setTexture(asteroidTexture);
	size = asteroidTexture.getSize();
	asteroidSprite.setOrigin(size.x * 0.5f, size.y * 0.5f);
	asteroidSprite.setPosition(sf::Vector2f(randXPos, randYPos));
	//asteroidSprite.setScale(0.75f, 0.75f);
	velocity = sf::Vector2f(0.0f, 2.5f);
	collisionBox = sf::FloatRect(asteroidSprite.getPosition().x - size.x * 0.5f, asteroidSprite.getPosition().y - size.y * 0.5f, size.x, size.y);
}

void Asteroid::loadTexture()
{
	if (!asteroidTexture.loadFromFile("res/gfx/asteroids/asterGreySml.png"))
	{
		std::cout << "Error loading asteroid texture\n";
	}
}

bool Asteroid::checkScreenBounds()
{
	// If we hit the left or right edges of the screen
	//if ((asteroidSprite.getPosition().x + (size.x * 0.5f) > window->getSize().x && velocity.x > 0) ||
	//	(asteroidSprite.getPosition().x - (size.x * 0.5f) < 0 && velocity.x < 0))
	//{
	//	// Bounce off by negating the current velocity
	//	velocity.x = -velocity.x;
	//}

	//// If we hit the top or btm edges of the screen
	//if ((asteroidSprite.getPosition().y + (size.y * 0.5f) > window->getSize().y && velocity.y > 0) ||
	//	(asteroidSprite.getPosition().y - (size.y * 0.5f) < 0 && velocity.y < 0))
	//{
	//	// Bounce off by negating the current velocity
	//	velocity.y = -velocity.y;
	//}

	// If we go out the bottom of the screen
	if (asteroidSprite.getPosition().y - size.y > window->getSize().y)
	{
		return true;
	}

	return false;
}

void Asteroid::move()
{
	asteroidSprite.setPosition(asteroidSprite.getPosition() + velocity);
}

sf::Sprite* Asteroid::getAsteroidSprite()
{
	return &asteroidSprite;
}

sf::FloatRect Asteroid::getCollisionBox()
{
	return collisionBox;
}

void Asteroid::setCollisionBox(float x, float y, float width, float height)
{
	collisionBox = sf::FloatRect(x, y, width, height);
}
