#pragma once
#include "Common.hpp"
#include "GObstacleTemplate.hpp"

class GObstacleImporter
{
private:
	std::map<std::string, GObstacleTemplate*> m_LoadedBlueprints;

public:
	GObstacleImporter();
	~GObstacleImporter();

	GObstacleTemplate* LoadBlueprint(std::string fileName);	//Loads the blueprint (if not already loaded), then returns a copy of the blueprint (caching a copy for speedy loading later)

};

