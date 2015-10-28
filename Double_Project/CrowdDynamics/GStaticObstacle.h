#pragma once
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
	GStaticObstacle();
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

	virtual string ToString();
};
