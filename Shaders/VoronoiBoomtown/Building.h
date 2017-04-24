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
	XMFLOAT3 location;
	float rotation;
	Model* asset;
	XMFLOAT3 dimensions;
	XMFLOAT2 offset; // for putting the origin at the centre of a cell
};

#endif