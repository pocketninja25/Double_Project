#pragma once
class GInfluenceMap
{
private:
	int m_xSquares;
	int m_ySquares;

	float* m_Map;

public:
	GInfluenceMap(int iXSquares, int iYSquares);
	~GInfluenceMap();

	float GetValue(int xPos, int yPos);
	void SetValue(int xPos, int yPos, float value);

};

