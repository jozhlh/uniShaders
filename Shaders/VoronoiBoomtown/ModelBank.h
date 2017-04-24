#ifndef _MODELBANK_H
#define _MODELBANK_H

#include "Building.h"
#include <vector>

#define MAJOR_BUILDINGS 11
#define MINOR_BUILDINGS 7

using std::vector;

class ModelBank
{
public:
	ModelBank();
	~ModelBank();

	void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int maxRegions);

	//vector<Building> majorbuildings;
	Building majorBuildings[MAJOR_BUILDINGS];
	Building minorBuildings[MINOR_BUILDINGS];
};

#endif // !_MODELBANK_H
