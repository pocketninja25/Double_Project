#include "GObstacle.h"

GObstacle::GObstacle(GObstacleTemplate* iTemplate, gen::CVector2 iPosition, bool iIsActive) :
	GEntity(iPosition, iIsActive),
	m_Template(iTemplate)
{
	//TODO: Do something to figure out what the walls are - need to construct the walls probably
	//TODO: Should i error check the template, if so how to deal with error
}

GObstacle::~GObstacle()
{
	for (auto &wall : m_Walls)	//Delete the dynamically allocated walls in m_Walls
	{
		delete wall;
	}

	//Vector m_Walls deallocates itself
	//Dont need to deallocate template as dont own it
}

void GObstacle::Update(float updateTime)
{
	//TODO: complete this function
}

string GObstacle::ToString()
{
	stringstream builder;
	
	builder << GEntity::ToString() << "Template UID: " << m_Template->GetUID() << "/n"
		<< "NoOfWalls: " << m_Walls.size() << "/n";

	return builder.str();
}