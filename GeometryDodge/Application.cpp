/*
 * This is the Application class and handles
 *		- Window creation.
 *		- Polling window events.
 *		- The main game loop.
 *		- Creation, updating and deletion of all Screen objects.
 *
 * Original @author D. Green.
 *
 * © D. Green. 2021.
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES
#include "Application.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CONSTRUCTOR / DESTRUCTOR
Application::Application(int width, int height) : windowWidth(width), windowHeight(height)
{
    displayGameOverTimer = 0.0f;

    initWindow();
}

Application::~Application()
{
    if (level)
    {
        delete level;
        level = nullptr;
    }

    if (menu)
    {
        delete menu;
        menu = nullptr;
    }
    
    if (gameOver)
    {
        delete gameOver;
        gameOver = nullptr;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// FUNCTIONS
void Application::initWindow()
{
    window.create(sf::VideoMode(windowWidth, windowHeight), "Server Asteroid Dodge!");
    // NEVER have both vSync AND framerate limit, it's either or, NEVER both!
    // If you find vSync is having no effect, check graphics driver's settings and change vSync from "off" to "controlled by application"
    //window.setVerticalSyncEnabled(true);    // Ensure the application runs at the same frequency as the monitor's refresh rate
    window.setFramerateLimit(60);           // Request a fixed framerate, diffrent OS may output slightly different FPS

    // Get the native screen res
    int nativeScreenWidth = sf::VideoMode::getDesktopMode().width;
    int nativeScreenHeight = sf::VideoMode::getDesktopMode().height;

    // Place the viewing window in the centre of the screen
    window.setPosition(sf::Vector2i((nativeScreenWidth * 0.5f) - (windowWidth * 0.5f), (nativeScreenHeight * 0.5f) - (windowHeight * 0.5f)));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Application::run()
{
    gameState.setCurrentState(State::MENU);
    bool isGameOver = false;

    // Run the program as long as the window is open
    while (window.isOpen())
    {
        level = new Level(&window, &input, &gameState);
        menu = new Menu(&window, &input, &gameState);
        gameOver = new GameOver(&window, &input, &gameState);

        // Initialise objects for delta time
        sf::Clock clock;
        float deltaTime;

        // If the game isn't over, keep processing stuff
        while (!isGameOver)
        {
            processWindowEvents();

            // Calculate delta time. How much time has passed 
            // since it was last calculated (in seconds) and restart the clock.
            deltaTime = clock.restart().asSeconds();

            isGameOver = runGameLoop(level, menu, gameOver, deltaTime);
        }

        // Destroy all old game objects, except gameState
        if (level)
        {
            delete level;
            level = nullptr;
        }

        if (menu)
        {
            delete menu;
            menu = nullptr;
        }

        if (gameOver)
        {
            delete gameOver;
            gameOver = nullptr;
        }

        // Set game back to main menu
        gameState.setCurrentState(State::MENU);
        isGameOver = false;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Application::processWindowEvents()
{
    // Check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    unsigned int n_buts = 0;
    //window.setJoystickThreshold(2.0f);        // This didn't seem to do anything, so drift threshold is controlled in Input class

    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
            {
                window.close();
                break;
            }
            case sf::Event::Resized:
            {
                window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
                break;
            }
            case sf::Event::JoystickConnected:
            {
                std::cout << "Controller is connected!.\n";
                n_buts = sf::Joystick::getButtonCount(0);
                std::cout << "joystick zero button count = " << n_buts << '\n';

                if (sf::Joystick::hasAxis(0, sf::Joystick::X))
                {
                    std::cout << "supports x-axis movement\n";
                }

                if (sf::Joystick::hasAxis(0, sf::Joystick::Y))
                {
                    std::cout << "supports y-axis movement\n";
                }

                break;
            }
            case sf::Event::JoystickDisconnected:
            {
                std::cout << "Controller is disconnected!.\n";
                break;
            }
            case sf::Event::JoystickMoved:
            {
                input.setLeftStick(sf::Joystick::getAxisPosition(0, sf::Joystick::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Y));
            }
            case sf::Event::JoystickButtonPressed:
            {
                input.setButtonState(1, sf::Joystick::isButtonPressed(0, 1));
            }
            case sf::Event::JoystickButtonReleased:
            {
                input.setButtonState(1, sf::Joystick::isButtonPressed(0, 1));
            }
            case sf::Event::KeyPressed:
            {
                input.setKeyDown(event.key.code);
                break;
            }
            case sf::Event::KeyReleased:
            {
                input.setKeyUp(event.key.code);
                break;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Application::runGameLoop(Level* level, Menu* menu, GameOver* gameOver, float deltaTime)
{
    bool gameEnd = false;

    switch (gameState.getCurrentState())
    {
        case(State::MENU):
        {
            menu->handleInput(deltaTime);
            menu->update(deltaTime);
            menu->render();
            break;
        }
        case(State::LEVEL):
        {
            level->handleInput(deltaTime);
            level->update(deltaTime);
            level->render();
            break;
        }
        case(State::GAMEOVER):
        {
            displayGameOverTimer += deltaTime;
            gameOver->update(deltaTime);
            gameOver->render();
            break;
        }
    }

    // This controls showing the gameover screen for 5 secs
    if (displayGameOverTimer > 5.0f)
    {
        displayGameOverTimer = 0.0f;
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////