#pragma once

#include "GEntity.h"

class GAgent : public GEntity
{
//---------------------------
// Private Data Members
//---------------------------
private:
	CVector2 m_Destination;	//The location in the scene the agent is attempting to reach
	float m_Mass;

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************
	GAgent(CVector2 iPosition = CVector2(0.0f, 0.0f), bool iIsActive = true);
	GAgent(float iXPos = 0.0f, float iYPos = 0.0f, bool iIsActive = true);

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

#ifdef _DEBUG
	virtual string ToString();
#endif

//---------------------------
// Private Functions
//---------------------------
private:
	//***************************
	// Other Functions
	//***************************
	void GetNewDestination();	//Obtain a new goal location

};

