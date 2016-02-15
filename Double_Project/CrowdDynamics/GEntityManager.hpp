#pragma once

#include "Common.hpp"

//Forward declare these classes, include the files for the cpp only (where needed)
class GEntity;
class GSceneManager;
class GAgent;
struct SAgentBlueprint;
class GAgentImporter;

class GEntityManager
{
//---------------------------
// Manager classes
//---------------------------
private:

//---------------------------
// Private Data Members
//---------------------------
private:
	std::map<UID, GAgent*> m_ActiveAgents;
	std::map<UID, GAgent*> m_InactiveAgents;

	GAgentImporter* m_AgentBlueprintLoader;

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
	std::map<std::string, float> GetAgentMeshes();
	bool GetAgent(UID request, GAgent* &returnedAgent);
	std::vector<UID> GetAgentUIDs();

	bool GetAgentMeshScale(std::string requestedMesh, float &scale);

	//***************************
	// Setters/Mutators
	//***************************
	UID AddAgent(CVector2 iPosition, bool iIsActive);	//Deprecated
	UID AddAgent(float iXPos, float iYPos, bool iIsActive);	//Deprecated
	UID AddAgent(std::string blueprintFile, bool overwriteStartPosition = false, CVector2 newStartPosition = CVector2(0.0f, 0.0f));

	bool SetAgentActivation(UID agent, bool isEnabled);

	//***************************
	// Other Functions
	//***************************

	void Update(float updateTime);

	void PerformCollisionAvoidance(const std::list<UID>& localAgents);	//Performs collision avoidance algorithms on the provided list of agents (each agent in the list checks collision with each other agent in the list)

	CVector2 GetRandomDestination();		//Return a random position in the game world TODO: Make sure that position is not within an obstacle


//^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Debug code
//vvvvvvvvvvvvvvvvvvvvvvvvvvv
public:
	bool SetAgentWatched(UID agentID, bool isWatched);
	bool GetAgentWatched(UID agentID);


};

