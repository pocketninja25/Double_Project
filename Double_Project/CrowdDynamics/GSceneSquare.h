#pragma once

#include "Common.h"
#include "GObject.h"
#include <vector>

class GSceneSquare : public GObject
{
//---------------------------
// Private Data Members
//---------------------------

private:
	gen::CVector2 m_Size;		//Width * Height of the Scene Square
	gen::CVector2 m_Origin;		//The bottom left position of the square
	std::vector<UID> m_Walls;	//Vector of UID's to wall objects that occupy/intersect this scene square

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************

	GSceneSquare(gen::CVector2 iSize, gen::CVector2 iOriginPos, bool iIsActive = true);
	GSceneSquare(gen::CVector2 iSize, float iOriginXPos, float iOriginYPos, bool iIsActive = true);
	GSceneSquare(float iXSize, float iYSize, gen::CVector2 iOriginPos, bool iIsActive = true);
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
	virtual std::string ToString();
#endif
};

