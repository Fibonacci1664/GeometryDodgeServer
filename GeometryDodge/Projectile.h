#pragma once
#include <SFML/Graphics.hpp>
#include "ProjectileDataMsg.h"

class Projectile
{
public:
	Projectile(sf::RenderWindow* hwnd, sf::Vector2f spawnPos);
	~Projectile();

	ProjectileDataMsg* packProjectileData(float dt, int asteroidID);
	bool update(float dt);
	void render(sf::RenderWindow* window);
	sf::Sprite* getProjectileSprite();
	sf::FloatRect getCollisionBox();
	void setCollisionBox(float x, float y, float width, float height);

private:
	void initProjectile();
	void loadTexture();
	bool checkScreenBounds();
	void move();

	sf::RenderWindow* window;
	sf::Texture projectileTexture;
	sf::Sprite projectileSprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2u size;
	sf::FloatRect projectileCollisionBox;

	sf::Vector2f spawnPosition;
	float totalGameTime = 0.0f;
	ProjectileDataMsg* projectileMsg;
};