#pragma once

#include "Common.h"
#include <vector>

class GEntity;	//Forward Declaration of Entity class

class GEntityManager
{
//---------------------------
// Private Data Members
//---------------------------
private:
	vector<GEntity*> m_Entities;

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************

	GEntityManager();

	virtual ~GEntityManager();

	//***************************
	// Getters/Accessors
	//***************************

	//***************************
	// Setters/Mutators
	//***************************
	void AddAgent(/*SomeAgentDetails*/);

	// /*Note for Development*/ Might be worth having these two functions private and use an AddObstacle function that accepts an enum (static/dynamic) - depends on what these constructions look like
	void AddStaticObstacle(/*SomeObstacleDetails*/);
	void AddDynamicObstacle(/*SomeObstacleDetails*/);

	//***************************
	// Other Functions
	//***************************

};

