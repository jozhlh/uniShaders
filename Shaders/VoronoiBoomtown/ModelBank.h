#ifndef _MODELBANK_H
#define _MODELBANK_H

#include "Building.h"

#define MAJOR_BUILDINGS 5
#define MINOR_BUILDINGS 7

class ModelBank
{
public:
	ModelBank();
	~ModelBank();

	void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	Building majorBuildings[MAJOR_BUILDINGS];
	Building minorBuildings[MINOR_BUILDINGS];
};

#endif // !_MODELBANK_H
