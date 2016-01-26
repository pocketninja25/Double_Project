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
			returnedAgent = m_InactiveAgents.at(request);

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

bool GEntityManager::SetAgentActivation(UID agent, bool isEnabled)
{
	GAgent* theAgent;
	if (this->GetAgent(agent, theAgent))
	{
		if (theAgent->IsActive() != isEnabled)	//If the agent is not already in the desired state
		{
			if (isEnabled)
			{
				//Enable the agent
				theAgent->Activate();
				m_InactiveAgents.erase(agent);
				m_ActiveAgents.insert(std::make_pair(agent, theAgent));
			}
			else
			{
				//Disable the agent
				theAgent->Deactivate();
				m_ActiveAgents.erase(agent);
				m_InactiveAgents.insert(std::make_pair(agent, theAgent));
			}
		}

	}
	
	return false;

}

void GEntityManager::Update(float updateTime)
{
	GInfluenceMap* influenceMap = GSceneManager::GetInstance()->GetInfluenceMap();

	for (auto &agent : m_ActiveAgents)
	{
		agent.second->CalculateInfluence(influenceMap);
	}

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

void GEntityManager::PerformCollisionAvoidance(const std::list<UID>& localAgents)
{
	//Check that each UID in the localAgents list is an existing agent
	std::vector<GAgent*> potentiallyCollidingAgents;
	GAgent* theAgent;
	for (auto agentID : localAgents)
	{
		if (GetAgent(agentID, theAgent))	//Agent with this UID exists - add the theAgent pointer to the list of potentially colliding agents
		{
			potentiallyCollidingAgents.push_back(theAgent);
		}
	}

	//One by one perform collision avoidance algorithms on the list of agents
	for (auto agent : potentiallyCollidingAgents)
	{
		agent->GiveLocalAgentsList(potentiallyCollidingAgents);
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
