#include "GAgent.h"

GAgent::GAgent(gen::CVector2 iPosition, bool iIsActive) :
	GEntity(iPosition, iIsActive)
{
}

GAgent::GAgent(float iXPos, float iYPos, bool iIsActive) : 
	GAgent(gen::CVector2(iXPos, iYPos), iIsActive)
{
	//Delagated Constructor - No Body
}

GAgent::~GAgent()
{
}

bool GAgent::HasReachedDestination()
{
	if (this->GetPosition().DistanceToSquared(m_Destination) < 1.0f /*TODO: Make this less magic*/)	//Can use DistanceSquared because for 0<x<1 xsquared < 1
	{
		return true;
	}
	return false;
}

void GAgent::SetNewDestination(gen::CVector2 newDestination)
{
	//Recieve a new destination (from the manager class) when the manager determines the agent has arrived, or should change destination otherwise
	m_Destination = newDestination;
}

void GAgent::Update(float updateTime)
{
	gen::CMatrix3x3 matrix = GetMatrix();
	matrix.FaceTarget2D(m_Destination);

	matrix.MoveLocalY2D(1.5f * updateTime);	//TODO: Make this less magic
}

#ifdef _DEBUG
std::string GAgent::ToString()
{
	std::stringstream builder;

	builder << GEntity::ToString() << "Destination: X: " << m_Destination.x << " Y: " << m_Destination.y << "\n" 
		<< "Mass: " << m_Mass << "\n";

	return builder.str();
}
#endif
