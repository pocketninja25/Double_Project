#pragma once
#include "GObstacle.h"


class GDynamicObstacle : public GObstacle
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
	GDynamicObstacle();
	virtual ~GDynamicObstacle();

	//***************************
	// Getters/Accessors
	//***************************

	//***************************
	// Setters/Mutators
	//***************************

	//Directly set the position of the obstacle
	void SetPosition(CVector2 iXY);
	void SetPosition(float iX, float iY);
	
	//Move the obstacle in the X and/or Y directions
	void MoveX(float dX);
	void MoveY(float dY);
	void Move(CVector2 dXY);
	void Move(float dX, float dY);

	//Move the obstacle in its local X and/or Y directions
	void MoveLocalX(float dX);
	void MoveLocalY(float dY);
	void MoveLocal(CVector2 dXY);
	void MoveLocal(float dX, float dY);

	//Rotate the obstacle (turn left/right) in degrees or radians
	void RotateXDeg(float dAlpha);
	void RotateXRad(float dAlpha);
	
	//***************************
	// Other Functions
	//***************************
	virtual void Update(float updateTime);

#ifdef _DEBUG
	virtual string ToString();
#endif
};
