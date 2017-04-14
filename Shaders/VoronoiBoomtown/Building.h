#ifndef _BUILDING_H
#define _BUILDING_H

#include "../DXFramework/Model.h"

class Building
{
public:
	Building();
	~Building();

	void Init(Model* model, float width, float length, int importance);

	int priority;
	Model* asset;
	XMFLOAT2 dimensions;
	XMFLOAT2 offset; // for putting the origin at the centre of a cell
};

#endif