#pragma once

#include "GEntity.h"
using gen::CVector3;

class GAgent : public GEntity
{
//---------------------------
// Private Data Members
//---------------------------
private:
	CVector3 m_Destination;	//The location in the scene the agent is attempting to reach
	float m_Mass;

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************
	GAgent();
	
	virtual ~GAgent();

	//***************************
	// Getters/Accessors
	//***************************

	//***************************
	// Setters/Mutators
	//***************************

	//***************************
	// Other Functions
	//***************************

	virtual void Update(float updateTime);	

	virtual string ToString();

//---------------------------
// Private Functions
//---------------------------
private:
	//***************************
	// Other Functions
	//***************************
	void GetNewDestination();	//Obtain a new goal location

};

