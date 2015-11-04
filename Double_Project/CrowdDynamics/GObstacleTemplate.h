#pragma once

#include "Common.h"

#include "GObject.h"
#include <vector>

// TODO: this might want to be an object so it has a UID - however probably wont have an update function

class GObstacleTemplate : public GObject
{
//---------------------------
// Private Data Members
//---------------------------
private:
	vector<gen::CVector2*> m_ObstacleMesh;			// TODO: This is tricky, dont know how wall storage is best done when some walls are updated and others arent

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************
	
	GObstacleTemplate(bool iIsActive = true);
	
	virtual ~GObstacleTemplate();

	//***************************
	// Getters/Accessors
	//***************************

	//***************************
	// Setters/Mutators
	//***************************

	//***************************
	// Other Functions
	//***************************

};