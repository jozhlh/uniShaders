#include "ModelBank.h"

void ModelBank::Init(ID3D11Device *device, ID3D11DeviceContext *deviceContext)
{
	for (int i = 0; i < MAJOR_BUILDINGS; i++)
	{
		majorBuildings[i].Init(new Model(device, deviceContext, L"../res/testObject0.png", L"../res/testObject0.obj"), 3, 3, i);
	}

	for (int i = 0; i < MINOR_BUILDINGS; i++)
	{
		minorBuildings[i].Init(new Model(device, deviceContext, L"../res/testObject0.png", L"../res/testObject0.obj"), 1, 1, i);
	}
}
