/*
 * This is the GameState class and handles
 *		- Tracking and updates the current state
 *
 * Original @author D. Green.
 *
 * ? D. Green. 2021.
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES
#include "GameState.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Set the current state
void GameState::setCurrentState(State s)
{
	currentState = s;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Returns the current state.
State GameState::getCurrentState()
{
	return currentState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////