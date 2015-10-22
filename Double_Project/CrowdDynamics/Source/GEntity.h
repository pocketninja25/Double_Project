#pragma once
#include "GObject.h"
#include "CMatrix3x3.h"
#include "CVector2.h"

using gen::CMatrix3x3;
using gen::CVector2;

class CGEntity :
	public CGObject
{
public:
	CGEntity();
	virtual ~CGEntity();

	CVector2 GetPosition()
	{
		return CVector2(matrix.GetRow(2));	//Get the row 2 - position, cast CVector3 to CVector2 by deleting the z value
	}
	CVector2 GetFacingVector()
	{
		return CVector2(matrix.GetRow(1)); 	//Get the row 1 - yVector (facing), cast CVector3 to CVector2 by deleting the z value
	}

private:
	//Position
	CMatrix3x3 matrix;
};
