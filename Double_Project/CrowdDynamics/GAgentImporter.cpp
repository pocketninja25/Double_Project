#include "GAgentImporter.hpp"

#include "tinyxml.h"
#include "tinystr.h"

GAgentImporter::GAgentImporter()
{
	
}


GAgentImporter::~GAgentImporter()
{
	for (auto iter = m_LoadedBlueprints.begin(); iter != m_LoadedBlueprints.end(); iter++)
	{
		delete (*iter).second;
	}
}

SAgentBlueprint GAgentImporter::LoadBlueprint(std::string fileName)
{
	fileName = "..//CrowdDynamics//" + fileName;
	if (m_LoadedBlueprints.count(fileName))
	{
		return SAgentBlueprint(*m_LoadedBlueprints.at(fileName));	//Return a copy of the blueprint
	}
	
	//Blueprint not already loaded, load it

	TiXmlDocument doc = TiXmlDocument(fileName.c_str());
	bool loadSuccess = doc.LoadFile();

	if (loadSuccess)
	{
		SAgentBlueprint* blueprintBuilder = new SAgentBlueprint();
		TiXmlElement* blueprintElement = doc.RootElement();

		blueprintBuilder->defaultTurningArc = gen::ToRadians(static_cast<float>(atof(blueprintElement->Attribute("turningArc"))));	//Accept degrees, convert to radians
		blueprintBuilder->maxGradientTraversal = static_cast<float>(atof(blueprintElement->Attribute("maxGradient")));
		blueprintBuilder->radius = static_cast<float>(atof(blueprintElement->Attribute("radius")));
		blueprintBuilder->velocity = static_cast<float>(atof(blueprintElement->Attribute("velocity")));
		blueprintBuilder->randomPosition = stobool(blueprintElement->Attribute("randomPosition"));
		blueprintBuilder->randomDestination = stobool(blueprintElement->Attribute("randomDestination"));
		blueprintBuilder->startsActive = stobool(blueprintElement->Attribute("startsActive"));

		if (blueprintBuilder->randomDestination)
		{
			blueprintBuilder->destination = CVector2(0.0f, 0.0f);
		}
		else
		{
			TiXmlElement* dest = blueprintElement->FirstChildElement("Destination");
			blueprintBuilder->destination.x = static_cast<float>(atof(dest->Attribute("x")));
			blueprintBuilder->destination.y = static_cast<float>(atof(dest->Attribute("y")));
		}
		if (blueprintBuilder->randomPosition)
		{
			blueprintBuilder->position = CVector2(0.0f, 0.0f);
		}
		else
		{
			TiXmlElement* pos = blueprintElement->FirstChildElement("Position");
			blueprintBuilder->destination.x = static_cast<float>(atof(pos->Attribute("x")));
			blueprintBuilder->destination.y = static_cast<float>(atof(pos->Attribute("y")));
		}
		m_LoadedBlueprints.emplace(make_pair(fileName, blueprintBuilder));
		return SAgentBlueprint(*blueprintBuilder);	//Return a copy of the blueprint
	}

	return SAgentBlueprint();	//Fail, return a blank blueprint
}
