/**************************************************************************************************
	Module:       MathIO.h
	Author:       Laurent Noel
	Date created: 11/07/07

	Support for stream input and output for math classes

	Copyright 2007, University of Central Lancashire and Laurent Noel

	Change history:
		V1.0    Created 11/07/07 - LN
**************************************************************************************************/

#ifndef GEN_C_MATHIO_H_INCLUDED
#define GEN_C_MATHIO_H_INCLUDED

#include <iostream>

#include "Defines.h"

namespace gen
{

// Forward declaration of classes, includes not necessary in header
class CVector2;
class CVector3;
class CVector4;
class CMatrix2x2;
class CMatrix3x3;
class CMatrix4x4;
class CQuaternion;


/*---------------------------------------------------------------------------------------------
	Vector IO
---------------------------------------------------------------------------------------------*/

std::ostream& operator<<( std::ostream& s, const CVector2& v );
std::ostream& operator<<( std::ostream& s, const CVector3& v );
std::ostream& operator<<( std::ostream& s, const CVector4& v );

std::istream& operator>>( std::istream& s, CVector2& v );
std::istream& operator>>( std::istream& s, CVector3& v );
std::istream& operator>>( std::istream& s, CVector4& v );


/*---------------------------------------------------------------------------------------------
	Matrix IO
---------------------------------------------------------------------------------------------*/

std::ostream& operator<<( std::ostream& s, const CMatrix2x2& m );
std::ostream& operator<<( std::ostream& s, const CMatrix3x3& m );
std::ostream& operator<<( std::ostream& s, const CMatrix4x4& m );


std::istream& operator>>( std::istream& s, CMatrix2x2& m );
std::istream& operator>>( std::istream& s, CMatrix3x3& m );
std::istream& operator>>( std::istream& s, CMatrix4x4& m );

/*---------------------------------------------------------------------------------------------
	Quaternion IO
---------------------------------------------------------------------------------------------*/

std::ostream& operator<<(std::ostream& s, const CQuaternion& v );

std::istream& operator>>(std::istream& s, CQuaternion& v );


} // namespace gen

#endif // GEN_C_MATHIO_H_INCLUDED
