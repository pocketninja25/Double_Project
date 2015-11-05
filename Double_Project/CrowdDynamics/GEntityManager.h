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

	//***************************
	// Other Functions
	//***************************

};

