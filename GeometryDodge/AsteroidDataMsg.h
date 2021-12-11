#pragma once

struct AsteroidDataMsg
{
	// An ID number associated with the corrosponding asteroid
	int asteroidID;

	// The velocity data of this asteroid
	float x;
	float y;

	// The time this msg was sent
	float timeSent;
};