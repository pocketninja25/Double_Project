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
	for (auto entity : m_ActiveEntities)
	{
		delete entity;
	}
	for (auto entity : m_InactiveEntities)
	{
		delete entity;
	}
}

void GEntityManager::AddAgent(gen::CVector2 iPosition, bool iIsActive)
{
	//Construct a new agent
	GAgent* newAgent = new GAgent(iPosition, iIsActive);
	 
	//Push the agent onto the relevant entitylist
	if (iIsActive)
	{
		m_ActiveEntities.push_back(newAgent);
	}
	else //inactive
	{
		m_InactiveEntities.push_back(newAgent);
	}
}

void GEntityManager::AddAgent(float iXPos, float iYPos, bool iIsActive)
{
	//Delegate implementation to other version of the function - preventing duplication errors
	this->AddAgent(gen::CVector2(iXPos, iYPos), iIsActive);
}

void GEntityManager::Update(float updateTime)
{
	for (auto entity: m_ActiveEntities)
	{
		entity->Update(updateTime);
	}
}

void GEntityManager::GetRandomDestination()
{
	gen::CVector2 size = mManager_Parent->GetWorldSize();


}