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
	GSceneManager(iWorldInfo.TimeStep, iWorldInfo.WorldSize, iWorldInfo.xSubdivisions, iWorldInfo.ySubdivisions)
{
	//Delegating Constructor
}

GSceneManager::GSceneManager(float iTimeStep, float iWorldXSize, float iWorldYSize, int iXSubdivisions, int iYSubdivisions) :
	GSceneManager(iTimeStep, gen::CVector2(iWorldXSize, iWorldYSize), iXSubdivisions, iYSubdivisions)
{
	//Delegating Constructor
}

GSceneManager::GSceneManager(float iTimeStep, gen::CVector2 iWorldSize, int iXSubdivisions, int iYSubdivisions) :
	mManager_Entity(new GEntityManager()),
	mManager_Obstacle(new GObstacleManager()),
	m_TimeStep(iTimeStep),
	m_TimeSinceLastUpdate(0.0f),
	m_WorldSize(iWorldSize),
	m_NoOfSquaresX(iXSubdivisions),
	m_NoOfSquaresY(iYSubdivisions),
	m_SceneSquares(new GSceneSquare*[iXSubdivisions * iYSubdivisions]),
	m_SquareSize(gen::CVector2(iWorldSize.x / iXSubdivisions, iWorldSize.y / iYSubdivisions)),
	m_Paused(false)
{
	//Construct the scenesquare objects
	for (int i = 0; i < m_NoOfSquaresX; i++)
	{
		for (int j = 0; j < m_NoOfSquaresY; j++)
		{
			m_SceneSquares[i * m_NoOfSquaresX + j] = new GSceneSquare(m_SquareSize, gen::CVector2(i * m_SquareSize.x, j * m_SquareSize.y));
		}
	}
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

	//Deallocate Managers
	delete mManager_Entity;
	delete mManager_Obstacle;

	//Reset the instance pointer, there is no longer an instance
	mManager_Scene = 0;
}

gen::CVector2 GSceneManager::GetWorldSize()
{
	return m_WorldSize;
}

bool GSceneManager::GetAgentMatrix(UID requestedUID, gen::CMatrix3x3 &matrix)
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

bool GSceneManager::GetAgentPosition(UID requestedUID, gen::CVector2 &position)
{
	GAgent* foundAgent = 0;	//The variable to save the agent should they be found
	if (mManager_Entity->GetAgent(requestedUID, foundAgent))	//If the agent can be found, foundAgent is populated (notNUll)
	{
		position = foundAgent->GetPosition();
		return true;
	}
	//No helpful data format for failed GetMatrix, just return uninitialised memory garbage
	return false;
}

float GSceneManager::GetTimeStep()
{
	return m_TimeStep;
}

bool GSceneManager::GetIsPaused()
{
	return m_Paused;
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

#endif

UID GSceneManager::AddAgent(gen::CVector2 iPosition, bool iIsActive)
{
	UID agentUID = mManager_Entity->AddAgent(iPosition, iIsActive);	//Create the agent object
	
	m_SceneSquares[GetWhichSquare(iPosition)]->AddAgent(agentUID); //Add the agent UID to this square's list of agents at the correct position

	return agentUID;
}

UID GSceneManager::AddAgent(float iXPos, float iYPos, bool iIsActive)
{
	return this->AddAgent(gen::CVector2(iXPos, iYPos), iIsActive);
}

std::vector<UID> GSceneManager::AddXAgents(int kNoAgents, bool iAreActive)
{
	std::vector<UID> agentUIDs;
	
	int xSquare;
	int ySquare;
	gen::CVector2 position;

	for (int i = 0; i < kNoAgents; i++)
	{
		position = gen::CVector2(RandomFloat(0.0f, m_WorldSize.x), RandomFloat(0.0f, m_WorldSize.y));
		
		agentUIDs.push_back(mManager_Entity->AddAgent(position, iAreActive));
		m_SceneSquares[GetWhichSquare(position)]->AddAgent(agentUIDs.back());
	}

	return agentUIDs;
}

void GSceneManager::ComputeAgentVelocities(const std::list<UID>& localAgents)
{
	mManager_Entity->ComputeAgentVelocities(localAgents);
}

bool GSceneManager::SetAgentPosition(UID agent, gen::CVector2 newPosition)
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

void GSceneManager::SetPaused(bool iPaused)
{
	m_Paused = iPaused;
}

void GSceneManager::PerformOneTimeStep()
{
	for (int i = 0; i < (m_NoOfSquaresX * m_NoOfSquaresY); i++)
	{
		m_SceneSquares[i]->Update(m_TimeStep);
	}
	//Begin the update tree
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

int GSceneManager::GetWhichSquare(gen::CVector2 itemPosition)
{
	//position/square width rounded down to the nearest integer = the square to add to
	int xSquare = static_cast<int>(itemPosition.x / m_SquareSize.x);
	int ySquare = static_cast<int>(itemPosition.y / m_SquareSize.y);

	//Do the clamp
	if (xSquare >= m_NoOfSquaresX)
	{
		xSquare = m_NoOfSquaresX - 1;
	}
	if (ySquare >= m_NoOfSquaresY)
	{
		ySquare = m_NoOfSquaresY - 1;
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
	gen::CVector2 tempPos;
	for (int i = 0; i < unassignedAgents.size(); i++)
	{
		if (this->GetAgentPosition(unassignedAgents[i], tempPos))	//If agent exists
		{
			m_SceneSquares[GetWhichSquare(tempPos)]->AddAgent(unassignedAgents[i]); //Add the current UID to this square's list of agents
		}
		//Else ignore this agent, it is removed from the scene system
	}
}


#ifdef _DEBUG
bool GSceneManager::SetAgentWatched(UID agentID, bool isWatched)
{
	return mManager_Entity->SetAgentWatched(agentID, isWatched);
}

bool GSceneManager::GetAgentWatched(UID agentID)
{
	return mManager_Entity->GetAgentWatched(agentID);
}
#endif