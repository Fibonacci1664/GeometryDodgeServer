#include "Projectile.h"
#include <iostream>
#include <cstdlib>

Projectile::Projectile(sf::RenderWindow* hwnd, sf::Vector2f spawnPos) : window(hwnd), spawnPosition(spawnPos)
{
	velocity = sf::Vector2f(0.0f, -10.0f);

	projectileMsg = new ProjectileDataMsg;

	initProjectile();
}

Projectile::~Projectile()
{

}

ProjectileDataMsg* Projectile::packProjectileData(float totalGameTime, int projectileID)
{
	projectileMsg->projectileID = projectileID;
	projectileMsg->timeSent = totalGameTime;
	projectileMsg->x = projectileSprite.getPosition().x;
	projectileMsg->y = projectileSprite.getPosition().y;

	return projectileMsg;
}

bool Projectile::update(float dt)
{
	move();
	projectileCollisionBox = sf::FloatRect(projectileSprite.getPosition().x - size.x * 0.5f, projectileSprite.getPosition().y - size.y * 0.5f, size.x, size.y);
	bool oob = checkScreenBounds();

	return oob;
}

void Projectile::render(sf::RenderWindow* window)
{
	window->draw(projectileSprite);
}

void Projectile::initProjectile()
{
	loadTexture();
	projectileTexture.setSmooth(true);
	projectileSprite.setTexture(projectileTexture);
	size = projectileTexture.getSize();
	projectileSprite.setOrigin(size.x * 0.5f, size.y * 0.5f);
	projectileSprite.setPosition(spawnPosition);
	//projectileSprite.setScale(0.75f, 0.75f);
	projectileCollisionBox = sf::FloatRect(projectileSprite.getPosition().x - size.x * 0.4f, projectileSprite.getPosition().y - size.y * 0.4f, size.x * 0.8f, size.y * 0.8f);
}

void Projectile::loadTexture()
{
	if (!projectileTexture.loadFromFile("res/gfx/effects/fire.png"))
	{
		std::cout << "Error loading projectile texture\n";
	}
}

bool Projectile::checkScreenBounds()
{
	// If we go out the top of the screen
	if (projectileSprite.getPosition().y + size.y < 0)
	{
		return true;
	}

	return false;
}

void Projectile::move()
{
	projectileSprite.setPosition(projectileSprite.getPosition() + velocity);
}

sf::Sprite* Projectile::getProjectileSprite()
{
	return &projectileSprite;
}

sf::FloatRect Projectile::getCollisionBox()
{
	return projectileCollisionBox;
}

void Projectile::setCollisionBox(float x, float y, float width, float height)
{
	projectileCollisionBox = sf::FloatRect(x, y, width, height);
}
