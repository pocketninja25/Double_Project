#include "GStaticObstacle.h"

GStaticObstacle::GStaticObstacle(GObstacleTemplate* iTemplate, gen::CVector2 iPosition, bool iIsActive) :
	GObstacle(iTemplate, iPosition, iIsActive)
{
}

GStaticObstacle::~GStaticObstacle()
{
}

void GStaticObstacle::Update(float updateTime)
{
	//TODO: implement this function
}

string GStaticObstacle::ToString()
{
	stringstream builder;

	builder << GObstacle::ToString();

	return builder.str();
}