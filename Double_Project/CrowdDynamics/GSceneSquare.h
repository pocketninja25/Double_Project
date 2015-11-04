#pragma once

#include "GObject.h"
#include "CVector2.h"
#include <vector>
using gen::CVector2;
using std::vector;

class GSceneSquare : public GObject
{
//---------------------------
// Private Data Members
//---------------------------

private:
	CVector2 m_Size;		//Width * Height of the Scene Square
	CVector2 m_Origin;		//The bottom left position of the square
	vector<UID> m_Walls;	//Vector of UID's to wall objects that occupy/intersect this scene square

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************

	GSceneSquare(CVector2 iSize, CVector2 iOriginPos, bool iIsActive = true);
	GSceneSquare(CVector2 iSize, float iOriginXPos, float iOriginYPos, bool iIsActive = true);
	GSceneSquare(float iXSize, float iYSize, CVector2 iOriginPos, bool iIsActive = true);
	GSceneSquare(float iXSize, float iYSize, float iOriginXPos, float iOriginYPos, bool iIsActive = true);

	virtual ~GSceneSquare();

	//***************************
	// Getters/Accessors
	//***************************

	//***************************
	// Setters/Mutators
	//***************************

	//***************************
	// Other Functions
	//***************************
	virtual void Update(float updateTime);

#ifdef _DEBUG
	virtual string ToString();
#endif
};

