// Mesh.h
#ifndef _PROCMESH_H_
#define _PROCMESH_H_

#include <d3d11.h>
#include <directxmath.h>
#include "texture.h"

using namespace DirectX;

struct NoiseData
{
	XMFLOAT2 Location;
	float Resolution;
	float Height;
};

class ProcMesh
{
protected:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	ProcMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int cellX, int cellZ, int xSegments, int zSegments);
	~ProcMesh();

	void SetNoise(NoiseData* noiseData) { m_Noise = noiseData; }
	void SendData(ID3D11DeviceContext*);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

protected:
	bool InitBuffers(ID3D11Device* device);
	void LoadTexture(ID3D11Device*, ID3D11DeviceContext* deviceContext, WCHAR*);
	void CalculateMesh();
	float GetHeight(float xPos, float zPos);

protected:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
	NoiseData* m_Noise;
	int x, z, xSegs, zSegs;
};

#endif