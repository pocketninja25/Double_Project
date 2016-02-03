#include "GInfluenceMap.hpp"

GInfluenceMap::GInfluenceMap(GIntPair noSquares, CVector2 squareSize) :
	GInfluenceMap(noSquares.x, noSquares.y, squareSize)
{
}

GInfluenceMap::GInfluenceMap(int iXSquares, int iYSquares, CVector2 squareSize):
	GObject(true),
	m_xSquares(iXSquares),
	m_ySquares(iYSquares),
	m_SquareSize(squareSize)
{
	m_MapSize.x = m_SquareSize.x * m_xSquares;
	m_MapSize.y = m_SquareSize.y * m_ySquares;

	m_Map = new float[m_xSquares * m_ySquares];
	m_Flow = new CVector2[m_xSquares * m_ySquares];

	ResetInfluence();
	ResetFlow();
}

GInfluenceMap::~GInfluenceMap()
{
	delete[] m_Map;
	delete[] m_Flow;
}

float GInfluenceMap::GetValue(int xPos, int yPos)
{
	ClampGridCoords(xPos, yPos);

	return m_Map[xPos * m_xSquares + yPos];
}

CVector2 GInfluenceMap::GetFlow(int xPos, int yPos)
{
	ClampGridCoords(xPos, yPos);
	CVector2 tempFlow = m_Flow[xPos * m_xSquares + yPos];

	tempFlow.Normalise();

	return tempFlow;
}

void GInfluenceMap::AddValue(int xPos, int yPos, float value)
{
	if (value <= 0)
	{
		return;
	}
	ClampGridCoords(xPos, yPos);

	m_Map[xPos * m_xSquares + yPos] += value;
}

void GInfluenceMap::AddFlow(int xPos, int yPos, CVector2 value)
{
	ClampGridCoords(xPos, yPos);

	m_Flow[xPos * m_xSquares + yPos] += value;
}


GIntPair GInfluenceMap::GetGridSquareFromPosition(const CVector2& position)
{
	GIntPair square;
	//position/square width rounded down to the nearest integer = the square to add to
	square.x = static_cast<int>(position.x / m_SquareSize.x);
	square.y = static_cast<int>(position.y / m_SquareSize.y);

	//Do the clamp
	if (square.x >= m_xSquares)
	{
		square.x = m_xSquares - 1;
	}
	else if (square.x < 0)
	{
		square.x = 0;
	}
	if (square.y >= m_ySquares)
	{
		square.y = m_ySquares - 1;
	}
	else if (square.y < 0)
	{
		square.y = 0;
	}
	return square;
}

CVector2 GInfluenceMap::GetSquareCentre(int xPos, int yPos)
{
	CVector2 position;
	position.x = (xPos * m_SquareSize.x) + (m_SquareSize.x / 2);
	position.y = (yPos * m_SquareSize.y) + (m_SquareSize.y / 2);

	return position;
}


void GInfluenceMap::Update(float updateTime)
{
	ResetInfluence();

}

void GInfluenceMap::ResetInfluence()
{
	for (int i = 0; i < m_xSquares * m_ySquares; i++)
	{
		m_Map[i] = 0.0f;
	}
}

void GInfluenceMap::ResetFlow()
{
	for (int i = 0; i < m_xSquares * m_ySquares; i++)
	{
		m_Flow[i] = CVector2(0.0f, 0.0f);
	}
}

void GInfluenceMap::ClampGridCoords(int& xPos, int& yPos)
{
	if (xPos >= m_xSquares)
	{
		xPos = m_xSquares - 1;
	}
	else if (xPos < 0)
	{
		xPos = 0;
	}
	if (yPos >= m_ySquares)
	{
		yPos = m_ySquares - 1;
	}
	else if (yPos < 0)
	{
		yPos = 0;
	}
}
