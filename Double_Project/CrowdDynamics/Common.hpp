#pragma once

//All headers will include this file, it will contain the core includes/variables etc that each file may need

#include "MathsLib.h"

#include <list>
#include <vector>
#include <map>

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

inline bool stobool(std::string theString)
{
	if (theString == "true")
	{
		return true;
	}
	return false;
}

struct SWorldBlueprint
{
	//Struct of data required to create a 'world instance' - scene manager
public:
	float TimeStep = 0.0f;
	CVector2 WorldSize = CVector2(0.0f, 0.0f);
	GIntPair subdivisions = GIntPair(0, 0);
	float influenceSquaresPerUnit = 0;
	GIntPair influenceSubdivisions = GIntPair(0, 0);
	std::map<std::string, int> agentDetails;	//Store blueprint file name & how many of that agent blueprint
};

struct SAgentBlueprint {
public:
	float radius;
	bool randomPosition;
	CVector2 position;
	bool randomDestination;
	CVector2 destination;
	bool startsActive;
	float velocity;
	float defaultTurningArc;	//In radians
	float maxGradientTraversal; //Not currently in use

};
