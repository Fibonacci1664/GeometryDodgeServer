#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "UIDataMsg.h"

class UI
{
public:
	UI();
	~UI();

	void update(float dt, int playerScore);
	void render(sf::RenderWindow* window);
	UIDataMsg* packUIData();

	sf::Text* getScoreText();
	int getScore();

private:
	void initUI();
	void loadFont();

	sf::Font font;
	sf::Text scoreText;

	UIDataMsg* UIMsg;

	int score;
	int timer;
	float count;
};