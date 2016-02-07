#include "GObstacleManager.hpp"
#include "GStaticObstacle.hpp"
#include "GDynamicObstacle.hpp"
#include "GObstacleTemplate.hpp"
#include "GObstacleImporter.hpp"
#include "GInfluenceMap.hpp"
#include "GSceneManager.hpp"

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
}

UID GObstacleManager::AddStaticObstacle(std::string obstacleBlueprintFile, CVector2 position)
{
	GObstacleTemplate* blueprint = m_ObstacleBlueprintLoader->LoadBlueprint(obstacleBlueprintFile);

	m_StaticObstacles.push_back(new GStaticObstacle(blueprint, position, true));
}

void GObstacleManager::Update(float updateTime)
{
	for (auto obstacle : m_StaticObstacles)
	{
		obstacle->Update(updateTime);
	}
	for (auto obstacle : m_DynamicObstacles)
	{
		obstacle->Update(updateTime);
	}

}


