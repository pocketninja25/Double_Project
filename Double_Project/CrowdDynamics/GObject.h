#pragma once

#include "Common.h"

#include <string>
#include <sstream>

class GObject
{

//---------------------------
// Static Data Members
//---------------------------
private:
	static UID s_NextUID;

//---------------------------
// Static Functions
//---------------------------
private:
	static UID GetNewUID();

//---------------------------
// Private Data Members
//---------------------------
private:
	UID m_UID;			//This objects UID (Unique IDentifier)
	bool m_IsActive;	//Whether or not the object is active, this will influence whether it is included in the update each timestep

//---------------------------
// Public Functions
//---------------------------
public:

	//***************************
	// Constructors/Destructors
	//***************************
	GObject(bool iIsActive = true);

	virtual ~GObject();

	//***************************
	// Getters/Accessors
	//***************************

	UID GetUID();
	bool IsActive();

	//***************************
	// Setters/Mutators
	//***************************

	void Activate();
	void Deactivate();

	//***************************
	// Other Functions
	//***************************

	virtual void Update(float updateTime) = 0;


#ifdef _DEBUG
	virtual std::string ToString();	//Outputs a string version of the information about the object for debugging and possibly file saving purposes
#endif

};

