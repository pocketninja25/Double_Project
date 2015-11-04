#include "GWall.h"

//TODO: Improve this constructor
GWall::GWall(gen::CVector2 iVertexA, gen::CVector2 iVertexB, gen::CVector2 iFacingVector, UID iObstacle, bool iIsActive) : 
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

string GWall::ToString()
{
	stringstream builder;

	builder << GObject::ToString() << "Vertices: A: " << m_Vertices.a.x << ", " << m_Vertices.a.y
		<< " B: " << m_Vertices.b.x << ", " << m_Vertices.b.y << "/n"
		<< "Facing: " << m_FacingVect.x << ", " << m_FacingVect.y
		<< "Parent: " << m_Parent << "/n";

	return builder.str;
}

bool GWall::IsInFront(gen::CVector2 objectPosition)	//Return true if the passed object is in front of the wall
{
	//Take vector from either point on the wall to the object
	gen::CVector2 toObject = objectPosition - m_Vertices.a;
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