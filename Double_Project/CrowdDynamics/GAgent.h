#pragma once

#include "Common.h"
#include "GEntity.h"

class GAgent : public GEntity
{

//---------------------------
// Private Data Members
//---------------------------
private:
	gen::CVector2 m_Destination;	//The location in the scene the agent is attempting to reach
	float m_Mass;
	
//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************
	GAgent(gen::CVector2 iPosition, bool iIsActive = true);
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
	virtual std::string ToString();
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

