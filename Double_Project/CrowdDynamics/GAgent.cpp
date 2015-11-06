#include "GAgent.h"

GAgent::GAgent(gen::CVector2 iPosition, bool iIsActive) :
	GEntity(iPosition, iIsActive)
{
}

GAgent::GAgent(float iXPos, float iYPos, bool iIsActive) : 
	GAgent(gen::CVector2(iXPos, iYPos), iIsActive)
{
	//Delagated Constructor - No Body
}

GAgent::~GAgent()
{
}

void GAgent::Update(float updateTime)
{
	//TODO: complete this function
}

#ifdef _DEBUG
std::string GAgent::ToString()
{
	std::stringstream builder;

	builder << GEntity::ToString() << "Destination: X: " << m_Destination.x << " Y: " << m_Destination.y << "\n" 
		<< "Mass: " << m_Mass << "\n";

	return builder.str();
}
#endif

void GAgent::GetNewDestination()
{
	//Ask a manager class for a new destination
}