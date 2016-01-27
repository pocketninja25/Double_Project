#include "GWall.hpp"

//TODO: Improve this constructor
GWall::GWall(CVector2 iVertexA, CVector2 iVertexB, CVector2 iFacingVector, UID iObstacle, bool iIsActive) : 
	GObject(iIsActive),
	m_Vertices(VertexPair(iVertexA, iVertexB)),
	m_FacingVect(iFacingVector),
	m_Parent(iObstacle)
{
}

GWall::~GWall()
{
}

void GWall::Update(float updateTime)
{
	//TODO: fill in this function
}

#ifdef _DEBUG
std::string GWall::ToString()
{
	std::stringstream builder;

	builder << GObject::ToString() << "Vertices: A: " << m_Vertices.a.x << ", " << m_Vertices.a.y
		<< " B: " << m_Vertices.b.x << ", " << m_Vertices.b.y << "/n"
		<< "Facing: " << m_FacingVect.x << ", " << m_FacingVect.y
		<< "Parent: " << m_Parent << "/n";

	return builder.str();
}
#endif

bool GWall::IsInFront(CVector2 objectPosition)	//Return true if the passed object is in front of the wall
{
	//Take vector from either point on the wall to the object
	CVector2 toObject = objectPosition - m_Vertices.a;
	toObject.Normalise();	//Make vector to object a normalised vector

	//Take facing vector and perform dot product with toObject

	if (m_FacingVect.Dot(toObject) > 0)
	{
		//Dot product > 0, angle < 90 degrees, in front
		return true;
	}
	//Dot product <= 0, angle >= 90 degrees, orthogonal or behind
	return false;
	

	//TODO: Test this
}