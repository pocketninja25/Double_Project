#include "GInfluenceMap.hpp"



GInfluenceMap::GInfluenceMap(int iXSquares, int iYSquares, CVector2 squareSize):
	m_xSquares(iXSquares),
	m_ySquares(iYSquares),
	m_SquareSize(squareSize)
{
	m_MapSize.x = m_SquareSize.x * m_xSquares;
	m_MapSize.y = m_SquareSize.y * m_ySquares;

	m_Map = new float[m_xSquares * m_ySquares];

	ResetMap();
}

GInfluenceMap::~GInfluenceMap()
{
	delete[] m_Map;
}

void GInfluenceMap::ResetMap()
{
	for (int i = 0; i < m_xSquares * m_ySquares; i++)
	{
		m_Map[i] = 0.0f;
	}
}

float GInfluenceMap::GetValue(int xPos, int yPos)
{

	if (xPos >= m_xSquares)
	{
		xPos = m_xSquares - 1;
	}
	else if (xPos < 0)
	{
		xPos = 0;
	}
	else if (yPos >= m_ySquares)
	{
		yPos = m_ySquares - 1;
	}
	else if (yPos < 0)
	{
		yPos = 0;
	}

	return m_Map[xPos * m_xSquares + yPos];
}

void GInfluenceMap::AddValue(int xPos, int yPos, float value)
{
	if (xPos >= m_xSquares)
	{
		xPos = m_xSquares - 1;
	}
	else if(xPos < 0)
	{
		xPos = 0;
	}
	else if (yPos >= m_ySquares)
	{
		yPos = m_ySquares - 1;
	}
	else if (yPos < 0)
	{
		yPos = 0;
	}

	m_Map[xPos * m_xSquares + yPos] += value;
}


float GInfluenceMap::GetSquareGradient(int xPos, int yPos, CVector2 myPos)
{
	CVector2 itsPos = this->GetSquareCentre(xPos, yPos);

	float dY = myPos.y - itsPos.y;
	float dX = myPos.x - itsPos.x;

	return dY / dX;
}


//float GInfluenceMap::GetAccumulatedCost(int xPos, int yPos, float radius)
//{
//	int convertedRadius = static_cast<int>(radius);
//	
//	int left = xPos - convertedRadius;
//	int right = xPos + convertedRadius;
//	int bottom = yPos - convertedRadius;
//	int top = yPos + convertedRadius;
//	
//	CVector2 centrePosition;
//	GetSquareCentre(xPos, yPos, centrePosition);
//
//	CVector2 thisSquarePosition;
//	float total = 0.0f;
//
//	for (int x = left; x <= right; x++)
//	{
//		for (int y = bottom; y <= top; y++)
//		{
//			GetSquareCentre(x, y, thisSquarePosition); 
//			if ((centrePosition-thisSquarePosition).Length() <= radius)
//			{
//				total += GetValue(x, y);
//			}
//		}
//	}
//
//	return total;
//}

GIntPair GInfluenceMap::GetGridSquareFromPosition(const CVector2 & position)
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