#include "GSceneManager.h"

#include "GEntityManager.h"
#include "GObstacleManager.h"

GSceneManager::GSceneManager(float iTimeStep, float iWorldXSize, float iWorldYSize, int iXSubdivisions, int iYSubdivisions) :
	GSceneManager(iTimeStep, gen::CVector2(iWorldXSize, iWorldYSize), iXSubdivisions, iYSubdivisions)
{
}

GSceneManager::GSceneManager(float iTimeStep, gen::CVector2 iWorldSize, int iXSubdivisions, int iYSubdivisions) :
	mManager_Entity(new GEntityManager(this)),
	mManager_Obstacle(new GObstacleManager(this)),
	m_TimeStep(iTimeStep),
	m_WorldSize(iWorldSize),
	m_SceneSquares(new GSceneSquare*[iXSubdivisions * iYSubdivisions])
{
	float xSquareSize = iWorldSize.x / iXSubdivisions;
	float ySquareSize = iWorldSize.y / iYSubdivisions;

	//Construct the scenesquare objects
	for (int i = 0; i < iXSubdivisions; i++)
	{
		for (int j = 0; j < iYSubdivisions; j++)
		{
			m_SceneSquares[i * j] = new GSceneSquare(xSquareSize, ySquareSize, i * xSquareSize, j * ySquareSize);
		}
	}
}

GSceneManager::~GSceneManager()
{
	int noSquares = m_NoOfSquaresX * m_NoOfSquaresY;

	//Deallocate each scene square
	for (int i = 0; i < noSquares; i++)
	{
		delete m_SceneSquares[i];
	}
	//Deallocate the scenesquare array
	delete m_SceneSquares;

	//Deallocate Managers
	delete mManager_Entity;
	delete mManager_Obstacle;

}

gen::CVector2 GSceneManager::GetWorldSize()
{
	return m_WorldSize;
}

void GSceneManager::Update()
{
	//Begin the update tree

}