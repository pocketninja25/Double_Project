#include "GEntity.h"

GEntity::GEntity(gen::CVector2 iPosition, bool iIsActive) :
	GObject(iIsActive), 
	m_Matrix(iPosition)
{
}

GEntity::GEntity(float iXPos, float iYPos, bool iIsActive) : 
	GEntity(gen::CVector2(iXPos, iYPos), iIsActive)
{
	//Delagated Constructor - No Body
}

GEntity::~GEntity()
{
}

gen::CVector2 GEntity::GetPosition()
{
	return m_Matrix.GetPosition2D();
}

gen::CVector2 GEntity::GetFacingVector()
{
	return gen::CVector2(m_Matrix.GetRow(1));	//Get the 2nd 'Facing' row of the matrix
}

#ifdef _DEBUG
string GEntity::ToString()
{
	stringstream builder;

	builder << GObject::ToString() << "Position: X: " << m_Matrix.GetPosition2D().x << " Y: " << m_Matrix.GetPosition2D().y << "/n";
	return builder.str();
}
#endif

gen::CMatrix3x3 GEntity::GetMatrix()
{
	return m_Matrix;
}

void GEntity::SetMatrix(gen::CMatrix3x3& iMatrix)
{
	m_Matrix = iMatrix
}