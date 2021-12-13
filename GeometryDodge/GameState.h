/*
 * This is the GameState class and handles
 *		- Tracking and updates the current state
 *
 * Original @author D. Green.
 *
 * � D. Green. 2021.
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
// Possible states. This enum can be modified to add additional states as required.
enum class State { MENU, LEVEL, GAMEOVER };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GameState
{
public:
	// Set the current state
	void setCurrentState(State s);
	// Returns the current state.
	State getCurrentState();

protected:
	State currentState;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////