#pragma once

#include "Common.hpp"
#include "GObject.hpp"

//Forward declare these classes, can include files in the cpp
class GObstacleTemplate;
class GWall;

class GObstacle : public GObject
{

//---------------------------
// Private Data Members
//---------------------------
private:
	GObstacleTemplate* m_Template;	//The template this obstacle is based on TODO: this may be better as a UID, but likely this should be one of the only direct pointers outside of managers

	std::vector<GWall*> m_Walls;	//Vector of the walls owned by this model


//---------------------------
// Protected Data Members
//---------------------------
protected:
	CMatrix3x3 m_Matrix;		//Matrix of obstacle

//---------------------------
// Public Functions
//---------------------------
public:

	//***************************
	// Constructors/Destructors
	//***************************
	GObstacle(GObstacleTemplate* iTemplate, CVector2 iPosition, bool iIsActive = true);
	//TODO: Write a delegating constructor for iPosition
	virtual ~GObstacle();
	
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
	virtual void Update(float updateTime);

#ifdef _DEBUG
	virtual std::string ToString();
#endif
};
