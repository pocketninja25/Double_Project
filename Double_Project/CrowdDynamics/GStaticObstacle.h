#pragma once

#include "Common.h"
#include "GObstacle.h"


class GStaticObstacle :	public GObstacle
{
//---------------------------
// Private Data Members
//---------------------------
private:

//---------------------------
// Public Functions
//---------------------------
public:

	//***************************
	// Constructors/Destructors
	//***************************
	GStaticObstacle(GObstacleTemplate* iTemplate, gen::CVector2 iPosition, bool iIsActive);
	virtual ~GStaticObstacle();

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
