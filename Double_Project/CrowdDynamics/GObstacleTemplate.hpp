#pragma once

#include "Common.hpp"
#include "GObject.hpp"

struct SWall
{
	GIntPair vertexIndices;
	CVector2 facingVector;
};

class GObstacleTemplate : public GObject
{
//---------------------------
// Private Data Members
//---------------------------
private:
	std::map<int, CVector2> m_Vertices;
	std::vector<SWall> m_VertexPairs;
	
	//Current method only supports axis aligned objects
	CVector2 m_BottomLeft;	//The point at the lowest left point of the "bounding box" around the obstacle
	CVector2 m_TopRight;	//The point at the upper right point of the "bounding box" around the obstacle

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************
	GObstacleTemplate(bool iIsActive = true);
	
	virtual ~GObstacleTemplate();

	//***************************
	// Getters/Accessors
	//***************************
	CVector2 GetVertex(int index)
	{
		return m_Vertices[index];
	}
	const std::vector<SWall>& GetVertexPairs()
	{
		return m_VertexPairs;
	}
	CVector2 GetBottomLeft()
	{
		return m_BottomLeft;
	}
	CVector2 GetTopRight()
	{
		return m_TopRight;
	}

	//***************************
	// Setters/Mutators
	//***************************
	void AddVertex(int index, CVector2 vertex);
	void AddVertexPair(GIntPair vertexPair);

	//***************************
	// Other Functions
	//***************************
	virtual void Update(float updateTime);

};