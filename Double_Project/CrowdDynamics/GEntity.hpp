#pragma once

#include "Common.hpp"
#include "GObject.hpp"

class GEntity :	public GObject
{
//---------------------------
// Private Data Members
//---------------------------
private:
	gen::CMatrix3x3 m_Matrix;

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************

	GEntity(gen::CVector2 iPosition, bool iIsActive = true);
	GEntity(float iXPos, float iYPos, bool iIsActive = true);

	virtual ~GEntity();

	//***************************
	// Getters/Accessors
	//***************************
	gen::CVector2 GetPosition();
	gen::CVector2 GetFacingVector();
	gen::CMatrix3x3 GetMatrix();

	//***************************
	// Setters/Mutators
	//***************************

	//***************************
	// Other Functions
	//***************************
	virtual void Update(float updateTime) = 0;	//Inherited pure virtual Update function, also pure virtual here to prevent instansiation of GEntity

#ifdef _DEBUG
	virtual std::string ToString();
#endif

//---------------------------
// Protected Functions
//---------------------------
protected:
	//***************************
	// Getters/Accessors
	//***************************
	void SetMatrix(gen::CMatrix3x3& iMatrix);

};