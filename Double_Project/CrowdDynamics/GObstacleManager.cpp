#include "GObstacleManager.hpp"
#include "GStaticObstacle.hpp"
#include "GDynamicObstacle.hpp"
#include "GObstacleTemplate.hpp"
#include "GObstacleImporter.hpp"


GObstacleManager::GObstacleManager()
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

UID GObstacleManager::AddStaticObstacle(std::string obstacleBlueprintFile, CVector2 position)
{
	m_ObstacleBlueprintLoader->Load
}


