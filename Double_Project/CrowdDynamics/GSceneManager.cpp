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
	m_SceneSquares(new GSceneSquare*[iXSubdivisions * iYSubdivisions]),
	m_Paused(false)
{
	float xSquareSize = iWorldSize.x / iXSubdivisions;
	float ySquareSize = iWorldSize.y / iYSubdivisions;

	//Construct the scenesquare objects
	for (int i = 0; i < iXSubdivisions; i++)
	{
		for (int j = 0; j < iYSubdivisions; j++)
		{
			m_SceneSquares[i * j] = new GSceneSquare(xSquareSize, ySquareSize, i * xSquareSize, j * ySquareSize);
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
	delete m_SceneSquares;

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
	//No helpful data format for failed GetMatrix, just return uninitialised memory garbage
	return false;
}

#endif

UID GSceneManager::AddAgent(gen::CVector2 iPosition, bool iIsActive)
{
	return mManager_Entity->AddAgent(iPosition, iIsActive);
}

UID GSceneManager::AddAgent(float iXPos, float iYPos, bool iIsActive)
{
	return this->AddAgent(gen::CVector2(iXPos, iYPos), iIsActive);
}

std::vector<UID> GSceneManager::AddXAgents(int kNoAgents, bool iAreActive)
{
	std::vector<UID> agentUIDs;

	for (int i = 0; i < kNoAgents; i++)
	{
		agentUIDs.push_back(
			mManager_Entity->AddAgent(
				gen::CVector2(RandomFloat(0.0f, m_WorldSize.x), RandomFloat(0.0f, m_WorldSize.y)),
				iAreActive));
	}

	return agentUIDs;
}

void GSceneManager::SetPaused(bool iPaused)
{
	m_Paused = iPaused;
}

void GSceneManager::Update(float frameTime)
{
	if (!m_Paused)
	{
		m_TimeSinceLastUpdate += frameTime;

		while (m_TimeSinceLastUpdate > m_TimeStep)		//While there are time steps left - this method allows for multiple time steps to be executed if they have built up //TODO: make this functionality independent of a function call
		{
			//Begin the update tree
			mManager_Entity->Update(m_TimeStep);


			//Time step complete, reduce the time since update by the time step
			m_TimeSinceLastUpdate -= m_TimeStep;
		}

	}
}


