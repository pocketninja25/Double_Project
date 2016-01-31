#include "GAgent.hpp"
#include "GSceneManager.hpp"

GAgent::GAgent(CVector2 iPosition, CVector2 iDestination, bool iIsActive) :
	GEntity(iPosition, iIsActive),
#ifdef _DEBUG
	dm_BeingWatched(false),
#endif
	m_Destination(iDestination),
	m_Velocity(15.0f),//TODO: find a way of calculating this better - or make it programmable
	m_MaxTurn(gen::ToRadians(45.0f)),
	m_PreviousMovementVect(CVector2(0.0f, 0.0f)),
	m_PreviousDesiredMovementVect(CVector2(0.0f, 0.0f)),
	m_Radius(sqrt(200.0f)/2),	//TODO: provide a reliable version of this number based on file data
	m_MaxGradientTraversal(sqrt(pow(m_Radius+m_Velocity, 2) - (2 * pow(m_Radius, 2)) ) + 1)	//(r + v)^2 - r^2 - r^2 = h^2 (h == the height(influence) at radius length away from the centre) TODO: Pick a better number
{
	m_DesiredMovementVect = CVector2(GetPosition(), m_Destination);
	m_DesiredMovementVect.Normalise();
	m_DesiredMovementVect *= m_Velocity;

}

GAgent::GAgent(float iXPos, float iYPos, float iXDest, float iYDest, bool iIsActive) : 
	GAgent(CVector2(iXPos, iYPos), CVector2(iXDest, iYDest), iIsActive)
{
	//Delagated Constructor - No Body
}

GAgent::~GAgent()
{
}

bool GAgent::HasReachedDestination()
{
	float distTo = gen::Abs(this->GetPosition().DistanceToSquared(m_Destination));

	if (distTo < m_Radius)	
	{
		return true;
	}
	return false;
}

CVector2 GAgent::GetDesiredMovement()
{
	return m_DesiredMovementVect;
}

CVector2 GAgent::GetDestination()
{
	return m_Destination;
}

void GAgent::SetNewDestination(CVector2 newDestination)
{
	//Recieve a new destination (from the manager class) when the manager determines the agent has arrived, or should change destination otherwise
	m_Destination = newDestination;
	
	//Calculate the new movement vector
	m_DesiredMovementVect = m_Destination - GetPosition();
	m_DesiredMovementVect.Normalise();
	m_DesiredMovementVect * m_Velocity;
}
void GAgent::SetPosition(CVector2 newPosition)
{
	CMatrix3x3 tempMatrix = GetMatrix();

	tempMatrix.e20 = newPosition.x;
	tempMatrix.e21 = newPosition.y;

	SetMatrix(tempMatrix);
}

void GAgent::Update(float updateTime)
{
	m_PreviousDesiredMovementVect = m_DesiredMovementVect;
	m_PreviousMovementVect = m_MovementVector;
	
	//Apply physics to model
	CMatrix3x3 matrix = GetMatrix();	//Get matrix of this agent
	matrix.FaceTarget2D(m_Destination);		//Face the agent towards it's destination

	m_DesiredMovementVect = m_Destination - GetPosition();
	m_DesiredMovementVect.Normalise();
	m_DesiredMovementVect *= m_Velocity;

	PerformGlobalCollisionAvoidance();
	PerformLocalCollisionAvoidance();

	//Move by the movement vector (units per second) modified by update time calculating units this frame
	matrix.Move2D(m_MovementVector);// *updateTime);
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

	CVector2 A;	//The back left vertex of the potential collision trapezium (line AE has points H and C on it)
	CVector2 B;	//The back right vertex of the potential collision trapezium (line BE has points G and D on it)
	CVector2 C;	//The front left vertex of the potential collision trapezium
	CVector2 D;	//The front right vertex of the potential collision trapezium
	CVector2 E;	//The position of this agent
	CVector2 F;	//The position of the other agent
	CVector2 G;	//The point of tangential intersection of the combined radii circle and the line BE
	CVector2 H;	//The point of tangential intersection of the combined radii circle and the line AE
	CVector2 I;	//The point along the line FE that intersects the agents' combined radii circle
	CVector2 J;	//The point at which the extension of the line FE intersects the combined radii of the agents

	CVector2 ABNormal;
	CVector2 CDNormal;
	CVector2 ACNormal;
	CVector2 BDNormal;

	float x;
	float y;

	GAgent* associatedAgent;
};
 
void GAgent::CalculateInfluence(GInfluenceMap* influenceMap)
{
	
	//Get influence square coords for position
	GIntPair bottomLeft;
	GIntPair topRight;

	//For now use influence range as radius + velocity
	CVector2 myPos = GetPosition();
	float influenceRange = (m_Radius + m_Velocity);

	bottomLeft = influenceMap->GetGridSquareFromPosition(CVector2(myPos.x - influenceRange, myPos.y - influenceRange));
	topRight = influenceMap->GetGridSquareFromPosition(CVector2(myPos.x + influenceRange, myPos.y + influenceRange));

	CVector2 squareCentre;
	for (int x = bottomLeft.x; x <= topRight.x; x++)
	{
		for (int y = bottomLeft.y; y <= topRight.y; y++)
		{
			squareCentre = influenceMap->GetSquareCentre(x, y);
			
			float influence = pow(influenceRange, 2) - pow(squareCentre.x - myPos.x, 2) - pow(squareCentre.y - myPos.y, 2);
			if (influence >= 0)	//If influence is not negative then square root and save the influence, otherwise ignore this square
			{
				influence = sqrt(influence);
				influenceMap->AddValue(x, y, influence);
			}
		}
	}

}
#include <iostream>
void GAgent::PerformGlobalCollisionAvoidance()
{
	GInfluenceMap* influenceMap = GSceneManager::GetInstance()->GetInfluenceMap();

	//Determine this agent's current position in world space and on the influence map
	CVector2 myPos = GetPosition();
	GIntPair coord = influenceMap->GetGridSquareFromPosition(myPos);

	CVector2 gridData[9]; //Use x and y as position of square, z as the gradient
	float gridHeight[9];



	//Calculate the gradient of the surrounding 4/8 squares
	int count = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{

			gridData[count] = influenceMap->GetSquareCentre(coord.x + i, coord.y + j);
			gridHeight[count] = influenceMap->GetSquareGradient(coord.x + i, coord.y + j);

			count++;
		}
	}

	int bestIndex = -1;
	CVector2 toDestination = m_Destination - myPos;
	toDestination.Normalise();
	for (int i = 0; i < 9; i++)
	{
		//Apply Limitation logic to decide whether this square is a viable target
		CVector2 toSquare = gridData[i] - myPos;
		toSquare.Normalise();
		if (toDestination.Dot(toSquare) > 0.5f)	//The target square is within 60 degrees either way of the ideal destination
		{
			if (gridHeight[4] - gridHeight[i] < m_MaxGradientTraversal)	//TODO: IMPROVE/REMOVE THESE VALUES
			{

				//Decide if this square is better than the current best descision
				if (bestIndex == -1)	//Set the first viable movement (cannot compare to another grid Height)
				{
					bestIndex = i;
				}
				else if (gridHeight[i] < gridHeight[bestIndex])	//Less of a 'hill' than current best
				{
					bestIndex = i;

				}
			}
		}
	}

	if (bestIndex == -1)	//All potential moves are bad, stay where you are
	{
		bestIndex = 0;
	}

	CVector2 targetPosition;
	switch (bestIndex)
	{
	case 0:	//bottomleft
		targetPosition = influenceMap->GetSquareCentre(coord.x - 1, coord.y - 1  );
		break;																   
	case 1:	//left															   
		targetPosition = influenceMap->GetSquareCentre(coord.x - 1, coord.y  );
		break;																   
	case 2:	//topleft														   
		targetPosition = influenceMap->GetSquareCentre(coord.x - 1, coord.y + 1  );
		break;																   
	case 3:	//bottom														   
		targetPosition = influenceMap->GetSquareCentre(coord.x, coord.y - 1 );
		break;																   
	case 4:	//centre														   
		targetPosition = influenceMap->GetSquareCentre(coord.x, coord.y  );
		break;																   
	case 5:	//top															   
		targetPosition = influenceMap->GetSquareCentre(coord.x, coord.y + 1  );
		break;																   
	case 6:	//bottomright													 
		targetPosition = influenceMap->GetSquareCentre(coord.x + 1, coord.y - 1  );
		break;																   
	case 7:	//right															   
		targetPosition = influenceMap->GetSquareCentre(coord.x + 1, coord.y  );
		break;																   
	case 8:	//topright														   
		targetPosition = influenceMap->GetSquareCentre(coord.x + 1, coord.y + 1  );
		break;
	default:
		break;
	}

	//TODO: apply limitations

	m_DesiredMovementVect = targetPosition - myPos;
	m_DesiredMovementVect.Normalise();
	m_DesiredMovementVect *= m_Velocity;
	//After determining the best option, take action upon it //TODO: Give agents a maximum turning angle per second/frame
	
	m_MovementVector = m_DesiredMovementVect * GSceneManager::GetInstance()->GetTimeStep();	//This will allow the two collision avoidance algorithms to work together or seperately, if the local avoidance is used this value will just be overridden, otherwise it is used for movement;
	
}

//-------------------------------------------------------------------------------------------
// This only covers local avoidance (essentially there as collision detection and resolution)
//-------------------------------------------------------------------------------------------
void GAgent::GiveLocalAgentsList(std::vector<GAgent*> localAgents)
{
	m_PotentiallyCollidingAgents = localAgents;
}

void GAgent::PerformLocalCollisionAvoidance()
{
	float updateTime = GSceneManager::GetInstance()->GetTimeStep();
	std::vector<SRestrictionObject> restrictions;
	std::vector<SRestrictionObject> insideRestrictions;
	SRestrictionObject buildRestriction;
	for (auto otherAgent : m_PotentiallyCollidingAgents)
	{
		//Test to see if this agent is the agent referenced in agent, if so then skip calcualtions for that agent
		if (otherAgent->GetUID() != this->GetUID())
		{
			//Determine the distance that the other agent can travel in one frame
			float mySpeed = this->m_DesiredMovementVect.Length() * updateTime;
			float otherSpeed = otherAgent->m_DesiredMovementVect.Length() * updateTime;
			float myRadius = this->m_Radius;
			float otherRadius = otherAgent->m_Radius;

			CVector2 vecToOther = this->GetPosition() - otherAgent->GetPosition();	//Vector from other agent to this agent
			//Determine if the other agent is too far away to matter to this agent's movement
			if (vecToOther.Length() <
				(myRadius + otherRadius +
					mySpeed +	//The distance this agent can travel
					otherSpeed))	//The distance the other agent can travel
			{	//Other agent is close enough to consider
				
				//Have all points be relative to E (this)


				//Create a restriction object to represent arc where collisions are possible
				buildRestriction.E = CVector2(0.0f, 0.0f);
				buildRestriction.F = this->GetPosition() - otherAgent->GetPosition();

				//Get direction vector from E to F
				buildRestriction.I = vecToOther;
				buildRestriction.I.Normalise();
				buildRestriction.J = buildRestriction.I;

				//Scale by the correct length
				buildRestriction.I *= (vecToOther.Length() - (this->m_Radius + otherAgent->m_Radius));
				buildRestriction.J *= (vecToOther.Length() + (this->m_Radius + otherAgent->m_Radius));
				
				buildRestriction.x = gen::ToDegrees(asinf((this->m_Radius + otherAgent->m_Radius) / vecToOther.Length()));
				buildRestriction.y = 90 - buildRestriction.x;
			

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
				CVector2 offset = ((this->m_DesiredMovementVect * updateTime) + (otherAgent->m_DesiredMovementVect * updateTime)) / 2;
				buildRestriction.A += offset;
				buildRestriction.B += offset;
				buildRestriction.C += offset;
				buildRestriction.D += offset;

				restrictions.push_back(buildRestriction);

			}	//else The other agent is too far to consider
		}	//else Agent ID is this agent, no calculation
	}	//End loop through agents
	
	
	m_MovementVector = MoveTheDesiredVect(restrictions, m_DesiredMovementVect, updateTime);
}

CVector2 GAgent::MoveTheDesiredVect(std::vector<SRestrictionObject>& restrictions, CVector2 attemptedMovement, float updateTime)
{
	std::vector<SRestrictionObject> insideRestrictions;	//Vector of restrictions which the movement vector falls inside of
	
	//Determine if the desired velocity would intersect this restriction object
	for (auto restriction : restrictions)
	{

		if (//restriction.ABNormal.Dot(attemptedMovement) > 0 &&
			restriction.ACNormal.Dot(attemptedMovement) > 0 &&
			restriction.BDNormal.Dot(attemptedMovement) > 0 &&
			restriction.CDNormal.Dot(attemptedMovement) > 0)
		{
			//Is inside the box
			insideRestrictions.push_back(restriction);
		}
		else if (restriction.x != restriction.x)
		{ 
			insideRestrictions.push_back(restriction);
		}

	}
	

	if (insideRestrictions.size() > 0)
	{
		if (insideRestrictions[0].x == insideRestrictions[0].x)
		{
			//Move the attempted movement then pass it to this function if insideRestrictions is > 0
			//TODO: MOVE THE MOVEMENT VECTOR AND POP THE RELEVANT RESTRICTION //THIS MAY BE COMPLETE ALREADY, look again
			CVector2 newPoints[4];
			CVector2 AB = insideRestrictions[0].B - insideRestrictions[0].A;
			CVector2 AE = attemptedMovement - insideRestrictions[0].A;
			float scale = (AE.Dot(AB)) / AB.LengthSquared();
			newPoints[0] = (AB * scale) + insideRestrictions[0].A;

			CVector2 AC = insideRestrictions[0].C - insideRestrictions[0].A;
			//CVector2 AE = attemptedMovement - insideRestrictions[0].A;
			scale = (AE.Dot(AC)) / AC.LengthSquared();
			newPoints[1] = (AC * scale) + insideRestrictions[0].A;

			CVector2 DB = insideRestrictions[0].B - insideRestrictions[0].D;
			CVector2 DE = attemptedMovement - insideRestrictions[0].D;
			scale = (DE.Dot(DB)) / DB.LengthSquared();
			newPoints[2] = (DB * scale) + insideRestrictions[0].D;

			CVector2 DC = insideRestrictions[0].C - insideRestrictions[0].D;
			//CVector2 DE = attemptedMovement - insideRestrictions[0].D;
			scale = (DE.Dot(DC)) / DC.LengthSquared();
			newPoints[3] = (DC * scale) + insideRestrictions[0].D;

			CVector2 closest = newPoints[0];
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
				return MoveTheDesiredVect(insideRestrictions, closest, updateTime);
			}
			return closest * updateTime;
		}
	}
	return attemptedMovement * updateTime;	//The desired velocity lies outside all restrictions
}
//-------------------------------------------------------------------------------------------
//END OF LOCAL AVOIDANCE ALGORITHM
//-------------------------------------------------------------------------------------------

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