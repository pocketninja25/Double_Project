#pragma once

#include "Common.hpp"
#include "GEntity.hpp"

struct SRestrictionObject;

class GAgent : public GEntity
{
//---------------------------
// Private Data Members
//---------------------------
private:
	gen::CVector2 m_Destination;	//The location in the scene the agent is attempting to reach
	float m_Velocity;				//The velocity of the agent in units per second
	gen::CVector2 m_DesiredMovementVect;	//The desired movement vector of the agent in units per second
	float m_Radius;
	gen::CVector2 m_MovementVector;

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************
	GAgent(gen::CVector2 iPosition, gen::CVector2 iDestination, bool iIsActive = true);
	GAgent(float iXPos = 0.0f, float iYPos = 0.0f, float iXDest = 0.0f, float iYDest = 0.0f, bool iIsActive = true);

	virtual ~GAgent();
	
	//***************************
	// Getters/Accessors
	//***************************
	bool HasReachedDestination();	//Returns whether or not the agent has reached their destination

	//***************************
	// Setters/Mutators
	//***************************
	void SetNewDestination(gen::CVector2 newDestination);	//Obtain a new goal location

	//***************************
	// Other Functions
	//***************************
	virtual void Update(float updateTime);		
	void ComputePossibleVelocities(const std::vector<GAgent*>& localAgents);
private:
	gen::CVector2 MoveDesiredVect(std::vector<SRestrictionObject> &restrictions, gen::CVector2 attemptedMovement);
public:

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

};

