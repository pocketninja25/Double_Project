#pragma once

#include "Common.h"
#include "GSceneSquare.h"
#include <vector>

//Forward declarations of other manager classes, dont want to include until cpp where possible to avoid circular dependency
class GEntityManager;
class GObstacleManager;

//This is the primary manager, it manages the scene, as well as the other managers
class GSceneManager
{
//---------------------------
// Manager classes
//---------------------------
private:
	GEntityManager* mManager_Entity;
	GObstacleManager* mManager_Obstacle;

//---------------------------
// Private Data Members
//---------------------------
private:
	float m_TimeStep;				//The amount of time updated each frame TODO: make this work concurrently with the client program, updating without a call to the update function (will need to set the program to 'Enabled'/'Disabled'
	float m_TimeSinceLastUpdate;	//The amount of time elapsed since the last update

	gen::CVector2 m_WorldSize;		//The total size of the world space /*Can be calculated, but less expensive to just store*/
	GSceneSquare** m_SceneSquares;	//Pointer to the 0th element in a dynamically allocated array sized m_NoOfSquaresX * m_NoOfSquaresY, access/positioning of elements defined by m_SceneSquares[x][y]
	int m_NoOfSquaresX;				//Number of squares in the X (horizontal) direction
	int m_NoOfSquaresY;				//Number of squares in the Y (vertical) direction

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************

	GSceneManager(float iTimeStep, gen::CVector2 iWorldSize, int xSubdivisions, int ySubdivisions);
	GSceneManager(float iTimeStep, float iWorldXSize, float iWorldYSize, int iXSubdivisions, int iYSubdivisions);

	virtual ~GSceneManager();

	//***************************
	// Getters/Accessors
	//***************************
	gen::CVector2 GetWorldSize();
	bool GetAgentMatrix(UID requestedUID, gen::CMatrix3x3& matrix);

#ifdef _DEBUG
	bool GetAgentString(UID requestedID, std::string& agentString);
#endif

	//***************************
	// Setters/Mutators
	//***************************
	UID AddAgent(gen::CVector2 iPosition, bool iIsActive);
	UID AddAgent(float iXPos, float iYPos, bool iIsActive);
	std::vector<UID> AddXAgents(int kNoAgents, bool iAreActive = true);		//Creates an amount of agents in random positions in the world and returns a vector of their UID's

	//***************************
	// Other Functions
	//***************************
	void Update(float frameTime);

};

