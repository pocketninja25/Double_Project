#include "GEntityManager.hpp"

#include "GEntity.hpp"
#include "GAgent.hpp"
#include "GSceneManager.hpp"


GEntityManager::GEntityManager()
{
}

GEntityManager::~GEntityManager()
{

	//Deallocate entity information
	for (auto &agent : m_ActiveAgents)
	{
		delete agent.second;
	}
	for (auto &agent : m_InactiveAgents)
	{
		delete agent.second;
	}
}

bool GEntityManager::GetAgent(UID request, GAgent* &returnedAgent)
{

	//Using try catch rather than map::count as the performance hit of an unlikely catch is much less than 1/2 map::count calls each frame
	//TODO: Review this descision
	try
	{
		returnedAgent = m_ActiveAgents.at(request);

	}
	catch (std::out_of_range err)
	{
		try
		{
			returnedAgent = m_ActiveAgents.at(request);

		}
		catch (std::out_of_range err)
		{
			returnedAgent = 0;	//returned agent will be null to prevent unwanted interference due to fail
			return false;		//Not in either list, failed
		}
	}
	return true;	//Havent fallen into both catch blocks, one of the lists succeeded, returnedagent has been populated
}

UID GEntityManager::AddAgent(gen::CVector2 iPosition, bool iIsActive)
{
	//Construct a new agent
	GAgent* newAgent = new GAgent(iPosition, GetRandomDestination(), iIsActive);
	 
	//Push the agent onto the relevant entitylist
	if (iIsActive)
	{
		m_ActiveAgents.emplace(newAgent->GetUID(), newAgent);
	}
	else //inactive
	{
		m_ActiveAgents.emplace(newAgent->GetUID(), newAgent);
	}

	return newAgent->GetUID();
}

UID GEntityManager::AddAgent(float iXPos, float iYPos, bool iIsActive)
{
	//Delegate implementation to other version of the function - preventing duplication errors
	return this->AddAgent(gen::CVector2(iXPos, iYPos), iIsActive);
}

void GEntityManager::Update(float updateTime)
{
	for (auto &agent: m_ActiveAgents)
	{
		agent.second->Update(updateTime);

		//Check if reached dest then give new random dest if true
		if (agent.second->HasReachedDestination())
		{
			agent.second->SetNewDestination(this->GetRandomDestination());
		}
	}
}

void GEntityManager::ComputeAgentVelocities(const std::list<UID>& localAgents)
{
	std::vector<GAgent*> allAgents;
	GAgent* theAgent;
	for (auto agentID : localAgents)
	{
		if (GetAgent(agentID, theAgent))	//Agent with this UID exists 
		{
			allAgents.push_back(theAgent);
		}
	}

	for (auto agent : allAgents)
	{
		agent->ComputePossibleVelocities(allAgents);
	}
	
}

gen::CVector2 GEntityManager::GetRandomDestination()
{
	gen::CVector2 size = GSceneManager::GetInstance()->GetWorldSize();

	return gen::CVector2(RandomFloat(0, size.x), RandomFloat(0, size.y));
}

#ifdef _DEBUG
bool GEntityManager::SetAgentWatched(UID agentID, bool isWatched)
{
	GAgent* theAgent;
	if (GetAgent(agentID, theAgent))
	{
		theAgent->SetWatched(isWatched);
		return true;
	}
	return false;
}
bool GEntityManager::GetAgentWatched(UID agentID)
{
	GAgent* theAgent;
	if (GetAgent(agentID, theAgent))
	{
		return theAgent->GetWatched();
	}
	return false;

}

#endif
