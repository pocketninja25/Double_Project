#include "GSceneSquare.h"

GSceneSquare::GSceneSquare(CVector2 iSize, CVector2 iOriginPos, bool iIsActive = true) : 
	GObject(iIsActive),
	m_Size(iSize),
	m_Origin(iOriginPos)
{
}

GSceneSquare::GSceneSquare(CVector2 iSize, float iOriginXPos, float iOriginYPos, bool iIsActive = true) : 
	GSceneSquare(iSize, CVector2(iOriginXPos, iOriginYPos))
{
	//Delagated Constructor - No Body
}

GSceneSquare::GSceneSquare(float iXSize, float iYSize, CVector2 iOriginPos, bool iIsActive) :
	GSceneSquare(CVector2(iXSize, iYSize), iOriginPos, iIsActive)
{
	//Delagated Constructor - No Body
}

GSceneSquare::GSceneSquare(float iXSize, float iYSize, float iOriginXPos, float iOriginYPos, bool iIsActive) :
	GSceneSquare(CVector2(iXSize, iYSize), CVector2(iOriginXPos, iOriginYPos), iIsActive)
{
	//Delagated Constructor - No Body
}

GSceneSquare::~GSceneSquare()
{
}