#pragma once

#include "Common.hpp"
#include <exception>
using std::exception;

class GInfluenceMap
{
private:
	int m_xSquares;
	int m_ySquares;

	gen::CVector2 m_MapSize;
	gen::CVector2 m_SquareSize;

	float* m_Map;

public:
	GInfluenceMap(int iXSquares, int iYSquares, gen::CVector2 squareSize);
	~GInfluenceMap();

	float GetValue(int xPos, int yPos);
	void ResetMap();	//Sets all squares to 0 influence
	void AddValue(int xPos, int yPos, float value);
	//Deprecated: float GetAccumulatedCost(int xPos, int yPos, float radius);
	float GetSquareGradient(int xPos, int yPos);	//Determines the gradient for the provided grid reference //TODO: implement this


	void GetGridSquareFromPosition(const gen::CVector2 &position, int &xPos, int &yPos);
	void GetSquareCentre(int xPos, int yPos, gen::CVector2 &position);
};

