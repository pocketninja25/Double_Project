#pragma once

#include "Common.hpp"
#include <map>

//Forward declare these classes, include the files for the cpp only (where needed)
class GEntity;
class GSceneManager;
class GAgent;

class GEntityManager
{
//---------------------------
// Manager classes
//---------------------------
private:

//---------------------------
// Private Data Members
//---------------------------
private:
	std::map<UID, GAgent*> m_ActiveAgents;
	std::map<UID, GAgent*> m_InactiveAgents;

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

	bool GetAgent(UID request, GAgent* &returnedAgent);

	//***************************
	// Setters/Mutators
	//***************************
	UID AddAgent(gen::CVector2 iPosition, bool iIsActive);
	UID AddAgent(float iXPos, float iYPos, bool iIsActive);

	//***************************
	// Other Functions
	//***************************

	void Update(float updateTime);

	gen::CVector2 GetRandomDestination();		//Return a random position in the game world TODO: Make sure that position is not within an obstacle

};
