#include "GAgent.hpp"
#include "GSceneManager.hpp"

GAgent::GAgent(gen::CVector2 iPosition, gen::CVector2 iDestination, bool iIsActive) :
	GEntity(iPosition, iIsActive),
	m_Destination(iDestination),
	m_Velocity(1.5f),//TODO: find a way of calculating this better
	m_Radius(sqrt(200.0f)/2)	//TODO: provide a reliable version of this number based on file data
{
	m_DesiredMovementVect = gen::CVector2(GetPosition(), m_Destination);
	m_DesiredMovementVect.Normalise();
	m_DesiredMovementVect * m_Velocity;
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
	m_DesiredMovementVect = m_Destination - GetPosition();
	m_DesiredMovementVect.Normalise();
	m_DesiredMovementVect * m_Velocity;
}
#include <assert.h>

void GAgent::Update(float updateTime)
{
	//Apply physics to model
	gen::CMatrix3x3 matrix = GetMatrix();	//Get matrix of this agent
	matrix.FaceTarget2D(m_Destination);		//Face the agent towards it's destination

	//Move by the movement vector (units per second) modified by update time calculating units this frame
	matrix.Move2D(m_MovementVector * updateTime);

	m_DesiredMovementVect = m_Destination - GetPosition();
	m_DesiredMovementVect.Normalise();
	m_DesiredMovementVect * m_Velocity;

	SetMatrix(matrix);

}

#ifdef _DEBUG
std::string GAgent::ToString()
{
	std::stringstream builder;

	builder << GEntity::ToString() << "Destination: X: " << m_Destination.x << " Y: " << m_Destination.y << "\n"
		<< "Velocity: " << m_Velocity << "\n"
		<< "DesiredMovementVect: X: " << m_DesiredMovementVect.x << "Y: " << m_DesiredMovementVect.y << "\n";

	return builder.str();
}
#endif

struct SRestrictionObject
{
	gen::CVector2 leftEdge;
	gen::CVector2 rightEdge;
	GAgent* associatedAgent;
};

void GAgent::ComputePossibleVelocities(const std::vector<GAgent*>& localAgents)
{
	float updateTime = GSceneManager::GetInstance()->GetTimeStep();
	std::vector<SRestrictionObject> restrictions;
	SRestrictionObject buildRestriction;
	for (auto otherAgent : localAgents)
	{
		//Test to see if this agent is the agent referenced in agent, if so then skip calcualtions for that agent
		if (otherAgent->GetUID() != this->GetUID())
		{
			//Determine the distance that the other agent can travel in one frame
			float mySpeed = this->m_DesiredMovementVect.Length();
			float otherSpeed = otherAgent->m_DesiredMovementVect.Length();
			float myRadius = this->m_Radius;
			float otherRadius = otherAgent->m_Radius;

			gen::CVector2 vecFromOther = this->GetPosition() - otherAgent->GetPosition();	//Vector from other agent to this agent
			//Determine if the other agent is too far away to matter to this agent's movement
			if (vecFromOther.Length() <
				(myRadius + otherRadius +
					(mySpeed * updateTime) +	//The distance this agent can travel
					(otherSpeed * updateTime)))	//The distance the other agent can travel
			{	//Other agent is close enough to consider
				
				//Create a restriction object to represent arc where collisions are possible
				gen::CVector2 otherVecToLeftEdge; //The vector from the other agent's centre to its nearest point on the 'left edge'
				otherVecToLeftEdge = vecFromOther;
				otherVecToLeftEdge.Normalise();
				otherVecToLeftEdge *= (otherAgent->m_Radius + (otherSpeed * updateTime) + this->m_Radius);	
				otherVecToLeftEdge.SetPerpendicular(); //The vector now points at the end point of the left edge vector
				gen::CVector2 otherLeftPoint = otherAgent->GetPosition() + otherVecToLeftEdge;

				gen::CVector2 myLeftPoint = this->GetPosition();

				buildRestriction.leftEdge = otherLeftPoint - myLeftPoint;
				buildRestriction.leftEdge.Normalise();

				gen::CVector2 otherVecToRightEdge;
				otherVecToRightEdge = -otherVecToLeftEdge;
				gen::CVector2 otherRightPoint = otherAgent->GetPosition() + otherVecToRightEdge;

				gen::CVector2 myRightPoint = this->GetPosition();

				buildRestriction.rightEdge = otherRightPoint - myRightPoint;
				buildRestriction.rightEdge.Normalise();

				buildRestriction.associatedAgent = otherAgent;
				restrictions.push_back(buildRestriction);

			}	//else The other agent is too far to consider
		}	//else Agent ID is this agent, no calculation
	}	//End loop through agents
	
	gen::CVector2 attemptedMovement = m_DesiredMovementVect;
	attemptedMovement.Normalise();
	for (auto restriction : restrictions)
	{
		float leftDotRight = fabs(restriction.leftEdge.Dot(restriction.rightEdge));
		if (fabs(restriction.leftEdge.Dot(attemptedMovement))  < leftDotRight && 
			fabs(attemptedMovement.Dot(restriction.rightEdge)) < leftDotRight)
		{
			gen::CVector2 distBetween = restriction.associatedAgent->GetPosition() - this->GetPosition();
			float length = distBetween.Length();
			length = length - restriction.associatedAgent->m_Radius - this->m_Radius;
			length *= 0.5f;
			attemptedMovement = m_DesiredMovementVect * length;
			////The desired vector lies between the arc of left and right, determine how far it can go without intersection
			//GAgent* otherAgent = restriction.associatedAgent;
			//
			//gen::CVector2 end = attemptedMovement + this->GetPosition();
			//gen::CVector2 start = this->GetPosition();
			//gen::CVector2 circle = otherAgent->GetPosition();
			//float radius = otherAgent->m_Radius + this->m_Radius;
			//
			//float a = ((end.x - start.x) * (end.x - start.x)) + ((end.y - start.y) * (end.y - start.y));
			//float b = (2 * (end.x - start.x) * (start.x - circle.x)) + (2 * (end.y - start.y) * (start.y - circle.y));
			//float c = ((start.x - circle.x) * (start.x - circle.x)) + ((start.y - circle.y) * (start.y - circle.y)) - (radius * radius);
			//float t = (2 * c) / (-b + sqrt((b * b) - (4 * a * c)));
			//
			//attemptedMovement *= t;
		}
	}
	m_MovementVector = attemptedMovement;

}