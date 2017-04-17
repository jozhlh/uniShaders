#include "ModelBank.h"

ModelBank::ModelBank()
{
}

ModelBank::~ModelBank()
{
}

void ModelBank::Init(ID3D11Device *device, ID3D11DeviceContext *deviceContext)
{
	for (int i = 0; i < MAJOR_BUILDINGS; i++)
	{
		majorBuildings[i].Init(new Model(device, deviceContext, L"../res/testObject0.png", L"../res/testObject0.obj"), 9, 3, 3, 0.05f, i);
	}

	for (int i = 0; i < MINOR_BUILDINGS; i++)
	{
		minorBuildings[i].Init(new Model(device, deviceContext, L"../res/testObject0.png", L"../res/testObject0.obj"), 3, 3, 3, 0.05f, i);
	}
}
