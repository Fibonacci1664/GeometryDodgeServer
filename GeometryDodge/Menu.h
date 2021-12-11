#pragma once
#include <SFML/Graphics.hpp>
#include "Screen.h"

class Menu : public Screen
{
public:
	Menu(sf::RenderWindow* hwnd, Input* in, GameState* gs);
	~Menu();

	void handleInput(float dt);
	void update(float dt);
	void render();

private:
	void initMenu();
	void initUI();
	void loadTexture();
	void beginDraw();
	void endDraw();

	void loadFont();

	sf::Font font;
	sf::Text playGamePrompt;

	sf::Texture bgTexture;
	sf::Sprite bgSprite;
};

