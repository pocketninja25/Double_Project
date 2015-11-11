#include "GAgent.h"

GAgent::GAgent(gen::CVector2 iPosition, gen::CVector2 iDestination, bool iIsActive) :
	GEntity(iPosition, iIsActive),
	m_Destination(iDestination)
{
}

GAgent::GAgent(float iXPos, float iYPos, float iXDest, float iYDest, bool iIsActive) : 
	GAgent(gen::CVector2(iXPos, iYPos), gen::CVector2(iXDest, iYDest), iIsActive)
{
	//Delagated Constructor - No Body
}

GAgent::~GAgent()
{
}

bool GAgent::HasReachedDestination()
{
	float distTo = gen::Abs(this->GetPosition().DistanceToSquared(m_Destination));

	if (distTo < 4.0f /*TODO: Make this less magic*/)	
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
	//Apply physics to model
	gen::CMatrix3x3 matrix = GetMatrix();	//Get matrix of this agent
	matrix.FaceTarget2D(m_Destination);		//Face the agent towards it's destination

	
	matrix.MoveLocalY2D(1.5f * updateTime);	//1.5 units per second - TODO: Make this less magic

	SetMatrix(matrix);
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
