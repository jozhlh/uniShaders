#ifndef _PROCCUBEMESH_H_
#define _PROCCUBEMESH_H_

#include "BaseMesh.h"
#include "ImprovedNoise.h"

using namespace DirectX;

struct NoiseData
{
	XMFLOAT2 Location;
	float Resolution;
	float Height;
	float scaleModifier;
};

class ProcCubeMesh : public BaseMesh
{

public:
	ProcCubeMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int cellX, int cellZ, int xSegments, int zSegments, NoiseData* noise, ImprovedNoise* noiseEngine);
	~ProcCubeMesh();

protected:
	float GetHeight(float xPos, float zPos);
	XMFLOAT3 Cross(XMFLOAT3 a, XMFLOAT3 b);
	XMFLOAT3 Subtract(XMFLOAT3 a, XMFLOAT3 b);
	void InitBuffers(ID3D11Device* device);
	NoiseData* m_Noise;
	ImprovedNoise* perlin;
	int x, z, xSegs, zSegs;
};

#endif