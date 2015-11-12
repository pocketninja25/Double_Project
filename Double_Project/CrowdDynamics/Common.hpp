#pragma once

//All headers will include this file, it will contain the core includes/variables etc that each file may need

#include "MathsLib.h"

typedef int UID;


inline float RandomFloat(float min, float max)	//Returns a random float between min and max
{
	float random = ((float)rand()) / (float)RAND_MAX;

	float range = max - min;
	return (random * range) + min;
}
