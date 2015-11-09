#include "GObstacleTemplate.h"

GObstacleTemplate::GObstacleTemplate(bool iIsActive) :
	GObject(iIsActive)
{
	//TODO: Load obstacle mesh (need filename or something, maybe a list of vertices)
}

GObstacleTemplate::~GObstacleTemplate()
{
	for (auto &vertex : m_ObstacleMesh)	//Deallocate pointer data
	{
		delete vertex;
	}
	//m_ObstacleMesh is automatically deallocated
}