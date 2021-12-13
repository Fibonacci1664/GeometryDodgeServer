// THIS IS THE SERVER
// EVERYTHING RUNS FROM HERE, THE CLIENT 'SIMPLY' MIRRORS WHAT'S GOING ON WITH CLIENT SIDE PREDICTION FOR THE PLAYER

/*
 * THIS WAS CREATED USING VISUAL STUDIO COMMUNITY EDITION - VERSION 16.11.6
 * AND SFML - VERSION 2.5.1
 */

 /*
  * This is the main point of entry for the program and handles
  *		- Setting the window size.
  *     - Creating and running the Application
  *
  * Original @author D. Green.
  *
  * � D. Green. 2021.
  */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Application.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// GLOBALS
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Set this if you wish to see all sent data printed to the console, doing so will slow down the application
bool printDataToConsole = false;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    std::srand(std::time(nullptr));

    Application app(WINDOW_WIDTH, WINDOW_HEIGHT);
    app.run();

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////