#include "GAgent.hpp"
#include "GSceneManager.hpp"

GAgent::GAgent(gen::CVector2 iPosition, gen::CVector2 iDestination, bool iIsActive) :
	GEntity(iPosition, iIsActive),
#ifdef _DEBUG
	dm_BeingWatched(false),
#endif
	m_Destination(iDestination),
	m_Velocity(15.0f),//TODO: find a way of calculating this better
	m_PreviousMovementVect(gen::CVector2(0.0f, 0.0f)),
	m_PreviousDesiredMovementVect(gen::CVector2(0.0f, 0.0f)),
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

gen::CVector2 GAgent::GetDesiredMovement()
{
	return m_DesiredMovementVect;
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
void GAgent::SetPosition(gen::CVector2 newPosition)
{
	gen::CMatrix3x3 tempMatrix = GetMatrix();

	tempMatrix.e20 = newPosition.x;
	tempMatrix.e21 = newPosition.y;

	SetMatrix(tempMatrix);
}

void GAgent::Update(float updateTime)
{
	m_PreviousDesiredMovementVect = m_DesiredMovementVect;
	m_PreviousMovementVect = m_MovementVector;

	//Apply physics to model
	gen::CMatrix3x3 matrix = GetMatrix();	//Get matrix of this agent
	matrix.FaceTarget2D(m_Destination);		//Face the agent towards it's destination

	//Move by the movement vector (units per second) modified by update time calculating units this frame
	matrix.Move2D(m_MovementVector);// *updateTime);

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
		<< "DesiredMovementVect: X: " << m_DesiredMovementVect.x << " Y: " << m_DesiredMovementVect.y << "\n";

	return builder.str();
}
#endif

struct SRestrictionObject
{
	gen::CVector2 A;
	gen::CVector2 B;
	gen::CVector2 C;
	gen::CVector2 D;
	gen::CVector2 E;
	gen::CVector2 F;
	gen::CVector2 G;
	gen::CVector2 H;
	gen::CVector2 I;
	gen::CVector2 J;

	gen::CVector2 ABNormal;
	gen::CVector2 CDNormal;
	gen::CVector2 ACNormal;
	gen::CVector2 BDNormal;

	float x;
	float y;

	GAgent* associatedAgent;
};

void GAgent::ComputePossibleVelocities(const std::vector<GAgent*>& localAgents)
{
	float updateTime = GSceneManager::GetInstance()->GetTimeStep();
	std::vector<SRestrictionObject> restrictions;
	std::vector<SRestrictionObject> insideRestrictions;
	SRestrictionObject buildRestriction;
	for (auto otherAgent : localAgents)
	{
		//Test to see if this agent is the agent referenced in agent, if so then skip calcualtions for that agent
		if (otherAgent->GetUID() != this->GetUID())
		{
			//Determine the distance that the other agent can travel in one frame
			float mySpeed = this->m_DesiredMovementVect.Length() * updateTime;
			float otherSpeed = otherAgent->m_DesiredMovementVect.Length() * updateTime;
			float myRadius = this->m_Radius;
			float otherRadius = otherAgent->m_Radius;

			gen::CVector2 vecToOther = otherAgent->GetPosition() - this->GetPosition();	//Vector from other agent to this agent
			//Determine if the other agent is too far away to matter to this agent's movement
			if (vecToOther.Length() <
				(myRadius + otherRadius +
					mySpeed +	//The distance this agent can travel
					otherSpeed))	//The distance the other agent can travel
			{	//Other agent is close enough to consider
				
				//Have all points be relative to E (this)


				//Create a restriction object to represent arc where collisions are possible
				buildRestriction.E = gen::CVector2(0.0f, 0.0f);
				buildRestriction.F = otherAgent->GetPosition() - this->GetPosition();

				//Get direction vector from E to F
				buildRestriction.I = vecToOther;
				buildRestriction.I.Normalise();
				buildRestriction.J = buildRestriction.I;

				//Scale by the correct length
				buildRestriction.I *= (vecToOther.Length() - (this->m_Radius + otherAgent->m_Radius));
				buildRestriction.J *= (vecToOther.Length() + (this->m_Radius + otherAgent->m_Radius));
				
				buildRestriction.x = gen::ToDegrees(asinf((this->m_Radius + otherAgent->m_Radius) / vecToOther.Length()));
				buildRestriction.y = 90 - buildRestriction.x;
				if (this->dm_BeingWatched && buildRestriction.x > 85.0f)
				{
					GSceneManager::GetInstance()->SetPaused(true);
										
				}
				if (this->dm_BeingWatched && buildRestriction.x != buildRestriction.x)
				{
					
				}

				buildRestriction.C = buildRestriction.I;	//Direction //Should be I - E but i is represented as Zero
				buildRestriction.C = buildRestriction.C * gen::Matrix2x2Rotation(gen::ToRadians(-buildRestriction.x));	//Rotate C x degrees counter-clockwise
				buildRestriction.C.Normalise();		//Remove scaling, only a direction
				buildRestriction.C *= gen::Cos(gen::ToDegrees(buildRestriction.x)) / (buildRestriction.I).Length();	//Scale to the correct length (represents a point now) scale = Tan(x) /length(vectorI)

				buildRestriction.D = buildRestriction.I + (buildRestriction.I - buildRestriction.C);	//Position I + (vector from C to I)


				//Scale C to A and D to B using similar triangles and the scale of I to J
				float scale = buildRestriction.J.Length() / buildRestriction.I.Length();

				buildRestriction.A = buildRestriction.C * scale;	//Get direction from vector from E to C and Rescale to correct length using scale from similar triangles
				
				//Do same as to get A but on the other side
				buildRestriction.B = buildRestriction.D * scale;

				//Determine normals
				buildRestriction.ABNormal = buildRestriction.I;
				buildRestriction.ABNormal.Normalise();
				buildRestriction.CDNormal = -buildRestriction.ABNormal;

				//Find H to find the AC Normal
				buildRestriction.H = buildRestriction.C;	//Get Direction
				buildRestriction.H.Normalise();
				buildRestriction.H *= cos(gen::ToRadians(buildRestriction.x)) * buildRestriction.F.Length();	//Use Trig to find length
				
				//Find G to find the BD Normal	- Similar to finding H
				buildRestriction.G = buildRestriction.D;
				buildRestriction.G.Normalise();
				buildRestriction.G *= buildRestriction.H.Length(); //G length is same as H length

				buildRestriction.ACNormal = buildRestriction.H - buildRestriction.F;	//Get direction
				buildRestriction.ACNormal.Normalise();									//Is a normal so make unit length

				buildRestriction.BDNormal = buildRestriction.G - buildRestriction.F;	//Get direction
				buildRestriction.BDNormal.Normalise();									//Is a normal so make unit length

				buildRestriction.associatedAgent = otherAgent;
				
				//Offset the velocity obstacle by (vA + vB)/2 to accomodate for the agents' movement as well as position/size
				//Only need to affect the points A, B, C, D
				gen::CVector2 offset = ((this->m_DesiredMovementVect * updateTime) + (otherAgent->m_DesiredMovementVect * updateTime)) / 2;
				buildRestriction.A += offset;
				buildRestriction.B += offset;
				buildRestriction.C += offset;
				buildRestriction.D += offset;

				restrictions.push_back(buildRestriction);

			}	//else The other agent is too far to consider
		}	//else Agent ID is this agent, no calculation
	}	//End loop through agents
	
	
	m_MovementVector = MoveTheDesiredVect(restrictions, m_DesiredMovementVect * updateTime);
}

gen::CVector2 GAgent::MoveTheDesiredVect(std::vector<SRestrictionObject>& restrictions, gen::CVector2 attemptedMovement)
{
	std::vector<SRestrictionObject> insideRestrictions;	//Vector of restrictions which the movement vector falls inside of
	for (auto restriction : restrictions)
	{
		//Determine if the desired velocity would intersect this restriction object

		if (//restriction.ABNormal.Dot(attemptedMovement) > 0 &&
			restriction.ACNormal.Dot(attemptedMovement) > 0 &&
			restriction.BDNormal.Dot(attemptedMovement) > 0 &&
			restriction.CDNormal.Dot(attemptedMovement) > 0)
		{
			//Is inside the box
			insideRestrictions.push_back(restriction);
		}
	}
	if (insideRestrictions.size() > 0)
	{
		//Move the attempted movement then pass it to this function if insideRestrictions is > 0
		//TODO: MOVE THE MOVEMENT VECTOR AND POP THE RELEVANT RESTRICTION
		gen::CVector2 newPoints[4];
		gen::CVector2 AB = insideRestrictions[0].B - insideRestrictions[0].A;
		gen::CVector2 AE = attemptedMovement - insideRestrictions[0].A;
		float scale = (AE.Dot(AB)) / AB.LengthSquared();
		newPoints[0] = (AB * scale) + insideRestrictions[0].A;

		gen::CVector2 AC = insideRestrictions[0].C - insideRestrictions[0].A;
		//gen::CVector2 AE = attemptedMovement - insideRestrictions[0].A;
		scale = (AE.Dot(AC)) / AC.LengthSquared();
		newPoints[1] = (AC * scale) + insideRestrictions[0].A;

		gen::CVector2 DB = insideRestrictions[0].B - insideRestrictions[0].D;
		gen::CVector2 DE = attemptedMovement - insideRestrictions[0].D;
		scale = (DE.Dot(DB)) / DB.LengthSquared();
		newPoints[2] = (DB * scale) + insideRestrictions[0].D;

		gen::CVector2 DC = insideRestrictions[0].C - insideRestrictions[0].D;
		//gen::CVector2 DE = attemptedMovement - insideRestrictions[0].D;
		scale = (DE.Dot(DC)) / DC.LengthSquared();
		newPoints[3] = (DC * scale) + insideRestrictions[0].D;

		gen::CVector2 closest = newPoints[0];
		for (int i = 1; i < 4; i++)
		{
			if (newPoints[i].Length() < closest.Length())
			{
				closest = newPoints[i];
			}
		}

		insideRestrictions.erase(insideRestrictions.begin());	//Remove the 0th item

		if (insideRestrictions.size() > 0)
		{
			return MoveTheDesiredVect(insideRestrictions, closest);
		}
		return closest;
	}
	return attemptedMovement;	//The desired velocity lies outside all restrictions
}

#ifdef _DEBUG

void GAgent::SetWatched(bool isWatched)
{
	dm_BeingWatched = isWatched;
}

bool GAgent::GetWatched()
{
	return dm_BeingWatched;
}

#endif