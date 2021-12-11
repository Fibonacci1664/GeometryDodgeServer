#include "Application.h"
#include <iostream>

Application::Application(int width, int height) : windowWidth(width), windowHeight(height)
{
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
}

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

void Application::run()
{
    gameState.setCurrentState(State::MENU);

    // Run the program as long as the window is open
    while (window.isOpen())
    {
        level = new Level(&window, &input, &gameState);
        menu = new Menu(&window, &input, &gameState);

        // Initialise objects for delta time
        sf::Clock clock;
        float deltaTime;

        // If the game isn't over, keep processing stuff
        while (gameState.getCurrentState() != State::GAMEOVER)
        {
            processWindowEvents();

            // Calculate delta time. How much time has passed 
            // since it was last calculated (in seconds) and restart the clock.
            deltaTime = clock.restart().asSeconds();

            runGameLoop(level, menu, deltaTime);
        }

        // Destroy all old game objects, except gameState
        break;

        // Set game back to main menu - TO DO
        //gameState->setCurrentState(State::MENU);
    }
}

void Application::processWindowEvents()
{
    // Check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    unsigned int n_buts = 0;
    //window.setJoystickThreshold(2.0f);        // This doesn't do anything, threshold is controlled in Input class

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
            case sf::Event::LostFocus:
            {
                // Pause game if window loses focus
            }
            case sf::Event::GainedFocus:
            {
                // Resume game when window gains focus again
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
                // Need an input class that handles input
                input.setLeftStick(sf::Joystick::getAxisPosition(0, sf::Joystick::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Y));
            }
            case sf::Event::JoystickButtonPressed:
            {
                // Check if button 1 (X button) has been pressed on joystick 0, will always return true,
                // as this event won't fire unlees the button has been pressed, set the input class accordingly for checking

                input.setButtonState(1, sf::Joystick::isButtonPressed(0, 1));
            }
            case sf::Event::JoystickButtonReleased:
            {
                // Check if button 1 (X button) has been pressed on joystick 0, will always return false,
                // as this event won't fire unlees the button has been released, set the input class accordingly for checking

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

void Application::runGameLoop(Level* level, Menu* menu, float deltaTime)
{
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
            break;
        }
        case(State::PAUSE):
        {
            /* pause->handleInput(deltaTime);
             pause->update(deltaTime);
             pause->render();
             break;*/
        }
    }
}