#include "GObstacle.h"

#include "GObstacleTemplate.h"
#include "GWall.h"

GObstacle::GObstacle(GObstacleTemplate* iTemplate, gen::CVector2 iPosition, bool iIsActive) :
	GObject(iIsActive),
	m_Template(iTemplate),
	m_Matrix(iPosition)
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

gen::CVector2 GObstacle::GetPosition()
{
	return m_Matrix.GetPosition2D();
}

gen::CVector2 GObstacle::GetFacingVector()
{
	return gen::CVector2(m_Matrix.GetRow(1));	//Get the 2nd 'Facing' row of the matrix
}

void GObstacle::Update(float updateTime)
{
	//TODO: complete this function
}

string GObstacle::ToString()
{
	stringstream builder;
	
	builder << GObject::ToString() << "Position: X: " << m_Matrix.GetPosition2D().x << " Y: " << m_Matrix.GetPosition2D().y << "/n" 
		<< "Template UID: " << m_Template->GetUID() << "/n"
		<< "NoOfWalls: " << m_Walls.size() << "/n";

	return builder.str();
}

