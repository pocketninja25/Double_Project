#pragma once

#include "GSceneSquare.h"
#include "CVector2.h"
using gen::CVector2;

class GSceneManager
{
//---------------------------
// Private Data Members
//---------------------------
private:
	GSceneSquare** m_SceneSquares;	//Pointer to the 0th element in a dynamically allocated array sized m_NoOfSquaresX * m_NoOfSquaresY, access/positioning of elements defined by m_SceneSquares[x][y]
	int m_NoOfSquaresX;				//Number of squares in the X (horizontal) direction
	int m_NoOfSquaresY;				//Number of squares in the Y (vertical) direction

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************

	GSceneManager(CVector2 worldSize, int xSubdivisions, int ySubdivisions);
	GSceneManager(float worldXSize, float worldYSize, int xSubdivisions, int ySubdivisions);
		
	virtual ~GSceneManager();

	//***************************
	// Getters/Accessors
	//***************************

	//***************************
	// Setters/Mutators
	//***************************

	//***************************
	// Other Functions
	//***************************
};

