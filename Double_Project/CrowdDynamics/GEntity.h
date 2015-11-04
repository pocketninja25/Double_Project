#pragma once

#include "GObject.h"
#include "CMatrix3x3.h"
using gen::CMatrix3x3;
using gen::CVector2;

class GEntity :	public GObject
{
//---------------------------
// Private Data Members
//---------------------------
private:
	CMatrix3x3 m_Matrix;

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************

	GEntity(CVector2 iPosition, bool iIsActive = true);
	GEntity(float iXPos, float iYPos, bool iIsActive = true);

	virtual ~GEntity();

	//***************************
	// Getters/Accessors
	//***************************
	CVector2 GetPosition();
	CVector2 GetFacingVector();

	//***************************
	// Setters/Mutators
	//***************************

	//***************************
	// Other Functions
	//***************************
	virtual void Update(float updateTime) = 0;	//Inherited pure virtual Update function, also pure virtual here to prevent instansiation of GEntity

#ifdef _DEBUG
	virtual string ToString();
#endif

//---------------------------
// Protected Functions
//---------------------------
protected:
	//***************************
	// Getters/Accessors
	//***************************
	CMatrix3x3 GetMatrix();

};
