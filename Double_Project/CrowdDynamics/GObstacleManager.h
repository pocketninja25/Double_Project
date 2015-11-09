#pragma once

#include "Common.h"
#include <vector>

//Forward declarations of obstacle classes
class GStaticObstacle;
class GDynamicObstacle;
class GObstacleTemplate;
class GSceneManager;

class GObstacleManager
{
//---------------------------
// Manager classes
//---------------------------
private:
	GSceneManager* mManager_Parent;		//Passed on construction, do not own this object, but as it owns GSceneManager, can assume that it will always exist as long as this class does

//---------------------------
// Private Data Members
//---------------------------
private:
	std::vector<GObstacleTemplate*> m_ObstacleTemplates;

	//Sorted into two categories for update tree management - hopefully wont need to update static obstacles each timestep
	std::vector<GStaticObstacle*> m_StaticObstacles;
	std::vector<GDynamicObstacle*> m_DynamicObstacles;

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************

	GObstacleManager(GSceneManager* iParent);

	virtual ~GObstacleManager();

	//***************************
	// Getters/Accessors
	//***************************

	//***************************
	// Setters/Mutators
	//***************************

	// TODO: Might be worth having these two functions private and use an AddObstacle function that accepts an enum (static/dynamic) - depends on what these constructions look like
	void AddStaticObstacle(/*SomeObstacleDetails*/);
	void AddDynamicObstacle(/*SomeObstacleDetails*/);

	//***************************
	// Other Functions
	//***************************
};

