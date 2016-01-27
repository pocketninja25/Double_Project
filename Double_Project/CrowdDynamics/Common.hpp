#pragma once

//All headers will include this file, it will contain the core includes/variables etc that each file may need

#include "MathsLib.h"

#include <list>
#include <vector>

#include "GIntPair.hpp"

using gen::CVector2;
using gen::CVector3;
using gen::CMatrix3x3;

typedef int UID;

inline float RandomFloat(float min, float max)	//Returns a random float between min and max
{
	float random = ((float)rand()) / (float)RAND_MAX;

	float range = max - min;
	return (random * range) + min;
}
