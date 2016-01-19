#pragma once

#include "Common.hpp"
#include "GEntity.hpp"
#include "GInfluenceMap.hpp"
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

	gen::CVector2 m_PreviousDesiredMovementVect;
	gen::CVector2 m_PreviousMovementVect;

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

	gen::CVector2 GetDesiredMovement();

	gen::CVector2 GetDestination();

	//***************************
	// Setters/Mutators
	//***************************
	void SetNewDestination(gen::CVector2 newDestination);	//Obtain a new goal location

	void SetPosition(gen::CVector2 newPosition);

	//***************************
	// Other Functions
	//***************************
	virtual void Update(float updateTime);		

	//void PerformCollisionAvoidance(const std::vector<GAgent*>& localAgents);	//Run global and local collision avoidance algorithms
	void CalculateInfluence(GInfluenceMap* influenceMap);
	void PerformGlobalCollisionAvoidance(const std::vector<GAgent*>& localAgents);
	void PerformLocalCollisionAvoidance(const std::vector<GAgent*>& localAgents);	

private:
	gen::CVector2 MoveTheDesiredVect(std::vector<SRestrictionObject> &restrictions, gen::CVector2 attemptedMovement, float updateTime);	//Used by PerformLocalCollisionAvoidance to manipulate the desired movement vector based on the passed restriction objects

#ifdef _DEBUG
public:
	virtual std::string ToString();
#endif

//---------------------------
// Private Functions
//---------------------------
private:
//***************************
// Other Functions
//***************************


//^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Debug code
//vvvvvvvvvvvvvvvvvvvvvvvvvvv
#ifdef _DEBUG
private:
	bool dm_BeingWatched;

public:
	void SetWatched(bool isWatched);

	bool GetWatched();
#endif
};

