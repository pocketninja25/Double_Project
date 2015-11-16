#pragma once

#include "Common.hpp"
#include "GSceneSquare.hpp"

//Forward declarations of other manager classes, dont want to include until cpp where possible to avoid circular dependency
class GEntityManager;
class GObstacleManager;

struct SWorldInfo
{
	//Struct of data required to create a 'world instance' - scene manager
public:
	float TimeStep = 0.0f;
	gen::CVector2 WorldSize = gen::CVector2(0.0f, 0.0f);
	int xSubdivisions = 0;
	int ySubdivisions = 0;
};

//This is the primary manager, it manages the scene, as well as the other managers
class GSceneManager
{
//---------------------------
// Singleton Data
//---------------------------
public: 
	static GSceneManager* GetInstance(SWorldInfo* iWorldData = 0);	//Accepts struct pointer of world data - this allows the Scene manager to have a complex setup through getinstance but not require any data to get the instance subsequent times
																	//All objects created by this class will also reference it this way, they do not need to worry about the instance not existing because they cannot exist without this
private:
	static GSceneManager* mManager_Scene;
	GSceneManager(SWorldInfo iWorldInfo);
	GSceneManager(float iTimeStep, gen::CVector2 iWorldSize, int xSubdivisions, int ySubdivisions);
	GSceneManager(float iTimeStep, float iWorldXSize, float iWorldYSize, int iXSubdivisions, int iYSubdivisions);
	
	//Delete copy constructor and = operator
	
	//Deleted
	GSceneManager(GSceneManager const&) = delete;
	//Deleted
	void operator=(GSceneManager const&) = delete;

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
	gen::CVector2 m_SquareSize;		//The size of a single grid square /*Can be calculated, but less expensive to just store*/
	GSceneSquare** m_SceneSquares;	//Pointer to the 0th element in a dynamically allocated array sized m_NoOfSquaresX * m_NoOfSquaresY, access/positioning of elements defined by m_SceneSquares[x][y]
	int m_NoOfSquaresX;				//Number of squares in the X (horizontal) direction
	int m_NoOfSquaresY;				//Number of squares in the Y (vertical) direction


	bool m_Paused;

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************
	virtual ~GSceneManager();

	//***************************
	// Getters/Accessors
	//***************************
	gen::CVector2 GetWorldSize();
	bool GetAgentMatrix(UID requestedUID, gen::CMatrix3x3 &matrix);
	bool GetAgentPosition(UID requestedUID, gen::CVector2 &position);


#ifdef _DEBUG
	bool GetAgentString(UID requestedID, std::string& agentString);
	bool GetSquareString(int xPos, int yPos, std::string& squareString);
#endif

	bool GetIsPaused();

	//***************************
	// Setters/Mutators
	//***************************
	UID AddAgent(gen::CVector2 iPosition, bool iIsActive);
	UID AddAgent(float iXPos, float iYPos, bool iIsActive);
	std::vector<UID> AddXAgents(int kNoAgents, bool iAreActive = true);		//Creates an amount of agents in random positions in the world and returns a vector of their UID's

	void SetPaused(bool iPaused);

	//***************************
	// Other Functions
	//***************************
	void Update(float frameTime);

//---------------------------
// Private Functions
//---------------------------
private:
	void MaintainSceneSquares();
};

