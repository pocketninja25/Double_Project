#pragma once


class GSceneSquare;	//Forward declaration of scene square class

class GSceneManager
{
//---------------------------
// Private Data Members
//---------------------------
private:
	GSceneSquare* m_SceneSquares;	//Pointer to the 0th element in a dynamically allocated array sized m_NoOfSquaresX * m_NoOfSquaresY, access/positioning of elements defined by m_SceneSquares[x][y]
	int m_NoOfSquaresX;				//Number of squares in the X (horizontal) direction
	int m_NoOfSquaresY;				//Number of squares in the Y (vertical) direction

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************

	GSceneManager();

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

