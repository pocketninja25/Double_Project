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
	//Constants that define movement restrictions
	float m_Velocity;				//The maximum velocity of the agent in units per second
	float km_MaxTurn;				//The defualt maximum rotation an agent can perform per second (in radians)
	float m_TempMaxTurn;			//The (potentially) modified max turn to deal with agent standstills
	float m_MaxGradientTraversal;	//THe maximum gradient increase an agent can climb

	CVector2 m_Destination;	//The location in the scene the agent is attempting to reach
	
	CVector2 m_DesiredMovementVect;	//The desired movement vector of the agent in units per second
	float m_Radius;
	CVector2 m_MovementVector;

	CVector2 m_PreviousDesiredMovementVect;
	CVector2 m_PreviousMovementVect;

	std::vector<GAgent*> m_PotentiallyCollidingAgents;

	float m_StillTimer;

	enum Directions {
		Dir_BottomLeft = 0,
		Dir_Left = 1,
		Dir_TopLeft = 2,
		Dir_Bottom = 3,
		Dir_Centre = 4,
		Dir_Top = 5,
		Dir_BottomRIght = 6,
		Dir_Right = 7,
		Dir_TopRight = 8,
		Dir_Size = 9
	};
	
//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************
	GAgent(SAgentBlueprint &agentDetails);
	GAgent(CVector2 iPosition, CVector2 iDestination, bool iIsActive = true);	//Deprecated
	GAgent(float iXPos = 0.0f, float iYPos = 0.0f, float iXDest = 0.0f, float iYDest = 0.0f, bool iIsActive = true);	//Deprecated

	virtual ~GAgent();
	
	//***************************
	// Getters/Accessors
	//***************************
	bool HasReachedDestination();	//Returns whether or not the agent has reached their destination

	CVector2 GetDesiredMovement();

	CVector2 GetDestination();

	//***************************
	// Setters/Mutators
	//***************************
	void SetNewDestination(CVector2 newDestination);	//Obtain a new goal location

	void SetPosition(CVector2 newPosition);

	//***************************
	// Other Functions
	//***************************
	virtual void Update(float updateTime);		

	void CalculateInfluence(GInfluenceMap* influenceMap);
	void GiveLocalAgentsList(std::vector<GAgent*> localAgents);
private:
	void PerformGlobalCollisionAvoidance();
	void PerformLocalCollisionAvoidance();

private:
	CVector2 MoveTheDesiredVect(std::vector<SRestrictionObject> &restrictions, CVector2 attemptedMovement, float updateTime);	//Used by PerformLocalCollisionAvoidance to manipulate the desired movement vector based on the passed restriction objects

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
private:
	bool dm_BeingWatched;

public:
	void SetWatched(bool isWatched);

	bool GetWatched();
};

