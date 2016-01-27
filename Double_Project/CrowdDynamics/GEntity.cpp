#include "GEntity.hpp"

GEntity::GEntity(CVector2 iPosition, bool iIsActive) :
	GObject(iIsActive), 
	m_Matrix(iPosition)
{
}

GEntity::GEntity(float iXPos, float iYPos, bool iIsActive) : 
	GEntity(CVector2(iXPos, iYPos), iIsActive)
{
	//Delagated Constructor - No Body
}

GEntity::~GEntity()
{
}

CVector2 GEntity::GetPosition()
{
	return m_Matrix.GetPosition2D();
}

CVector2 GEntity::GetFacingVector()
{
	return CVector2(m_Matrix.GetRow(1));	//Get the 2nd 'Facing' row of the matrix
}

CMatrix3x3 GEntity::GetMatrix()
{
	return m_Matrix;
}

#ifdef _DEBUG
std::string GEntity::ToString()
{
	std::stringstream builder;

	builder << GObject::ToString() << "Position: X: " << m_Matrix.GetPosition2D().x << " Y: " << m_Matrix.GetPosition2D().y << "/n";
	return builder.str();
}
#endif

void GEntity::SetMatrix(CMatrix3x3& iMatrix)
{
	m_Matrix = iMatrix;
}