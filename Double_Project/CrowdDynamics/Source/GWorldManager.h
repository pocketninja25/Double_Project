#pragma once

#include "CVector2.h"
using gen::CVector2;

class CGWorldManager
{
public:
	CGWorldManager(CVector2 worldSize, int xSubdivisions, int ySubdivisions);
	virtual ~CGWorldManager();


};
