#pragma once

#include "Common.hpp"
#include <exception>
using std::exception;

class GInfluenceMap
{
private:
	int m_xSquares;
	int m_ySquares;

	CVector2 m_MapSize;
	CVector2 m_SquareSize;

	float* m_Map;

public:
	GInfluenceMap(int iXSquares, int iYSquares, CVector2 squareSize);
	~GInfluenceMap();

	float GetValue(int xPos, int yPos);
	void ResetMap();	//Sets all squares to 0 influence
	void AddValue(int xPos, int yPos, float value);
	//Deprecated: float GetAccumulatedCost(int xPos, int yPos, float radius);
	float GetSquareGradient(int xPos, int yPos);	//Determines the gradient for the provided grid reference //TODO: implement this


	GIntPair GetGridSquareFromPosition(const CVector2 &position);
	CVector2 GetSquareCentre(int xPos, int yPos);
};

