#pragma once

#include "Common.h"

#include "GObject.h"
#include <vector>

class GObstacleTemplate : public GObject
{
//---------------------------
// Private Data Members
//---------------------------
private:
	std::vector<gen::CVector2*> m_ObstacleMesh;			// TODO: This is tricky, dont know how wall storage is best done when some walls are updated and others arent

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