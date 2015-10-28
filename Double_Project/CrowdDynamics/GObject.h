#pragma once
class GObject
{
public:
	GObject();
	virtual ~GObject();
private:
	int UID;
public:
	virtual void Update(float timeStep);
	int GetUID();
};

