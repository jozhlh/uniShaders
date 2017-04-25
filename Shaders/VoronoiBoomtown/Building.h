// VoronoiBoomtown - Building.h
// Josh Hale - 2017
// A container for models which are placed on the grid

#ifndef _BUILDING_H
#define _BUILDING_H

#include "../DXFramework/Model.h"

class Building
{
public:
	Building();
	~Building();

	void Init(Model* model, float width, float height, float length, float size, int importance);

	int priority;
	float scale;
	float rotation;
	Model* asset;
	XMFLOAT3 dimensions;
	XMFLOAT3 location;
};

#endif