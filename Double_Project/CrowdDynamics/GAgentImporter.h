#pragma once
#include "GAgent.hpp"
#include <map>

class GAgentImporter
{
private:
	std::map<std::string, SAgentBlueprint*> m_LoadedBlueprints;

public:
	GAgentImporter();
	~GAgentImporter();

	SAgentBlueprint LoadBlueprint(std::string fileName);	//Loads the blueprint (if not already loaded), then returns a copy of the blueprint (caching a copy for speedy loading later)
};

