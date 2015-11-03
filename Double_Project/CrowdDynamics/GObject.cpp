#include "GObject.h"

//-----------------------
// Static Members
//-----------------------


static UID s_NextUID = 0;

static UID GetNewUID()			//Returns the next free UID and increments the nextUID counter
{
	s_NextUID++;
	return (s_NextUID - 1);
}

//-----------------------
// End of Static Members
//-----------------------

GObject::GObject(bool iIsActive)
{
	m_UID = GetNewUID();

	m_IsActive = iIsActive;
}

GObject::~GObject()
{
}

UID GObject::GetUID()
{
	return m_UID;
}

bool GObject::IsActive()
{
	return m_IsActive;
}

void GObject::Activate()
{
	m_IsActive = true;
}

void GObject::Deactivate()
{
	m_IsActive = false;
}


#ifdef _DEBUG
string GObject::ToString()		//Return the state of this Object as a string (Debug tool)
{
	std::stringstream builder;

	builder << "ObjectID: " << m_UID << "\n" << "Active: " << m_IsActive << "\n";

	return builder.str();
}
#endif