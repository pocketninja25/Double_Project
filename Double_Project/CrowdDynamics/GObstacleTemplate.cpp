#include "GObstacleTemplate.hpp"

GObstacleTemplate::GObstacleTemplate(bool iIsActive) :
	GObject(iIsActive),
	m_BottomLeft(CVector2(0.0f, 0.0f)),
	m_TopRight(CVector2(0.0f, 0.0f))
{
}

GObstacleTemplate::~GObstacleTemplate()
{
}

void GObstacleTemplate::AddVertex(int index, CVector2 vertex)
{

	if (vertex.x < m_BottomLeft.x)
	{
		m_BottomLeft.x = vertex.x;
	}
	if (vertex.y < m_BottomLeft.y)
	{
		m_BottomLeft.y = vertex.y;
	}
	if (vertex.x > m_TopRight.x)
	{
		m_TopRight.x = vertex.x;
	}
	if (vertex.y > m_TopRight.y)
	{
		m_TopRight.y = vertex.y;
	}

	m_Vertices.emplace(std::make_pair(index, vertex));
}

void GObstacleTemplate::AddVertexPair(GIntPair vertexPair)
{
	SWall newWall;

	newWall.vertexIndices = vertexPair;

	//				 y					Shows how the normal is calculated with respect to vertices x and y
	//				 ^
	//				 l
	//   Normal<-----l
	//				 l
	//				 x
	
	CVector2 facingBuilder = m_Vertices[vertexPair.y] - m_Vertices[vertexPair.x];
	facingBuilder.Normalise();
	facingBuilder.SetPerpendicular();

	newWall.facingVector = facingBuilder;
	
	m_VertexPairs.push_back(newWall);
}

void GObstacleTemplate::Update(float updateTime)
{
}
