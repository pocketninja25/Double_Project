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

void GSceneSquare::Update(float updateTime)
{
	//TODO: Fill this in
}

#ifdef _DEBUG
std::string GSceneSquare::ToString()
{
	std::stringstream builder;

	builder << GObject::ToString() << "Square Width: " << m_Size.x << " Square Height " << m_Size.y << "\n"
		<< "Square Origin: " << m_Origin << "\n"
		<< "No of walls: " << m_Walls.size() << "\n";

	return builder.str();
}

#endif