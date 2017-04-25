// ProcCubeMesh.h
// Josh Hale - 2017
// Generates cuboids with the top surface aligned with noise, for noise on a grid

#ifndef _PROCCUBEMESH_H_
#define _PROCCUBEMESH_H_

#include "BaseMesh.h"
#include "ImprovedNoise.h"

using namespace DirectX;

// Noise parameters for passing to noise engine
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
	ProcCubeMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int cellX, int cellZ, int xSegments, int zSegments, float gap, NoiseData* noise, ImprovedNoise* noiseEngine);
	~ProcCubeMesh();

protected:
	// Initialization Methods
	void InitBuffers(ID3D11Device* device);
	float GetHeight(float xPos, float zPos);

	// Vector arithmetic on float classes
	XMFLOAT3 Cross(XMFLOAT3 a, XMFLOAT3 b);
	XMFLOAT3 Subtract(XMFLOAT3 a, XMFLOAT3 b);
	
	// Noise pointers
	NoiseData* m_Noise;
	ImprovedNoise* perlin;

	// Calculation variables
	int x, z, xSegs, zSegs;
	float border;
};

#endif