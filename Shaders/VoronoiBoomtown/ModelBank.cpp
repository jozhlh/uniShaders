#include "ModelBank.h"

ModelBank::ModelBank()
{
}

ModelBank::~ModelBank()
{
}

void ModelBank::Init(ID3D11Device *device, ID3D11DeviceContext *deviceContext)
{
	/*for (int i = 0; i < MAJOR_BUILDINGS; i++)
	{
		majorBuildings[i].Init(new Model(device, deviceContext, L"../res/testObject0.png", L"../res/testObject0.obj"), 9, 3, 3, 0.05f, i);
	}*/
	majorBuildings[0].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomChurch.obj"), 3, 4, 4, 0.05f, 0);
	majorBuildings[1].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, 0.05f, 1);
	majorBuildings[2].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSaloon.obj"), 7, 3, 5, 0.05f, 2);
	majorBuildings[3].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomGeneralStore.obj"), 7, 3, 5, 0.05f, 3);
	majorBuildings[4].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomHotel.obj"), 5, 6, 5, 0.05f, 4);
	majorBuildings[5].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, 0.05f, 1);
	majorBuildings[6].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, 0.05f, 1);
	majorBuildings[7].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, 0.05f, 1);
	majorBuildings[8].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, 0.05f, 1);
	majorBuildings[9].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, 0.05f, 1);
	majorBuildings[10].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomSilo.obj"), 8, 5, 5, 0.05f, 1);

	/*for (int i = 0; i < MINOR_BUILDINGS; i++)
	{
		minorBuildings[i].Init(new Model(device, deviceContext, L"../res/testObject0.png", L"../res/testObject0.obj"), 3, 3, 3, 0.05f, i);
	}*/
	minorBuildings[0].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomDerrick.obj"), 3, 7, 3, 0.05f, 1);
	minorBuildings[1].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomBarrel.obj"), 1, 1, 1, 0.05f, 2);
	minorBuildings[2].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomCactus.obj"), 1, 2, 1, 0.05f, 3);
	minorBuildings[3].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomCactus2.obj"), 1, 2, 1, 0.05f, 4);
	minorBuildings[4].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomCart.obj"), 2, 1, 1, 0.05f, 5);
	minorBuildings[5].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomDonkey.obj"), 3, 4, 1, 0.05f, 6);
	minorBuildings[6].Init(new Model(device, deviceContext, L"../res/palette.png", L"../res/boomTent.obj"), 1, 1, 1, 0.05f, 7);
}
