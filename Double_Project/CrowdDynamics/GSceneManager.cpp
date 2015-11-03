#include "GSceneManager.h"



GSceneManager::GSceneManager(float worldXSize, float worldYSize, int xSubdivisions, int ySubdivisions) :
	GSceneManager(CVector2(worldXSize, worldYSize), xSubdivisions, ySubdivisions)
{
}

GSceneManager::GSceneManager(CVector2 worldSize, int xSubdivisions, int ySubdivisions)
{
	m_SceneSquares = new GSceneSquare*[xSubdivisions * ySubdivisions];

	float xSquareSize = worldSize.x / xSubdivisions;
	float ySquareSize = worldSize.y / ySubdivisions;

	//Construct the scenesquare objects
	for (int i = 0; i < xSubdivisions; i++)
	{
		for (int j = 0; j < ySubdivisions; j++)
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

}
