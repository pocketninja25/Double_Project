#pragma once

typedef int UID;

#include <string>
using std::string;

class GObject
{

//---------------------------
// Private Data Members
//---------------------------
private:

	UID m_UID;
	bool m_IsActive;

//---------------------------
// Public Functions
//---------------------------
public:

	//***************************
	// Constructors/Destructors
	//***************************
	GObject();

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
	virtual string ToString();	//Outputs a string version of the information about the object for debugging and possibly file saving purposes
	

};

