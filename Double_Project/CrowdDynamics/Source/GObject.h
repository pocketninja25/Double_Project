#pragma once

class CGObject
{
public:
	CGObject();
	virtual ~CGObject();

	//Update function
	virtual void Update();

	int GetUID()
	{
		return UID;
	}

private:
	int UID;
};

