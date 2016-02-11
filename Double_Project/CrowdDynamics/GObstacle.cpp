#include "GObstacle.hpp"

#include "GObstacleTemplate.hpp"
#include "GSceneManager.hpp"

GObstacle::GObstacle(GObstacleTemplate* iTemplate, CVector2 iPosition, bool iIsActive) :
	GObject(iIsActive),
	m_Template(iTemplate),
	m_Matrix(iPosition)
{
}

GObstacle::~GObstacle()
{
	//Dont need to deallocate template as dont own it
}

CVector2 GObstacle::GetPosition()
{
	return m_Matrix.GetPosition2D();
}

CVector2 GObstacle::GetFacingVector()
{
	return CVector2(m_Matrix.GetRow(1));	//Get the 2nd 'Facing' row of the matrix
}

void GObstacle::Update(float updateTime)
{
	//GInfluenceMap* influenceMap = GSceneManager::GetInstance()->GetInfluenceMap();
	//
	//CVector2 modifiedBottomLeft = m_Template->GetBottomLeft() + this->GetPosition();
	//CVector2 modifiedTopRight = m_Template->GetTopRight() + this->GetPosition();
	//
	//GIntPair iBottomLeft = influenceMap->GetGridSquareFromPosition(modifiedBottomLeft);
	//GIntPair iTopRight = influenceMap->GetGridSquareFromPosition(modifiedTopRight);
	//
	//const std::vector<SWall> wallList = m_Template->GetVertexPairs();
	//long int count = 0;
	////For each influence square
	//for (int i = iBottomLeft.x; i < iTopRight.x; i++)
	//{
	//	for (int j = iBottomLeft.y; j < iTopRight.y; j++)
	//	{
	//		bool isInside = true;		//Assume the square is inside until evidence is found otherwise
	//		for (auto wall : wallList)
	//		{
	//			GIntPair thisWallIndices = wall.vertexIndices;
	//			CVector2 thisWallPosition = m_Template->GetVertex(wall.vertexIndices.x); //Approximated by using the x vector
	//			CVector2 squareToWall = thisWallPosition - influenceMap->GetSquareCentre(i, j);
	//			squareToWall.Normalise();
	//			if (squareToWall.Dot(wall.facingVector) > 0)	//Angle between is <= 90degrees (this square is in front of the wall, therefore it is outside the object)
	//			{
	//				isInside = false;
	//				break;	//Break out of the walllist loop
	//			}
	//
	//		}
	//		if (isInside)	//If the influence square is confirmed to be inside the obstacle
	//		{
	//			count++;
	//			//Set this influence square to be blocked
	//			influenceMap->SetBlocked(i, j, true);
	//		}
	//	}
	//}
}

bool GObstacle::ContainsPosition(CVector2 position)
{
	GInfluenceMap* influenceMap = GSceneManager::GetInstance()->GetInfluenceMap();
	const std::vector<SWall> wallList = m_Template->GetVertexPairs();
	
	for (auto wall : wallList)
	{
		GIntPair thisWallIndices = wall.vertexIndices;
		CVector2 thisWallPosition = m_Matrix.TransformPoint2D(m_Template->GetVertex(wall.vertexIndices.x)); //Approximated by using the x vector (modify the point by the matrix)
		CVector2 thisWallFacing = m_Matrix.TransformVector2D(wall.facingVector);
		CVector2 squareToWall = thisWallPosition - position;
		squareToWall.Normalise();
		if (squareToWall.Dot(thisWallFacing) > 0)	//Angle between is <= 90degrees (this square is in front of the wall, therefore it is outside the object)
		{
			return false;	//Proven obstacle is outside at least one wall, therefore outside the obstacle
			
		}

	}
	return true;	//Proven not outside therefore obstacle is inside
}

#ifdef _DEBUG
std::string GObstacle::ToString()
{
	std::stringstream builder;
	
	builder << GObject::ToString() << "Position: X: " << m_Matrix.GetPosition2D().x << " Y: " << m_Matrix.GetPosition2D().y << "/n"
		<< "Template UID: " << m_Template->GetUID() << "/n";

	return builder.str();
}

#endif