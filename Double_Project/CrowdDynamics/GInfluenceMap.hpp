#pragma once

#include "Common.hpp"
#include "GObject.hpp"

class GInfluenceMap : GObject
{
private:
	int m_xSquares;
	int m_ySquares;

	CVector2 m_MapSize;
	CVector2 m_SquareSize;

	float* m_Map;
	CVector2* m_Flow;
	bool* m_Blocked;

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
	float GetValue(GIntPair pos);
	CVector2 GetFlow(int xPos, int yPos);	//Get unit vector representing the combined direction of traffic by influencers on that square
	CVector2 GetFlow(GIntPair pos);
	bool GetIsBlocked(int xPos, int yPos);
	bool GetIsBlocked(GIntPair pos);
//***************************
// Setters/Mutators
//***************************
	void AddValue(int xPos, int yPos, float value);
	void AddFlow(int xPos, int yPos, CVector2 value);	//Provide unit vector representing direction of flow
	void SetBlocked(int xPos, int yPos, bool isBlocked);

	void ResetInfluence();	//Sets all squares to 0 influence
	void ResetFlow();
	void ResetBlocked();
//***************************
// Other Functions
//***************************
	void Update(float updateTime);
	

	//Convert from world to grid and vice versa
	GIntPair GetGridSquareFromPosition(const CVector2 &position);
	CVector2 GetSquareCentre(int xPos, int yPos);

private:
	void ClampGridCoords(int& xPos, int& yPos);
};

