#include "Menu.h"

Menu::Menu(sf::RenderWindow* hwnd, Input* in, GameState* gs) : Screen(hwnd, in, gs)
{
	initMenu();
}

Menu::~Menu()
{

}

void Menu::initMenu()
{
	loadTexture();
	bgTexture.setSmooth(true);
	bgSprite.setTexture(bgTexture);
	bgSprite.setScale(5.0f, 2.8125f);

	initUI();
}

void Menu::initUI()
{
	loadFont();

	playGamePrompt.setFont(font);
	playGamePrompt.setString("Press 'P' to play game!");
	playGamePrompt.setCharacterSize(32);
	playGamePrompt.setFillColor(sf::Color::Red);
	playGamePrompt.setOutlineColor(sf::Color::Black);
	playGamePrompt.setOutlineThickness(1.0f);
	playGamePrompt.setPosition(window->getSize().x * 0.33f, window->getSize().y * 0.45f);
}

void Menu::loadFont()
{
	if (!font.loadFromFile("res/fonts/kenvector_future_thin.ttf"))
	{
		std::cout << "Error loading font\n";
	}
}

void Menu::loadTexture()
{
	if (!bgTexture.loadFromFile("res/gfx/bg/blue.png"))
	{
		std::cout << "Error loading background texture.\n";
	}
}

void Menu::handleInput(float dt)
{
	// Check the bool key array in the input class to see if the index corrosponding
	// to the letter P is true, i.e. the key has been pressed
	if (input->isKeyDown(sf::Keyboard::P))
	{
		setGameState(State::LEVEL);
	}
}

void Menu::update(float dt)
{

}

void Menu::render()
{
	beginDraw();
	window->draw(bgSprite);
	window->draw(playGamePrompt);
	endDraw();
}

void Menu::beginDraw()
{
	window->clear(sf::Color::Black);
}

void Menu::endDraw()
{
	window->display();
}
