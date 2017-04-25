// Voronoi Boomtown - ModelBank.cpp
// Josh Hale - 2017
// A container for model and texture data required by the application

#include "ModelBank.h"

ModelBank::ModelBank()
{
}

ModelBank::~ModelBank()
{
}

// Load models and textures from file
void ModelBank::Init(ID3D11Device *device, ID3D11DeviceContext *deviceContext)
{

	float modelSize = 0.1f;
	majorBuildings[0].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomChurch.obj"), 3, 4, 4, modelSize, 0);
	majorBuildings[1].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, modelSize, 1);
	majorBuildings[2].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSaloon.obj"), 7, 3, 5, modelSize, 2);
	majorBuildings[3].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomGeneralStore.obj"), 7, 3, 5, modelSize, 3);
	majorBuildings[4].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomHotel.obj"), 5, 6, 5, modelSize, 4);
	majorBuildings[5].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, modelSize, 1);
	majorBuildings[6].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, modelSize, 1);
	majorBuildings[7].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, modelSize, 1);
	majorBuildings[8].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, modelSize, 1);
	majorBuildings[9].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, modelSize, 1);
	majorBuildings[10].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, modelSize, 1);
	
	minorBuildings[0].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomDerrick.obj"), 3, 7, 3, modelSize, 1);
	minorBuildings[1].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomBarrel.obj"), 1, 1, 1, modelSize, 2);
	minorBuildings[2].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomCactus.obj"), 1, 2, 1, modelSize, 3);
	minorBuildings[3].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomCactus2.obj"), 1, 2, 1, modelSize, 4);
	minorBuildings[4].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomCart.obj"), 3, 1, 1, modelSize, 5);
	minorBuildings[5].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomDonkey.obj"), 3, 4, 1, modelSize, 6);
	minorBuildings[6].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomTent.obj"), 1, 1, 1, modelSize, 7);
}
