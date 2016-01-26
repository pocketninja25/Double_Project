#include "GInfluenceMap.hpp"



GInfluenceMap::GInfluenceMap(int iXSquares, int iYSquares, gen::CVector2 squareSize):
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

float GInfluenceMap::GetAccumulatedCost(int xPos, int yPos, float radius)
{
	int convertedRadius = static_cast<int>(radius);
	
	int left = xPos - convertedRadius;
	int right = xPos + convertedRadius;
	int bottom = yPos - convertedRadius;
	int top = yPos + convertedRadius;
	
	gen::CVector2 centrePosition;
	GetSquareCentre(xPos, yPos, centrePosition);

	gen::CVector2 thisSquarePosition;
	float total = 0.0f;

	for (int x = left; x <= right; x++)
	{
		for (int y = bottom; y <= top; y++)
		{
			GetSquareCentre(x, y, thisSquarePosition); 
			if ((centrePosition-thisSquarePosition).Length() <= radius)
			{
				total += GetValue(x, y);
			}
		}
	}

	return total;
}

void GInfluenceMap::GetGridSquareFromPosition(const gen::CVector2 & position, int & xPos, int & yPos)
{
	//position/square width rounded down to the nearest integer = the square to add to
	xPos = static_cast<int>(position.x / m_SquareSize.x);
	yPos = static_cast<int>(position.y / m_SquareSize.y);

	//Do the clamp
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

void GInfluenceMap::GetSquareCentre(int xPos, int yPos, gen::CVector2 & position)
{
	position.x = (xPos * m_SquareSize.x) + (m_SquareSize.x / 2);
	position.y = (yPos * m_SquareSize.y) + (m_SquareSize.y / 2);

}