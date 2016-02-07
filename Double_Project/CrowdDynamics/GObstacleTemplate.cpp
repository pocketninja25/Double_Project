#include "GObstacleTemplate.hpp"

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

void GObstacleTemplate::AddVertex(int index, CVector2 vertex)
{
	m_Vertices.emplace(std::make_pair(index, vertex));
}

void GObstacleTemplate::AddVertexPair(GIntPair vertexPair)
{
	m_VertexPairs.push_back(vertexPair);
}

void GObstacleTemplate::Update(float updateTime)
{
}
