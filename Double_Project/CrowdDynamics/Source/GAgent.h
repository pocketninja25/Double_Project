#pragma once
#include "GEntity.h"

#include "CVector3.h"
using gen::CVector3;

class CGAgent :
	public CGEntity
{
public:
	CGAgent();
	virtual ~CGAgent();

private:
	CVector3 destination;	//The position on the map that the agent is moving towards
	float mass;
	
};


