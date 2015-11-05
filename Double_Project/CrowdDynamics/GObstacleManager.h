#pragma once

#include "Common.h"
#include <vector>

//Forward declarations of obstacle classes
class GStaticObstacle;
class GDynamicObstacle;
class GObstacleTemplate;

class GObstacleManager
{
//---------------------------
// Private Data Members
//---------------------------
private:
	vector<GObstacleTemplate*> m_ObstacleTemplates;

	//Sorted into two categories for update tree management - hopefully wont need to update static obstacles each timestep
	vector<GStaticObstacle*> m_StaticObstacles;
	vector<GDynamicObstacle*> m_DynamicObstacles;

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************

	GObstacleManager();

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

