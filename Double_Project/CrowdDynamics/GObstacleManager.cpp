#include "GObstacleManager.hpp"
#include "GStaticObstacle.hpp"
#include "GDynamicObstacle.hpp"
#include "GObstacleTemplate.hpp"
#include "GObstacleImporter.hpp"
#include "GInfluenceMap.hpp"
#include "GSceneManager.hpp"

GObstacleManager::GObstacleManager()
{
	m_ObstacleBlueprintLoader = new GObstacleImporter();
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
	delete m_ObstacleBlueprintLoader;
}

UID GObstacleManager::AddStaticObstacle(std::string obstacleBlueprintFile, CVector2 position)
{
	GObstacleTemplate* blueprint = m_ObstacleBlueprintLoader->LoadBlueprint(obstacleBlueprintFile);

	m_StaticObstacles.push_back(new GStaticObstacle(blueprint, position, true));
	return m_StaticObstacles.back()->GetUID();
}

bool GObstacleManager::PositionBlocked(CVector2 position)
{
	for (auto obstacle : m_StaticObstacles)
	{
		if (obstacle->ContainsPosition(position))
		{
			return true;
		}
	}
	for (auto obstacle : m_DynamicObstacles)
	{
		if (obstacle->ContainsPosition(position))
		{
			return true;
		}
	}
	return false;
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


