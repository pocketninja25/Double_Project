#include "GAgentImporter.h"

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

bool stobool(std::string theString)
{
	if (theString == "true")
	{
		return true;
	}
	return false;
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

		blueprintBuilder->defaultTurningArc = gen::ToRadians(atof(blueprintElement->Attribute("turningArc")));	//Accept degrees, convert to radians
		blueprintBuilder->maxGradientTraversal = atof(blueprintElement->Attribute("maxGradient"));
		blueprintBuilder->radius = atof(blueprintElement->Attribute("radius"));
		blueprintBuilder->velocity = atof(blueprintElement->Attribute("velocity"));
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
			blueprintBuilder->destination.x = atof(dest->Attribute("x"));
			blueprintBuilder->destination.y = atof(dest->Attribute("y"));
		}
		if (blueprintBuilder->randomPosition)
		{
			blueprintBuilder->position = CVector2(0.0f, 0.0f);
		}
		else
		{
			TiXmlElement* pos = blueprintElement->FirstChildElement("Position");
			blueprintBuilder->destination.x = atof(pos->Attribute("x"));
			blueprintBuilder->destination.y = atof(pos->Attribute("y"));
		}
		m_LoadedBlueprints.emplace(std::make_pair( fileName, blueprintBuilder));
		return SAgentBlueprint(*blueprintBuilder);	//Return a copy of the blueprint
	}

	return SAgentBlueprint();
}
