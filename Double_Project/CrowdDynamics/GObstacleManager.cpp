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

bool GObstacleManager::GetObstacleMesh(UID requestedID, std::string & meshFile)
{
	for (auto obstacle : m_StaticObstacles)
	{
		if (obstacle->GetUID() == requestedID)
		{
			meshFile = obstacle->GetTemplate()->GetMeshFile();
			return true;
		}
	}
	for (auto obstacle : m_DynamicObstacles)
	{
		if (obstacle->GetUID() == requestedID)
		{
			meshFile = obstacle->GetTemplate()->GetMeshFile();
			return true;
		}
	}

	return false;
}

bool GObstacleManager::GetObstacleMatrix(UID requestedID, CMatrix3x3& matrix)
{
	for (auto obstacle : m_StaticObstacles)
	{
		if (obstacle->GetUID() == requestedID)
		{
			matrix = obstacle->GetMatrix();
			return true;
		}
	}
	for (auto obstacle : m_DynamicObstacles)
	{
		if (obstacle->GetUID() == requestedID)
		{
			matrix = obstacle->GetMatrix();
			return true;
		}
	}

	return false;
}

std::map<std::string, float> GObstacleManager::GetObstacleMeshes()
{
	std::map<std::string, float> meshes;
	for (auto obstacle : m_StaticObstacles)
	{
		std::string thisMeshFile = obstacle->GetTemplate()->GetMeshFile();
		bool found = false;
		for (auto mesh : meshes)
		{
			if (mesh.first == thisMeshFile)
			{
				found = true;
			}
		}
		if (!found)
		{
			meshes.emplace(thisMeshFile, obstacle->GetTemplate()->GetMeshScale());
		}
	}
	for (auto obstacle : m_DynamicObstacles)
	{
		std::string thisMeshFile = obstacle->GetTemplate()->GetMeshFile();
		bool found = false;
		for (auto mesh : meshes)
		{
			if (mesh.first == thisMeshFile)
			{
				found = true;
			}
		}
		if (!found)
		{
			meshes.emplace(thisMeshFile, obstacle->GetTemplate()->GetMeshScale());
		}
	}
	return meshes;
}

std::vector<UID> GObstacleManager::GetObstacleUIDs()
{
	std::vector<UID> UIDList;
	for (auto obstacle : m_StaticObstacles)
	{
		UIDList.push_back(obstacle->GetUID());
	}
	for (auto obstacle : m_DynamicObstacles)
	{
		UIDList.push_back(obstacle->GetUID());
	}
	return UIDList;
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


