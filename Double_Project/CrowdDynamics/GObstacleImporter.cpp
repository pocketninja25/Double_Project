#include "GObstacleImporter.hpp"

#include "tinyxml.h"
#include "tinystr.h"

GObstacleImporter::GObstacleImporter()
{
	m_LoadedBlueprints.count("Foo");
}


GObstacleImporter::~GObstacleImporter()
{
	for (auto iter = m_LoadedBlueprints.begin(); iter != m_LoadedBlueprints.end(); iter++)
	{
		delete (*iter).second;
	}
}

GObstacleTemplate* GObstacleImporter::LoadBlueprint(std::string fileName)
{
	fileName = "..//CrowdDynamics//" + fileName;
	if (m_LoadedBlueprints.count(fileName))
	{
		return m_LoadedBlueprints.at(fileName);	//Return a pointer to the blueprint
	}

	//Blueprint not already loaded, load it

	TiXmlDocument doc = TiXmlDocument(fileName.c_str());
	bool loadSuccess = doc.LoadFile();

	if (loadSuccess)
	{
		GObstacleTemplate* blueprintBuilder = new GObstacleTemplate();
		TiXmlElement* rootElement = doc.RootElement();

		TiXmlElement* traversalElt = rootElement->FirstChildElement("VertexList");

		traversalElt = traversalElt->FirstChildElement("Vertex");

		while (traversalElt)
		{
			int id;
			CVector2 vertex;

			id = atoi(traversalElt->Attribute("id"));
			vertex.x = static_cast<float>(atof(traversalElt->Attribute("x")));
			vertex.y = static_cast<float>(atof(traversalElt->Attribute("y")));
			
			blueprintBuilder->AddVertex(id, vertex);
			traversalElt = traversalElt->NextSiblingElement("Vertex");
		}

		traversalElt = rootElement->FirstChildElement("VertexPairs");

		traversalElt = traversalElt->FirstChildElement("Pair");

		while (traversalElt)
		{
			GIntPair vertexPair;
						
			vertexPair.x = atoi(traversalElt->Attribute("x"));
			vertexPair.y = atoi(traversalElt->Attribute("y"));

			blueprintBuilder->AddVertexPair(vertexPair);
			traversalElt = traversalElt->NextSiblingElement("Pair");
		}
		
		traversalElt = rootElement->FirstChildElement("Mesh");
		blueprintBuilder->SetMeshFile(traversalElt->Attribute("file"));
		blueprintBuilder->SetMeshScale(static_cast<float>(atof(traversalElt->Attribute("scale"))));

		m_LoadedBlueprints.emplace(std::make_pair(fileName, blueprintBuilder));

		return blueprintBuilder;	//Return a pointer to the blueprint
	}

	return nullptr;	//Fail, return null pointer
}
