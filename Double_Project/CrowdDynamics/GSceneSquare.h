#pragma once

#include "GObject.h"
#include "CVector2.h"
using gen::CVector2;
#include <vector>
using std::vector;

class GSceneSquare : public GObject
{
//---------------------------
// Private Data Members
//---------------------------

private:
	CVector2 m_Size;		//Width * Height of the Scene Square
	CVector2 m_Centre;		//The centre position of the square - may be better if done as a corner of the square rather than the centre, for now centre is conceptually easier
	vector<UID> m_Walls;	//Vector of UID's to wall objects that occupy/intersect this scene square

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************

	GSceneSquare();

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

	virtual string ToString();

};

