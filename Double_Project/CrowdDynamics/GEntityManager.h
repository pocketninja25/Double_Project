#pragma once

#include "Common.h"
#include <vector>

//Forward declare these classes, include the files for the cpp only (where needed)
class GEntity;

class GEntityManager
{
//---------------------------
// Private Data Members
//---------------------------
private:
	std::vector<GEntity*> m_ActiveEntities;
	std::vector<GEntity*> m_InactiveEntities;

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************

	GEntityManager();

	virtual ~GEntityManager();

	//***************************
	// Getters/Accessors
	//***************************

	//***************************
	// Setters/Mutators
	//***************************
	void AddAgent(gen::CVector2 iPosition, bool iIsActive);
	void AddAgent(float iXPos, float iYPos, bool iIsActive);

	//***************************
	// Other Functions
	//***************************

	void Update(float updateTime);

};

