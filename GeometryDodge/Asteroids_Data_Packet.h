#pragma once
#include "AsteroidDataMsg.h"
#include <vector>

struct Asteroids_Data_Packet
{
	int asteroidDataMsgSize = 0;
	std::vector<AsteroidDataMsg*> asteroidDataMsgs;
};