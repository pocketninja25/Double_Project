#include "GAgent.h"

GAgent::GAgent(gen::CVector2 iPosition, gen::CVector2 iDestination, bool iIsActive) :
	GEntity(iPosition, iIsActive),
	m_Destination(iDestination),
	m_Velocity(1.5f)		//TODO: find a way of calculating this better
{
	m_MovementVect = gen::CVector2(GetMatrix().Position2D(), m_Destination);
	m_MovementVect.Normalise();
	m_MovementVect * m_Velocity;
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
	
	//Calculate the new movement vector
	m_MovementVect = gen::CVector2(GetMatrix().Position2D(), m_Destination);
	m_MovementVect.Normalise();
	m_MovementVect * m_Velocity;
}

void GAgent::Update(float updateTime)
{
	//Apply physics to model
	gen::CMatrix3x3 matrix = GetMatrix();	//Get matrix of this agent
	matrix.FaceTarget2D(m_Destination);		//Face the agent towards it's destination

	//Move by the movement vector (units per second) modified by update time calculating units this frame
	matrix.Move2D(m_MovementVect * updateTime);

	SetMatrix(matrix);
}

#ifdef _DEBUG
std::string GAgent::ToString()
{
	std::stringstream builder;

	builder << GEntity::ToString() << "Destination: X: " << m_Destination.x << " Y: " << m_Destination.y << "\n"
		<< "Velocity: " << m_Velocity << "\n"
		<< "MovementVect: X: " << m_MovementVect.x << "Y: " << m_MovementVect.y << "\n";

	return builder.str();
}
#endif
