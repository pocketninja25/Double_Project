#pragma once

#include "Common.hpp"
#include "GSceneSquare.hpp"
#include "GInfluenceMap.hpp"

//Forward declarations of other manager classes, dont want to include until cpp where possible to avoid circular dependency
class GEntityManager;
class GObstacleManager;
class GWorldImporter;

//This is the primary manager, it manages the scene, as well as the other managers
class GSceneManager
{
	//---------------------------
	// Singleton Data
	//---------------------------
public:
	static GSceneManager* GetInstance(std::string worldBlueprintFile = "");	//Accepts struct pointer of world data - this allows the Scene manager to have a complex setup through getinstance but not require any data to get the instance subsequent times
																	//All objects created by this class will also reference it this way, they do not need to worry about the instance not existing because they cannot exist without this
private:
	static GSceneManager* mManager_Scene;
	GSceneManager(std::string fileName);
	GSceneManager(SWorldBlueprint iWorldInfo);	//Deprecated
	GSceneManager(float iTimeStep, CVector2 iWorldSize, int xSubdivisions, int ySubdivisions, float influenceSquaresPerUnit);	//Deprecated
	GSceneManager(float iTimeStep, float iWorldXSize, float iWorldYSize, int iXSubdivisions, int iYSubdivisions, float influenceSquaresPerUnit);	//Deprecated


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
	GWorldImporter* m_WorldBlueprintLoader;
	SWorldBlueprint m_WorldBlueprint;

	float m_TimeStep;				//The amount of time updated each frame TODO: make this work concurrently with the client program, updating without a call to the update function (will need to set the program to 'Enabled'/'Disabled'
	float m_TimeSinceLastUpdate;	//The amount of time elapsed since the last update

	CVector2 m_WorldSize;			//The total size of the world space /*Can be calculated, but less expensive to just store*/
	CVector2 m_SquareSize;			//The size of a single grid square /*Can be calculated, but less expensive to just store*/
	GSceneSquare** m_SceneSquares;	//Pointer to the 0th element in a dynamically allocated array sized m_NoOfSquaresX * m_NoOfSquaresY, access/positioning of elements defined by m_SceneSquares[x][y]
	GIntPair m_NoSquares;			//The number of squares in the x and y directions

	bool m_Paused;

	GInfluenceMap* m_InfluenceMap;
	

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
	CVector2 GetWorldSize();
	SWorldBlueprint GetWorldBlueprint();

	std::map<std::string, float> GetAgentMeshes();

	std::vector<UID> GetAgentUIDs();

	bool GetAgentMatrix(UID requestedUID, CMatrix3x3 &matrix);
	bool GetAgentPosition(UID requestedUID, CVector2 &position);
	bool GetAgentDesiredVector(UID requestedUID, CVector2 &desiredVector);
	bool GetAgentDestination(UID requestedUID, CVector2 &destination);
	bool GetAgentMeshFile(UID requestedUID, std::string &meshFile);
	bool GetAgentMeshScale(std::string requestedMesh, float &scale);

	GInfluenceMap* GetInfluenceMap();

	std::vector<UID> GetObstacleUIDs();
	std::map<std::string, float> GetObstacleMeshes();
	bool GetObstacleMesh(UID requestedUID, std::string &mesh);
	bool GetPositionBlockedByObstacle(CVector2 position);

	bool GetObstacleMatrix(UID requestedUID, CMatrix3x3 &matrix);

	float GetTimeStep();

	bool GetIsPaused();

#ifdef _DEBUG
	bool GetAgentString(UID requestedID, std::string& agentString);
	bool GetSquareString(int xPos, int yPos, std::string& squareString);
	bool GetSquareString(CVector2 coordinate, std::string& squareString);
#endif

	//***************************
	// Setters/Mutators
	//***************************
	UID AddAgent(CVector2 iPosition, bool iIsActive);		//Deprecated
	UID AddAgent(float iXPos, float iYPos, bool iIsActive);		//Deprecated
	UID AddAgent(std::string blueprintFile, bool overwriteStartLocation = false, CVector2 newLocation = CVector2(0.0f, 0.0f));

	std::vector<UID> AddXAgents(const int kNoAgents, std::string blueprintFile);		//Creates an amount of agents in random positions in the world and returns a vector of their UID's
	void PerformCollisionAvoidance(const std::list<UID>& localAgents);

	bool SetAgentPosition(UID agent, CVector2 newPosition);
	bool SetAgentActivation(UID agent, bool isEnabled);

	UID AddStaticObstacle(std::string blueprintFile, CVector2 position);

	void SetPaused(bool iPaused);

	//***************************
	// Other Functions
	//***************************
	void Update(float frameTime);
	void PerformOneTimeStep();

	int GetWhichSquare(CVector2 itemPosition);	//Returns the array index of the square that owns this position

//---------------------------
// Private Functions
//---------------------------
private:
	void MaintainSceneSquares();
	
//^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Debug code
//vvvvvvvvvvvvvvvvvvvvvvvvvvv
	public:
		bool SetAgentWatched(UID agentID, bool isWatched);
		bool GetAgentWatched(UID agentID);

};
