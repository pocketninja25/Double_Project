#pragma once

#include <vector>
using std::vector;

class GEntity;

class CGEntityManager
{
public:
	CGEntityManager();
	virtual ~CGEntityManager();

private:
	vector<GEntity*> entities;

	void AddAgent(/*Agent details*/);
	void AddDynamicObstacle(/*DynamicObstacle details*/);
	void AddStaticObstacle(/*StaticObstacle details*/);
};
