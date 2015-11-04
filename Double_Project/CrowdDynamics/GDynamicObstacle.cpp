#include "GDynamicObstacle.h"

GDynamicObstacle::GDynamicObstacle() : GObstacle()
{

}

GDynamicObstacle::~GDynamicObstacle()
{

}

void GDynamicObstacle::SetPosition(gen::CVector2 iXY)
{
	GetMatrix().SetRow(2, iXY);	//Set row 2 (position row) with new position
}

void GDynamicObstacle::SetPosition(float iX, float iY) 
{
	SetPosition(gen::CVector2(iX, iY));			//Delegate to alternate argument version of this function
}

void GDynamicObstacle::MoveX(float dX)
{
	gen::CMatrix3x3 mat = GetMatrix();
	mat.MoveX2D(dX);
	SetMatrix(mat);		
}

void GDynamicObstacle::MoveY(float dY)
{
	gen::CMatrix3x3 mat = GetMatrix();
	mat.MoveY2D(dY);
	SetMatrix(mat);
}

void GDynamicObstacle::Move(gen::CVector2 dXY)
{
	gen::CMatrix3x3 mat = GetMatrix();
	mat.Move2D(dXY);
	SetMatrix(mat);
}

void GDynamicObstacle::Move(float dX, float dY)
{
	Move(gen::CVector2(dX, dY));				//Delegate to alternate argument version of this function
}

void GDynamicObstacle::MoveLocalX(float dX)
{
	gen::CMatrix3x3 mat = GetMatrix();
	mat.MoveLocalX2D(dX);
	SetMatrix(mat);
}

void GDynamicObstacle::MoveLocalY(float dY)
{
	gen::CMatrix3x3 mat = GetMatrix();
	mat.MoveLocalY2D(dY);
	SetMatrix(mat);
}

void GDynamicObstacle::MoveLocal(gen::CVector2 dXY)
{
	gen::CMatrix3x3 mat = GetMatrix();
	mat.MoveLocal2D(dXY);
	SetMatrix(mat);
}

void GDynamicObstacle::MoveLocal(float dX, float dY)
{
	MoveLocal(gen::CVector2(dX, dY));			//Delegate to alternate argument version of this function
}

void GDynamicObstacle::RotateXDeg(float dAlpha)
{
	RotateXRad(gen::ToRadians(dAlpha));			//Convert then Delegate to alternate argument version of this function
}
void GDynamicObstacle::RotateXRad(float dAlpha)
{
	gen::CMatrix3x3 mat = GetMatrix();
	mat.RotateX(dAlpha);
	SetMatrix(mat);
}
void GDynamicObstacle::RotateYDeg(float dAlpha)
{
	RotateYRad(gen::ToRadians(dAlpha));			//Convert then Delegate to alternate argument version of this function
}
void GDynamicObstacle::RotateYRad(float dAlpha)
{
	gen::CMatrix3x3 mat = GetMatrix();
	mat.RotateY(dAlpha);
	SetMatrix(mat);
}

void GDynamicObstacle::RotateLocalXDeg(float dAlpha)
{
	RotateLocalXRad(gen::ToRadians(dAlpha));	//Convert then Delegate to alternate argument version of this function
}
void GDynamicObstacle::RotateLocalXRad(float dAlpha)
{
	gen::CMatrix3x3 mat = GetMatrix();
	mat.RotateLocalX(dAlpha);
	SetMatrix(mat);
}
void GDynamicObstacle::RotateLocalYDeg(float dAlpha)
{
	RotateLocalYRad(gen::ToRadians(dAlpha));	//Convert then Delegate to alternate argument version of this function
}
void GDynamicObstacle::RotateLocalYRad(float dAlpha)
{
	gen::CMatrix3x3 mat = GetMatrix();
	mat.RotateLocalY(dAlpha);
	SetMatrix(mat);
}


void GDynamicObstacle::Update(float updateTime)
{
	//TODO: implement
}

string GDynamicObstacle::ToString()
{
	stringstream builder;

	builder << GObstacle::ToString();
}