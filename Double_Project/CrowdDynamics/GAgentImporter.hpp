#pragma once
#include "Common.hpp"
#include "GObstacleTemplate.hpp"

class GAgentImporter
{
private:
	std::map<std::string, SAgentBlueprint*> m_LoadedBlueprints;

public:
	GAgentImporter();
	~GAgentImporter();

	GObstacleTemplate LoadBlueprint(std::string fileName);	//Loads the blueprint (if not already loaded), then returns a copy of the blueprint (caching a copy for speedy loading later)
};

