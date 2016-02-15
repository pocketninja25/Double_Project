#include "Common.hpp"

//Forward declarations of obstacle classes
class GStaticObstacle;
class GDynamicObstacle;
class GObstacleTemplate;
class GSceneManager;
class GObstacleImporter;

class GObstacleManager
{
//---------------------------
// Private Data Members
//---------------------------
private:
	GObstacleImporter* m_ObstacleBlueprintLoader;

	//Sorted into two categories for update tree management - hopefully wont need to update static obstacles each timestep
	std::vector<GStaticObstacle*> m_StaticObstacles;
	std::vector<GDynamicObstacle*> m_DynamicObstacles;

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************
	GObstacleManager();

	virtual ~GObstacleManager();

	//***************************
	// Getters/Accessors
	//***************************
	bool GetObstacleMesh(UID requestedID, std::string& meshFile);
	bool GetObstacleMatrix(UID requestedID, CMatrix3x3& matrix);

	std::map<std::string, float> GetObstacleMeshes();

	std::vector<UID> GetObstacleUIDs();

	//***************************
	// Setters/Mutators
	//***************************

	// TODO: Might be worth having these two functions private and use an AddObstacle function that accepts an enum (static/dynamic) - depends on what these constructions look like
	UID AddStaticObstacle(std::string obstacleBlueprintFile, CVector2 position );
	//void AddDynamicObstacle(/*SomeObstacleDetails*/);

	bool PositionBlocked(CVector2 position);

	//***************************
	// Other Functions
	//***************************
	void Update(float updateTime);

};

