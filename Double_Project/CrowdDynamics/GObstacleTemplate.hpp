#pragma once

#include "Common.hpp"

#include "GObject.hpp"

class GObstacleTemplate : public GObject
{
//---------------------------
// Private Data Members
//---------------------------
private:
	std::map<int, CVector2> m_Vertices;
	std::vector<GIntPair> m_VertexPairs;

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