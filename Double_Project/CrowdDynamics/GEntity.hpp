#pragma once

#include "Common.hpp"
#include "GObject.hpp"

class GEntity :	public GObject
{
//---------------------------
// Private Data Members
//---------------------------
private:
	CMatrix3x3 m_Matrix;

	float scale;

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
	CMatrix3x3 GetMatrix();

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
	void SetMatrix(CMatrix3x3& iMatrix);

};
