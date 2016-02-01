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
	CVector2* m_Flow;

public:
//***************************
// Constructors/Destructors
//***************************
	GInfluenceMap(GIntPair noSquares, CVector2 squareSize);
	GInfluenceMap(int iXSquares, int iYSquares, CVector2 squareSize);

	~GInfluenceMap();	

//***************************
// Getters/Accessors
//***************************
	float GetValue(int xPos, int yPos);
	CVector2 GetFlow(int xPos, int yPos);	//Get unit vector representing the combined direction of traffic by influencers on that square

//***************************
// Setters/Mutators
//***************************
	void AddValue(int xPos, int yPos, float value);
	void AddFlow(int xPos, int yPos, CVector2 value);	//Provide unit vector representing direction of flow

//***************************
// Other Functions
//***************************
	void ResetMap();	//Sets all squares to 0 influence


	//Convert from world to grid and vice versa
	GIntPair GetGridSquareFromPosition(const CVector2 &position);
	CVector2 GetSquareCentre(int xPos, int yPos);

private:
	void ClampGridCoords(int& xPos, int& yPos);
};

