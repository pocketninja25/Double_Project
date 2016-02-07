#include "GStaticObstacle.hpp"

GStaticObstacle::GStaticObstacle(GObstacleTemplate* iTemplate, CVector2 iPosition, bool iIsActive) :
	GObstacle(iTemplate, iPosition, iIsActive)
{
}

GStaticObstacle::~GStaticObstacle()
{
}

void GStaticObstacle::Update(float updateTime)
{
	GObstacle::Update(updateTime);

	//TODO: implement this function
}

#ifdef _DEBUG
std::string GStaticObstacle::ToString()
{
	std::stringstream builder;

	builder << GObstacle::ToString();

	return builder.str();
}
#endif