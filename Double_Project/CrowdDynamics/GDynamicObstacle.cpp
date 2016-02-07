#include "GDynamicObstacle.hpp"

GDynamicObstacle::GDynamicObstacle(GObstacleTemplate* iTemplate, CVector2 iPosition, bool iIsActive) : 
	GObstacle(iTemplate, iPosition, iIsActive)
{

}

GDynamicObstacle::~GDynamicObstacle()
{

}

void GDynamicObstacle::SetPosition(CVector2 iXY)
{
	m_Matrix.SetRow(2, iXY);	//Set row 2 (position row) with new position
}

void GDynamicObstacle::SetPosition(float iX, float iY) 
{
	SetPosition(CVector2(iX, iY));			//Delegate to alternate argument version of this function
}

void GDynamicObstacle::MoveX(float dX)
{
	m_Matrix.MoveX2D(dX);
}

void GDynamicObstacle::MoveY(float dY)
{
	m_Matrix.MoveY2D(dY);
}

void GDynamicObstacle::Move(CVector2 dXY)
{
	m_Matrix.Move2D(dXY);
}

void GDynamicObstacle::Move(float dX, float dY)
{
	Move(CVector2(dX, dY));				//Delegate to alternate argument version of this function
}

void GDynamicObstacle::MoveLocalX(float dX)
{
	m_Matrix.MoveLocalX2D(dX);
}

void GDynamicObstacle::MoveLocalY(float dY)
{
	m_Matrix.MoveLocalY2D(dY);
}

void GDynamicObstacle::MoveLocal(CVector2 dXY)
{
	m_Matrix.MoveLocal2D(dXY);
}

void GDynamicObstacle::MoveLocal(float dX, float dY)
{
	MoveLocal(CVector2(dX, dY));			//Delegate to alternate argument version of this function
}

void GDynamicObstacle::RotateXDeg(float dAlpha)
{
	RotateXRad(gen::ToRadians(dAlpha));			//Convert then Delegate to alternate argument version of this function
}
void GDynamicObstacle::RotateXRad(float dAlpha)
{
	m_Matrix.RotateX(dAlpha);
}
void GDynamicObstacle::RotateYDeg(float dAlpha)
{
	RotateYRad(gen::ToRadians(dAlpha));			//Convert then Delegate to alternate argument version of this function
}
void GDynamicObstacle::RotateYRad(float dAlpha)
{
	m_Matrix.RotateY(dAlpha);
}

void GDynamicObstacle::RotateLocalXDeg(float dAlpha)
{
	RotateLocalXRad(gen::ToRadians(dAlpha));	//Convert then Delegate to alternate argument version of this function
}
void GDynamicObstacle::RotateLocalXRad(float dAlpha)
{
	m_Matrix.RotateLocalX(dAlpha);
}
void GDynamicObstacle::RotateLocalYDeg(float dAlpha)
{
	RotateLocalYRad(gen::ToRadians(dAlpha));	//Convert then Delegate to alternate argument version of this function
}
void GDynamicObstacle::RotateLocalYRad(float dAlpha)
{
	m_Matrix.RotateLocalY(dAlpha);
}


void GDynamicObstacle::Update(float updateTime)
{
	GObstacle::Update(updateTime);
}

#ifdef _DEBUG
std::string GDynamicObstacle::ToString()
{
	std::stringstream builder;

	builder << GObstacle::ToString();
	return builder.str();
}
#endif