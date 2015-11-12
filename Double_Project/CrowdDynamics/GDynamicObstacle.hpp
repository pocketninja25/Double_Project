#pragma once

#include "Common.hpp"
#include "GObstacle.hpp"


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
	GDynamicObstacle(GObstacleTemplate* iTemplate, gen::CVector2 iPosition, bool iIsActive = true);
	virtual ~GDynamicObstacle();

	//***************************
	// Getters/Accessors
	//***************************

	//***************************
	// Setters/Mutators
	//***************************

	//Directly set the position of the obstacle
	void SetPosition(gen::CVector2 iXY);
	void SetPosition(float iX, float iY);
	
	//Move the obstacle in the X and/or Y directions
	void MoveX(float dX);
	void MoveY(float dY);
	void Move(gen::CVector2 dXY);
	void Move(float dX, float dY);

	//Move the obstacle in its local X and/or Y directions
	void MoveLocalX(float dX);
	void MoveLocalY(float dY);
	void MoveLocal(gen::CVector2 dXY);
	void MoveLocal(float dX, float dY);

	//Rotate the obstacle (turn left/right (world space)) in degrees or radians
	void RotateXDeg(float dAlpha);
	void RotateXRad(float dAlpha);
	void RotateYDeg(float dAlpha);
	void RotateYRad(float dAlpha);

	//Rotate the obstacle (turn left/right (local space)) in degrees or radians
	void RotateLocalXDeg(float dAlpha);
	void RotateLocalXRad(float dAlpha);
	void RotateLocalYDeg(float dAlpha);
	void RotateLocalYRad(float dAlpha);

	//***************************
	// Other Functions
	//***************************
	virtual void Update(float updateTime);

#ifdef _DEBUG
	virtual std::string ToString();
#endif
};
