#pragma once

#include "Common.h"
#include "GObject.h"

struct VertexPair
{
	VertexPair(gen::CVector2 iVertexA, gen::CVector2 iVertexB) :
		a(iVertexA),
		b(iVertexB)
	{
	}

	gen::CVector2 a;
	gen::CVector2 b;
};

class GWall : public GObject
{
//---------------------------
// Private Data Members
//---------------------------
private:
	VertexPair m_Vertices;		//Points to a pair of vertices
	gen::CVector2 m_FacingVect;	//The facing vector of the wall (the direction the wall influences)
	UID m_Parent;				//The UID of the static or dynamic obstacle that this wall belongs to

//---------------------------
// Public Functions
//---------------------------
public:
	
	//***************************
	// Constructors/Destructors
	//***************************
	GWall(gen::CVector2 iVertexA, gen::CVector2 iVertexB, gen::CVector2 iFacingVector, UID iParent, bool iIsActive = true);
	virtual ~GWall();

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

	bool IsInFront(gen::CVector2 objectPosition);	//Returns true if the passed object position is "In front" of this wall (within 90 degrees either side of the wall's facing vector)


};