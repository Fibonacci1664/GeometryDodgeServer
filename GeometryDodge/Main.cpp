// THIS IS THE NEW SERVER
// EVERYTHING RUNS FROM HERE, THE CLIENT SIMPLY MIRRORS WHAT'S GOING ON WITH CLIENT SIDE PREDICTION FOR THE PLAYER

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Application.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Set this if you wish to see all sent data printed to the console, doing so will slow down the application
bool printDataToConsole = false;

int main()
{
    std::srand(std::time(nullptr));

    Application app(WINDOW_WIDTH, WINDOW_HEIGHT);
    app.run();

	return 0;
}