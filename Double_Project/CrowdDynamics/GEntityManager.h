#pragma once

#include "Common.h"
#include <vector>

#include "GEntity.h"
#include "GAgent.h"

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

	//***************************
	// Other Functions
	//***************************

};

