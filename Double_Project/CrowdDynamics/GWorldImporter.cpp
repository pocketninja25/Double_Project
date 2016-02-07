#include "GWorldImporter.hpp"

#include "tinyxml.h"
#include "tinystr.h"

GWorldImporter::GWorldImporter()
{

}


GWorldImporter::~GWorldImporter()
{
	for (auto iter = m_LoadedBlueprints.begin(); iter != m_LoadedBlueprints.end(); iter++)
	{
		delete (*iter).second;
	}
}

SWorldBlueprint GWorldImporter::LoadBlueprint(std::string fileName)
{
	fileName = "..//CrowdDynamics//" + fileName;
	if (m_LoadedBlueprints.count(fileName))
	{
		return SWorldBlueprint(*m_LoadedBlueprints.at(fileName));	//Return a copy of the blueprint
	}

	//Blueprint not already loaded, load it

	TiXmlDocument doc = TiXmlDocument(fileName.c_str());
	bool loadSuccess = doc.LoadFile();

	if (loadSuccess)
	{
		SWorldBlueprint* blueprintBuilder = new SWorldBlueprint();
		TiXmlElement* rootElement = doc.RootElement();
		
		TiXmlElement* traversalElt = rootElement->FirstChildElement("TimeStep");
		blueprintBuilder->TimeStep = 1 / static_cast<float>(atof(traversalElt->Attribute("ticks")));	//Accepts a tick rate, 1/tickrate = TimeStep
		
		traversalElt = rootElement->FirstChildElement("WorldSize");
		blueprintBuilder->WorldSize.x = static_cast<float>(atof(traversalElt->Attribute("x")));
		blueprintBuilder->WorldSize.y = static_cast<float>(atof(traversalElt->Attribute("y")));

		traversalElt = rootElement->FirstChildElement("Subdivisions");
		blueprintBuilder->subdivisions.x = atoi(traversalElt->Attribute("x"));
		blueprintBuilder->subdivisions.y = atoi(traversalElt->Attribute("y"));
		
		traversalElt = rootElement->FirstChildElement("influencePerUnit");
		blueprintBuilder->influenceSquaresPerUnit = static_cast<float>(atof(traversalElt->Attribute("value")));

		traversalElt = rootElement->FirstChildElement("Agents");
		while (traversalElt)
		{
			int noAgents;
			std::string agentBlueprintFile;

			noAgents = atoi(traversalElt->Attribute("number"));
			agentBlueprintFile = traversalElt->Attribute("blueprint");
			
			blueprintBuilder->agentDetails.emplace(std::make_pair(agentBlueprintFile, noAgents));
			traversalElt = traversalElt->NextSiblingElement("Agents");
		}
		
		//Have the influence map represent 1 square = 1 unit, any incomplete integer in world size is clamped into the nearest square (e.g. world size x = 3.2 so have 3 squares for x, item at position 3.1 clamps to square 3)
		blueprintBuilder->influenceSubdivisions.x = static_cast<int>(blueprintBuilder->WorldSize.x * blueprintBuilder->influenceSquaresPerUnit);
		blueprintBuilder->influenceSubdivisions.y = static_cast<int>(blueprintBuilder->WorldSize.y * blueprintBuilder->influenceSquaresPerUnit);


		m_LoadedBlueprints.emplace(std::make_pair(fileName, blueprintBuilder));
		return SWorldBlueprint(*blueprintBuilder);	//Return a copy of the blueprint
	}

	return SWorldBlueprint();	//Fail, return a blank blueprint
}
