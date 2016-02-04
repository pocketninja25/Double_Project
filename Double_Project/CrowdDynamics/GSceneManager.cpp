#include "GSceneManager.hpp"

#include "GEntityManager.hpp"
#include "GObstacleManager.hpp"
#include "GAgent.hpp"

GSceneManager* GSceneManager::mManager_Scene = 0;

GSceneManager* GSceneManager::GetInstance(SWorldInfo* iWorldData)
{
	if (!mManager_Scene && iWorldData)	//If there is no manager instance but there is a set of world data create an instance
	{
		//Construct the manager

		mManager_Scene = new GSceneManager(*iWorldData);
	}

	return mManager_Scene;
}

GSceneManager::GSceneManager(SWorldInfo iWorldInfo) :
	GSceneManager(iWorldInfo.TimeStep, iWorldInfo.WorldSize, iWorldInfo.xSubdivisions, iWorldInfo.ySubdivisions, iWorldInfo.influenceSquaresPerUnit)
{
	//Delegating Constructor
}

GSceneManager::GSceneManager(float iTimeStep, float iWorldXSize, float iWorldYSize, int iXSubdivisions, int iYSubdivisions, float influenceSquaresPerUnit) :
	GSceneManager(iTimeStep, CVector2(iWorldXSize, iWorldYSize), iXSubdivisions, iYSubdivisions, influenceSquaresPerUnit)
{
	//Delegating Constructor
}

GSceneManager::GSceneManager(float iTimeStep, CVector2 iWorldSize, int iXSubdivisions, int iYSubdivisions, float influenceSquaresPerUnit) :
	mManager_Entity(new GEntityManager()),
	mManager_Obstacle(new GObstacleManager()),
	m_TimeStep(iTimeStep),
	m_TimeSinceLastUpdate(0.0f),
	m_WorldSize(iWorldSize),
	m_NoOfSquaresX(iXSubdivisions),
	m_NoOfSquaresY(iYSubdivisions),
	m_SceneSquares(new GSceneSquare*[iXSubdivisions * iYSubdivisions]),
	m_SquareSize(CVector2(iWorldSize.x / iXSubdivisions, iWorldSize.y / iYSubdivisions)),
	m_Paused(false)
{
	//Construct the scenesquare objects
	for (int i = 0; i < m_NoOfSquaresX; i++)
	{
		for (int j = 0; j < m_NoOfSquaresY; j++)
		{
			m_SceneSquares[i * m_NoOfSquaresX + j] = new GSceneSquare(m_SquareSize, CVector2(i * m_SquareSize.x, j * m_SquareSize.y));
		}
	}

	//Have the influence map represent 1 square = 1 unit, any incomplete integer in world size is clamped into the nearest square (e.g. world size x = 3.2 so have 3 squares for x, item at position 3.1 clamps to square 3)
	int tempX = static_cast<int>(m_WorldSize.x);
	int tempY = static_cast<int>(m_WorldSize.y);

	m_InfluenceMap = new GInfluenceMap(tempX * influenceSquaresPerUnit, tempY * influenceSquaresPerUnit, CVector2(1/influenceSquaresPerUnit, 1/influenceSquaresPerUnit));
}

GSceneManager::~GSceneManager()
{
	int noSquares = m_NoOfSquaresX * m_NoOfSquaresY;

	//Deallocate each scene square
	for (int i = 0; i < noSquares; i++)
	{
		delete m_SceneSquares[i];
	}
	//Deallocate the scenesquare array
	delete[] m_SceneSquares;

	//Deallocate the influence map
	delete m_InfluenceMap;

	//Deallocate Managers
	delete mManager_Entity;
	delete mManager_Obstacle;

	//Reset the instance pointer, there is no longer an instance
	mManager_Scene = 0;
}

CVector2 GSceneManager::GetWorldSize()
{
	return m_WorldSize;
}

bool GSceneManager::GetAgentMatrix(UID requestedUID, CMatrix3x3 &matrix)
{
	GAgent* foundAgent = 0;	//The variable to save the agent should they be found
	if (mManager_Entity->GetAgent(requestedUID, foundAgent))	//If the agent can be found, foundAgent is populated (notNUll)
	{
		matrix = foundAgent->GetMatrix();
		return true;
	}
	//No helpful data format for failed GetMatrix, just return uninitialised memory garbage
	return false;
}

bool GSceneManager::GetAgentPosition(UID requestedUID, CVector2 &position)
{
	GAgent* foundAgent = 0;	//The variable to save the agent should they be found
	if (mManager_Entity->GetAgent(requestedUID, foundAgent))	//If the agent can be found, foundAgent is populated (notNull)
	{
		position = foundAgent->GetPosition();
		return true;
	}
	//No helpful data format for failed GetMatrix, just return uninitialised memory garbage
	return false;
}

bool GSceneManager::GetAgentDesiredVector(UID requestedUID, CVector2 &desiredVector)
{
	GAgent* foundAgent = 0;	//The variable to save the agent should they be found
	if (mManager_Entity->GetAgent(requestedUID, foundAgent))	//If the agent can be found, foundAgent is populated (notNull)
	{
		desiredVector = foundAgent->GetDesiredMovement();
		return true;
	}
	//No helpful data format for failed GetMatrix, just return existing desiredVector
	return false;
}

bool GSceneManager::GetAgentDestination(UID requestedUID, CVector2 & destination)
{
	GAgent* foundAgent = 0;	//The variable to save the agent should they be found
	if (mManager_Entity->GetAgent(requestedUID, foundAgent))	//If the agent can be found, foundAgent is populated (notNull)
	{
		destination = foundAgent->GetDestination();
		return true;
	}
	//No helpful data format for failed GetMatrix, just return existing desiredVector
	return false;
}

GInfluenceMap * GSceneManager::GetInfluenceMap()
{
	return m_InfluenceMap;
}


float GSceneManager::GetTimeStep()
{
	return m_TimeStep;
}

bool GSceneManager::GetIsPaused()
{
	return m_Paused;
}

UID GSceneManager::AddAgent(CVector2 iPosition, bool iIsActive)
{
	UID agentUID = mManager_Entity->AddAgent(iPosition, iIsActive);	//Create the agent object
	
	m_SceneSquares[GetWhichSquare(iPosition)]->AddAgent(agentUID); //Add the agent UID to this square's list of agents at the correct position

	return agentUID;
}

UID GSceneManager::AddAgent(float iXPos, float iYPos, bool iIsActive)
{
	return this->AddAgent(CVector2(iXPos, iYPos), iIsActive);
}

UID GSceneManager::AddAgent(SAgentTemplate iTemplate)
{
	UID agentID = mManager_Entity->AddAgent(iTemplate);
	GAgent* theAgent;
	if (mManager_Entity->GetAgent(agentID, theAgent))
	{
		m_SceneSquares[GetWhichSquare(theAgent->GetPosition())]->AddAgent(agentID);
	}
	return agentID;
}

std::vector<UID> GSceneManager::AddXAgents(const int kNoAgents, SAgentTemplate blueprint)
{
	std::vector<UID> agentUIDs;

	GAgent* theAgent;
	for (int i = 0; i < kNoAgents; i++)
	{
		agentUIDs.push_back(mManager_Entity->AddAgent(blueprint));
		if (mManager_Entity->GetAgent(agentUIDs.back(), theAgent))
		{
			m_SceneSquares[GetWhichSquare(theAgent->GetPosition())]->AddAgent(agentUIDs.back());
		}
	}
	return agentUIDs;
}

/*	//Deprecated version
std::vector<UID> GSceneManager::AddXAgents(int kNoAgents, bool iAreActive)
{
	std::vector<UID> agentUIDs;

	CVector2 position;

	for (int i = 0; i < kNoAgents; i++)
	{
		position = CVector2(RandomFloat(0.0f, m_WorldSize.x), RandomFloat(0.0f, m_WorldSize.y));
		
		agentUIDs.push_back(mManager_Entity->AddAgent(position, iAreActive));
		m_SceneSquares[GetWhichSquare(position)]->AddAgent(agentUIDs.back());
	}
	/* //Alternate version to put half on bottom left, half on top right
	std::vector<UID> agentUIDs;

	CVector2 position;

	for (int i = 0; i < 300; i++)
	{
		position = CVector2(RandomFloat(0.0f, m_WorldSize.x / 4), RandomFloat(0.0f, m_WorldSize.y / 4));

		agentUIDs.push_back(mManager_Entity->AddAgent(position, iAreActive));
		m_SceneSquares[GetWhichSquare(position)]->AddAgent(agentUIDs.back());
	}

	for (int i = 300; i < kNoAgents; i++)
	{
		position = CVector2(RandomFloat((3 * m_WorldSize.x) / 4, m_WorldSize.x), RandomFloat((3 * m_WorldSize.y) / 4, m_WorldSize.y));

		agentUIDs.push_back(mManager_Entity->AddAgent(position, iAreActive));
		m_SceneSquares[GetWhichSquare(position)]->AddAgent(agentUIDs.back());
	}
	*/
/*

	return agentUIDs;
}
*/

void GSceneManager::PerformCollisionAvoidance(const std::list<UID>& localAgents)
{

	mManager_Entity->PerformCollisionAvoidance(localAgents);
}


bool GSceneManager::SetAgentPosition(UID agent, CVector2 newPosition)
{
	GAgent* foundAgent = 0;	//The variable to save the agent should they be found
	if (mManager_Entity->GetAgent(agent, foundAgent))	//If the agent can be found, foundAgent is populated (notNUll)
	{
		//Remove from original square		
		m_SceneSquares[GetWhichSquare(foundAgent->GetPosition())]->RemoveAgent(agent);

		//Set new position
		foundAgent->SetPosition(newPosition);

		//Add to new square
		m_SceneSquares[GetWhichSquare(foundAgent->GetPosition())]->AddAgent(agent);
		return true;
	}

	return false;
}

bool GSceneManager::SetAgentActivation(UID agent, bool isEnabled)
{
	return mManager_Entity->SetAgentActivation(agent, isEnabled);
}


void GSceneManager::SetPaused(bool iPaused)
{
	m_Paused = iPaused;
}


void GSceneManager::PerformOneTimeStep()
{
	m_InfluenceMap->Update(m_TimeStep);

	for (int i = 0; i < (m_NoOfSquaresX * m_NoOfSquaresY); i++)
	{
		m_SceneSquares[i]->Update(m_TimeStep);
	}
	
	mManager_Entity->Update(m_TimeStep);
	
	this->MaintainSceneSquares();

}

void GSceneManager::Update(float frameTime)
{
	if (!m_Paused)
	{
		m_TimeSinceLastUpdate += frameTime;

		while (m_TimeSinceLastUpdate > m_TimeStep)		//While there are time steps left - this method allows for multiple time steps to be executed if they have built up //TODO: make this functionality independent of a function call
		{
			m_InfluenceMap->Update(m_TimeStep);

			for (int i = 0; i < (m_NoOfSquaresX * m_NoOfSquaresY); i++)
			{
				m_SceneSquares[i]->Update(m_TimeStep);
			}
			
			//Begin the update tree
			mManager_Entity->Update(m_TimeStep);
			
			this->MaintainSceneSquares();

			//Time step complete, reduce the time since update by the time step
			m_TimeSinceLastUpdate -= m_TimeStep;
		}
	}
}

int GSceneManager::GetWhichSquare(CVector2 itemPosition)
{
	//position/square width rounded down to the nearest integer = the square to add to
	int xSquare = static_cast<int>(itemPosition.x / m_SquareSize.x);
	int ySquare = static_cast<int>(itemPosition.y / m_SquareSize.y);

	//Do the clamp
	if (xSquare >= m_NoOfSquaresX)
	{
		xSquare = m_NoOfSquaresX - 1;
	}
	else if (xSquare < 0)
	{
		xSquare = 0;
	}
	if (ySquare >= m_NoOfSquaresY)
	{
		ySquare = m_NoOfSquaresY - 1;
	}
	else if (ySquare < 0)
	{
		ySquare = 0;
	}

	return xSquare * m_NoOfSquaresX + ySquare;
}

void GSceneManager::MaintainSceneSquares()
{
	std::vector<UID> tempAgents;
	std::vector<UID> unassignedAgents;	
	
	for (int i = 0; i < m_NoOfSquaresX * m_NoOfSquaresY; i++)
	{
		//Get list of agents transferring from this scenesquare
		tempAgents = m_SceneSquares[i]->TransferAgents();
		
		//Append the new agents to the unassignedAgents list
		if (tempAgents.size() > 0)
		{
			unassignedAgents.insert(unassignedAgents.end(), tempAgents.begin(), tempAgents.end());
		}
	}

	//Assign the unassignedAgents to their new square
	CVector2 tempPos;
	for (unsigned int i = 0; i < unassignedAgents.size(); i++)
	{
		if (this->GetAgentPosition(unassignedAgents[i], tempPos))	//If agent exists
		{
			m_SceneSquares[GetWhichSquare(tempPos)]->AddAgent(unassignedAgents[i]); //Add the current UID to this square's list of agents
		}
		//Else ignore this agent, it is removed from the scene system
	}
}

#ifdef _DEBUG
bool GSceneManager::GetAgentString(UID requestedUID, std::string &agentString)
{
	GAgent* foundAgent = 0;	//The variable to save the agent should they be found
	if (mManager_Entity->GetAgent(requestedUID, foundAgent))	//If the agent can be found, foundAgent is populated (notNUll)
	{
		agentString = foundAgent->ToString();
		return true;
	}
	//No helpful data format for failed attempt, just return uninitialised memory garbage
	return false;
}

bool GSceneManager::GetSquareString(int xPos, int yPos, std::string& squareString)
{
	if (xPos >= 0 && xPos < m_NoOfSquaresX && yPos >= 0 && yPos < m_NoOfSquaresY)	//If the square is a valid square
	{
		squareString = m_SceneSquares[xPos * m_NoOfSquaresX + yPos]->ToString();
		return true;
	}
	//No helpful data format for failed attempt, just return uninitialised memory garbage
	return false;
}

bool GSceneManager::GetSquareString(CVector2 coordinate, std::string& squareString)
{
	squareString = m_SceneSquares[this->GetWhichSquare(coordinate)]->ToString();
	return true;
}

#endif
bool GSceneManager::SetAgentWatched(UID agentID, bool isWatched)
{
	return mManager_Entity->SetAgentWatched(agentID, isWatched);
}

bool GSceneManager::GetAgentWatched(UID agentID)
{
	return mManager_Entity->GetAgentWatched(agentID);
}