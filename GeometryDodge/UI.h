/*
 * This is the UI class and handles
 *		- Initialisation of UI elements.
 *		- Rendering of the UI.
 *		- Updating the UI with the correct score.
 *		- Packing UI data into a struct for network data transfer.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES
#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "UIDataMsg.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////