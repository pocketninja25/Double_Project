#pragma once

#include <vector>
using std::vector;
#include "CVector2.h"
using gen::CVector2;

// /*Note for development*/ this might want to be an object so it has a UID - however probably wont have an update function

class GObstacleTemplate
{
//---------------------------
// Private Data Members
//---------------------------
private:
	vector<CVector2*> m_ObstacleMesh;			// /*Note for development*/ This is tricky, dont know how wall storage is best done when some walls are updated and others arent

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************
	
	GObstacleTemplate();
	
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