#include "GEntityManager.h"

#include "GEntity.h"
#include "GAgent.h"
#include "GSceneManager.h"

GEntityManager::GEntityManager(GSceneManager* iParentManager):
	mManager_Parent(iParentManager)
{
}

GEntityManager::~GEntityManager()
{

	//Deallocate entity information
	for (auto &agent : m_ActiveAgents)
	{
		delete agent;
	}
	for (auto &agent : m_InactiveAgents)
	{
		delete agent;
	}
}

void GEntityManager::AddAgent(gen::CVector2 iPosition, bool iIsActive)
{
	//Construct a new agent
	GAgent* newAgent = new GAgent(iPosition, iIsActive);
	 
	//Push the agent onto the relevant entitylist
	if (iIsActive)
	{
		m_ActiveAgents.push_back(newAgent);
	}
	else //inactive
	{
		m_InactiveAgents.push_back(newAgent);
	}
}

void GEntityManager::AddAgent(float iXPos, float iYPos, bool iIsActive)
{
	//Delegate implementation to other version of the function - preventing duplication errors
	this->AddAgent(gen::CVector2(iXPos, iYPos), iIsActive);
}

void GEntityManager::Update(float updateTime)
{
	for (auto &agent: m_ActiveAgents)
	{
		agent->Update(updateTime);

		//Check if reached dest then give new random dest if true
		if (agent->HasReachedDestination())
		{
			agent->SetNewDestination(this->GetRandomDestination());
		}
	}
}

gen::CVector2 GEntityManager::GetRandomDestination()
{
	gen::CVector2 size = mManager_Parent->GetWorldSize();

	return gen::CVector2(RandomFloat(0, size.x), RandomFloat(0, size.y));
}