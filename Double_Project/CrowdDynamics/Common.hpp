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

struct SWorldInfo
{
	//Struct of data required to create a 'world instance' - scene manager
public:
	float TimeStep = 0.0f;
	CVector2 WorldSize = CVector2(0.0f, 0.0f);
	int xSubdivisions = 0;
	int ySubdivisions = 0;
	float influenceSquaresPerUnit = 0;
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
