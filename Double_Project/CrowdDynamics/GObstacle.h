#pragma once

#include "Common.h"
#include "GEntity.h"
#include <vector>

#include "GObstacleTemplate.h"
#include "GWall.h"

class GObstacle : public GEntity
{

//---------------------------
// Private Data Members
//---------------------------
private:
	GObstacleTemplate* m_Template;	//The template this obstacle is based on TODO: this may be better as a UID, but likely this should be one of the only direct pointers outside of managers

	vector<GWall*> m_Walls;			//Vector of the walls owned by this model

//---------------------------
// Public Functions
//---------------------------
public:

	//***************************
	// Constructors/Destructors
	//***************************
	GObstacle(GObstacleTemplate* iTemplate, gen::CVector2 iPosition, bool iIsActive = true);
	virtual ~GObstacle();
	
	//***************************
	// Getters/Accessors
	//***************************

	//***************************
	// Setters/Mutators
	//***************************

	//***************************
	// Other Functions
	//***************************
	virtual void Update(float updateTime);

#ifdef _DEBUG
	virtual string ToString();
#endif
};
