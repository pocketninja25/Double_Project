#pragma once

#include "Common.h"
#include <vector>

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
	GSceneManager* mManager_Parent;		//Passed on construction, do not own this object, but as it owns GEntityManager, can assume that it will always exist as long as this class does

//---------------------------
// Private Data Members
//---------------------------
private:
	std::vector<GAgent*> m_ActiveAgents;
	std::vector<GAgent*> m_InactiveAgents;

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************

	GEntityManager(GSceneManager* iParentManager);

	virtual ~GEntityManager();

	//***************************
	// Getters/Accessors
	//***************************

	//***************************
	// Setters/Mutators
	//***************************
	void AddAgent(gen::CVector2 iPosition, bool iIsActive);
	void AddAgent(float iXPos, float iYPos, bool iIsActive);

	//***************************
	// Other Functions
	//***************************

	void Update(float updateTime);

	gen::CVector2 GetRandomDestination();		//Return a random position in the game world TODO: Make sure that position is not within an obstacle

};

