#include "GSceneSquare.hpp"
#include "GSceneManager.hpp"


GSceneSquare::GSceneSquare(gen::CVector2 iSize, gen::CVector2 iOriginPos, bool iIsActive) :
	GObject(iIsActive),
	m_Size(iSize),
	m_Origin(iOriginPos)
{
}

GSceneSquare::GSceneSquare(gen::CVector2 iSize, float iOriginXPos, float iOriginYPos, bool iIsActive) :
	GSceneSquare(iSize, gen::CVector2(iOriginXPos, iOriginYPos))
{
	//Delagated Constructor - No Body
}

GSceneSquare::GSceneSquare(float iXSize, float iYSize, gen::CVector2 iOriginPos, bool iIsActive) :
	GSceneSquare(gen::CVector2(iXSize, iYSize), iOriginPos, iIsActive)
{
	//Delagated Constructor - No Body
}

GSceneSquare::GSceneSquare(float iXSize, float iYSize, float iOriginXPos, float iOriginYPos, bool iIsActive) :
	GSceneSquare(gen::CVector2(iXSize, iYSize), gen::CVector2(iOriginXPos, iOriginYPos), iIsActive)
{
	//Delagated Constructor - No Body
}

GSceneSquare::~GSceneSquare()
{
}

void GSceneSquare::Update(float updateTime)
{
	GSceneManager::GetInstance()->ComputeAgentVelocities(m_Agents);
	
}

std::vector<UID> GSceneSquare::TransferAgents()
{
	GSceneManager* sceneManager = GSceneManager::GetInstance();
	
	std::vector<UID> rejectionList;	//List of the agents no longer in this scene square

	gen::CVector2 position;
	auto ID = m_Agents.begin();
	bool incremented;
	while (ID != m_Agents.end())
	{
		incremented = false;
		if (sceneManager->GetAgentPosition(*ID, position))	//The agent exists, retrieved the position
		{
			//Test if the position is outside of the bounding box of this scene square
			if (position.x < m_Origin.x || 
				position.y < m_Origin.y || 
				position.x >= (m_Origin.x + m_Size.x) ||
				position.y >= (m_Origin.y + m_Size.y)	)
			{
				//Set the agent to be rejected
				rejectionList.push_back(*ID);
				//Remove the agent from this list
				ID = m_Agents.erase(ID);
				incremented = true;
			}
		}
		else //The agent does not exist
		{
			//Remove the agent
			ID = m_Agents.erase(ID);
			incremented = true;
		}
		//If the agent exists and is still within the bounding box then the iterator has not been incremented
		if (!incremented)
		{
			//Increment the iterator
			ID++;
		}
	}

	return rejectionList;
}

void GSceneSquare::AddAgent(UID agentID)
{
	m_Agents.push_back(agentID);
}

void GSceneSquare::RemoveAgent(UID agentID)
{
	m_Agents.remove(agentID);
}

#ifdef _DEBUG
std::string GSceneSquare::ToString()
{
	std::stringstream builder;

	builder << GObject::ToString() << "Square Width: " << m_Size.x << " Square Height: " << m_Size.y << std::endl
		<< "Square Origin: " << m_Origin << std::endl
		<< "No of Agents: " << m_Agents.size() << std::endl
		<< "No of walls: " << m_Walls.size() << std::endl;
		
	return builder.str();
}
#endif