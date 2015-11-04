#include "GSceneSquare.h"

GSceneSquare::GSceneSquare(gen::CVector2 iSize, gen::CVector2 iOriginPos, bool iIsActive) :
	GObject(iIsActive),
	m_Size(iSize),
	m_Origin(iOriginPos)
{
}

GSceneSquare::GSceneSquare(gen::CVector2 iSize, float iOriginXPos, float iOriginYPos, bool iIsActive) :
	GSceneSquare(iSize, gen::CVector2(iOriginXPos, iOriginYPos))
{
	//Delagated Constructor - No Body
}

GSceneSquare::GSceneSquare(float iXSize, float iYSize, gen::CVector2 iOriginPos, bool iIsActive) :
	GSceneSquare(gen::CVector2(iXSize, iYSize), iOriginPos, iIsActive)
{
	//Delagated Constructor - No Body
}

GSceneSquare::GSceneSquare(float iXSize, float iYSize, float iOriginXPos, float iOriginYPos, bool iIsActive) :
	GSceneSquare(gen::CVector2(iXSize, iYSize), gen::CVector2(iOriginXPos, iOriginYPos), iIsActive)
{
	//Delagated Constructor - No Body
}

GSceneSquare::~GSceneSquare()
{
}