#include "GObstacleManager.h"
#include "GStaticObstacle.h"
#include "GDynamicObstacle.h"
#include "GObstacleTemplate.h"

GObstacleManager::GObstacleManager(GSceneManager* iParent) :
	mManager_Parent(iParent)
{
}

GObstacleManager::~GObstacleManager()
{
	//Deallocate obstacles
	for (auto &obstacle : m_StaticObstacles)
	{
		delete obstacle;
	}
	for (auto &obstacle : m_DynamicObstacles)
	{
		delete obstacle;
	}
	//Deallocate templates
	for (auto &obsTemplate : m_ObstacleTemplates)
	{
		delete obsTemplate;
	}
}
