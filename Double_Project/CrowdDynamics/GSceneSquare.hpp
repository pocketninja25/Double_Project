#pragma once

#include "Common.hpp"
#include "GObject.hpp"


class GSceneSquare : public GObject
{
//---------------------------
// Private Data Members
//---------------------------

private:
	gen::CVector2 m_Size;		//Width * Height of the Scene Square
	gen::CVector2 m_Origin;		//The bottom left position of the square
	std::list<UID> m_Walls;		//TODO: Consider static wall list & dynamic wall list //Vector of UID's to wall objects that occupy/intersect this scene square : This class does not own these pointers
	std::list<UID> m_Agents;	//TODO: have the SceneSquare care about if agent is active

//---------------------------
// Public Functions
//---------------------------
public:
	//***************************
	// Constructors/Destructors
	//***************************
	GSceneSquare(gen::CVector2 iSize, gen::CVector2 iOriginPos, bool iIsActive = true);
	GSceneSquare(gen::CVector2 iSize, float iOriginXPos, float iOriginYPos, bool iIsActive = true);
	GSceneSquare(float iXSize, float iYSize, gen::CVector2 iOriginPos, bool iIsActive = true);
	GSceneSquare(float iXSize, float iYSize, float iOriginXPos, float iOriginYPos, bool iIsActive = true);

	virtual ~GSceneSquare();

	//***************************
	// Getters/Accessors
	//***************************
	std::vector<UID> TransferAgents();	//Return a list of agent UID's who have moved out of this scene square since the last time this function was called //Assumedly this function is called each time step

	//***************************
	// Setters/Mutators
	//***************************
	void AddAgent(UID agentID);
	void RemoveAgent(UID agentID);

	//***************************
	// Other Functions
	//***************************
	virtual void Update(float updateTime);

//^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Debug code
//vvvvvvvvvvvvvvvvvvvvvvvvvvv
#ifdef _DEBUG
	virtual std::string ToString();

#endif
};

