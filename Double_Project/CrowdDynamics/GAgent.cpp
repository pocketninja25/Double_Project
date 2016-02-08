#include "GAgent.hpp"
#include "GSceneManager.hpp"

GAgent::GAgent(SAgentBlueprint &agentDetails) :
	GEntity(agentDetails.position, agentDetails.startsActive),
	m_Destination(agentDetails.destination),
	m_Velocity(agentDetails.velocity),
#ifdef _DEBUG
	dm_BeingWatched(false),
#endif
	km_MaxTurn(agentDetails.defaultTurningArc),
	m_TempMaxTurn(km_MaxTurn),
	m_PreviousMovementVect(CVector2(0.0f, 0.0f)),
	m_Radius(agentDetails.radius),
	m_MaxGradientTraversal(agentDetails.maxGradientTraversal),
	m_StillTimer(0.0f)
{
	m_DesiredMovementVect = CVector2(GetPosition(), m_Destination);
	m_DesiredMovementVect.Normalise();
	m_DesiredMovementVect *= m_Velocity;

}

GAgent::GAgent(CVector2 iPosition, CVector2 iDestination, bool iIsActive) :
	GEntity(iPosition, iIsActive),
#ifdef _DEBUG
	dm_BeingWatched(false),
#endif
	m_Destination(iDestination),
	m_Velocity(15.0f),
	km_MaxTurn(gen::ToRadians(60.0f)),
	m_TempMaxTurn(km_MaxTurn),
	m_PreviousMovementVect(CVector2(0.0f, 0.0f)),
	m_PreviousDesiredMovementVect(CVector2(0.0f, 0.0f)),
	m_Radius(sqrt(200.0f)/2),	
	m_MaxGradientTraversal(1.0f),	//(r + v)^2 - r^2 - r^2 = h^2 (h == the height(influence) at radius length away from the centre) 
	m_StillTimer(0.0f)
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
	//PerformLocalCollisionAvoidance();

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

	int coeffX = 1;	
	int coeffY = 1;
	int coeffZ = 1;

	//For now use influence range as radius + velocity
	CVector2 myPos = GetPosition();
	float influenceRange = (m_Radius + m_Velocity);

	bottomLeft = influenceMap->GetGridSquareFromPosition(CVector2(myPos.x - influenceRange, myPos.y - (influenceRange)));
	topRight = influenceMap->GetGridSquareFromPosition(CVector2(myPos.x + influenceRange, myPos.y + (influenceRange)));

	CVector2 flowDirection = m_Destination - myPos;
	flowDirection.Normalise();

	CVector2 squareCentre;
	for (int x = bottomLeft.x; x <= topRight.x; x++)
	{
		for (int y = bottomLeft.y; y <= topRight.y; y++)
		{
			squareCentre = influenceMap->GetSquareCentre(x, y);
			
			float influence = coeffZ * (pow(influenceRange, 2) - pow(squareCentre.x - myPos.x, 2) / coeffX - pow(squareCentre.y - myPos.y, 2) / coeffY);

			if (influence >= 0)	//If influence is not negative then square root and save the influence, otherwise ignore this square
			{
				influence = sqrt(influence);
				influenceMap->AddValue(x, y, influence);
				influenceMap->AddFlow(x, y, flowDirection);
			}
		}
	}

}



void GAgent::PerformGlobalCollisionAvoidance()
{
	GInfluenceMap* influenceMap = GSceneManager::GetInstance()->GetInfluenceMap();

	//Determine this agent's current position in world space and on the influence map
	CVector2 myPos = GetPosition();
	GIntPair coord = influenceMap->GetGridSquareFromPosition(myPos);

	CVector2 gridCentre[Dir_Size]; //Stores Position of square
	float gridHeight[Dir_Size];
	CVector2 gridFlow[Dir_Size];
	bool gridIsBlocked[Dir_Size];

	//Calculate the gradient of the surrounding squares
	int count = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{

			gridCentre[count] = influenceMap->GetSquareCentre(coord.x + i, coord.y + j);


			//TODO: PERFORM BETTER CALCULATION FOR HEIGHT (SOME FORM OF GRADIENT - remove the unused METHODs)
			//METHOD 1: GET THE DIRECT INFLUENCE VALUE
			gridHeight[count] = influenceMap->GetValue(coord.x + i, coord.y + j);

			//METHOD 2: CALCULATE AN average of the next x squares
			int maxSquaresPerSecond = static_cast<int>(m_Velocity + 1);	//Consider the 'next' x squares and calculate an average
			float accumulation = 0;
			for (int k = 0; k < maxSquaresPerSecond; k++)
			{
				accumulation += influenceMap->GetValue(coord.x + (i * k), coord.y + (j * k));
			}
			accumulation /= maxSquaresPerSecond;
			gridHeight[count] = accumulation;


			//Get Flow Data
			gridFlow[count] = influenceMap->GetFlow(coord.x + i, coord.y + j);

			//Get blocking data
			gridIsBlocked[count] = influenceMap->GetIsBlocked(coord.x + i, coord.y + j);

			count++;
		}
	}

	CVector2 averageFlow(0.0f, 0.0f);
	for (int i = 0; i < Dir_Size; i++)
	{
		averageFlow += gridFlow[i];
	}
	averageFlow.Normalise();

	int bestIndex = Dir_Size; //Initialise to an incorrect value - test later to see if incorrect is chosen, then decide what to do accordingly
	CVector2 toDestination = m_Destination - myPos;
	toDestination.Normalise();

	//TODO: Average the toDestination vector with the relevant flowVector in a 75% - 25% split to create a guided direction
	CVector2 thisSquareFlow = (0.75f * toDestination) + (0.25f *  averageFlow);
	thisSquareFlow.Normalise();


	for (int i = 0; i < Dir_Size; i++)
	{
		if (!gridIsBlocked[i])	//No point testing other conditions if the grid square is blocked
		{
			//Apply Limitation logic to decide whether this square is a viable target
			CVector2 toSquare = gridCentre[i] - myPos;
			toSquare.Normalise();

			if (thisSquareFlow.Dot(toSquare) > cos(m_TempMaxTurn))	//The target square is within 60 degrees either way of the ideal destination
			{
				if (gridHeight[i] < gridHeight[Dir_Centre])	//Only pick if the location has less resistance than current position
				{
					//Decide if this square is better than the current best descision
					if (bestIndex == Dir_Size)	//Set the first valid movement (cannot compare to another grid Height)
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
	}

	if (bestIndex == Dir_Size)	//All potential moves are bad, stay where you are
	{
		bestIndex = Dir_Centre;
	}
	CVector2 targetPosition;
	if (bestIndex == Dir_Centre)
	{
		targetPosition = myPos;	//Target where you stand, i.e. dont move (rather than moving to the centre of the current square, which could mean going backwards)
		
		m_StillTimer += GSceneManager::GetInstance()->GetTimeStep(); //Increase the timer due to being stuck
		
	}
	else
	{
		targetPosition = gridCentre[bestIndex];
		if (m_StillTimer > 0.0f)
		{	
			m_StillTimer -= 2 * GSceneManager::GetInstance()->GetTimeStep();	//Decrease the timer due to being unstuck (dont reset, just reduce at double speed)
		}
	}
	//Add 10 degrees to maneuverability for every second stuck
	m_TempMaxTurn = km_MaxTurn + gen::ToRadians(20.0f * m_StillTimer);

	//TODO: apply additional limitations

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


void GAgent::SetWatched(bool isWatched)
{
	dm_BeingWatched = isWatched;
}

bool GAgent::GetWatched()
{
	return dm_BeingWatched;
}
