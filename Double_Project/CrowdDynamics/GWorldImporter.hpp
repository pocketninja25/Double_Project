#pragma once
#include "Common.hpp"

class GWorldImporter
{
private:
	std::map<std::string, SWorldBlueprint*> m_LoadedBlueprints;

public:
	GWorldImporter();
	~GWorldImporter();

	SWorldBlueprint LoadBlueprint(std::string fileName);	//Loads the blueprint (if not already loaded), then returns a copy of the blueprint (caching a copy for speedy loading later)
};
