#ifndef _CELL_H
#define _CELL_H

#include "../DXFramework/CubeMesh.h"
#include "../DXFramework/ProcCubeMesh.h"
#include "SpecularLightShader.h"


class Cell
{
public:
	Cell(ID3D11Device *device, ID3D11DeviceContext *deviceContext, int cellX, int cellZ, int xSegments, int zSegments, NoiseData* noise, float size, ImprovedNoise* noiseEngine);
	~Cell();

	bool CellContainsPoint(XMFLOAT3 point);
	void SetOccupied(bool occupy) { occupied = occupy; }
	bool IsOccupied() { return occupied; }
	void SetTexture(string filename) {}
	void SetCoordinates(float x, float y, float z) { position = XMFLOAT3(x, y, z); }
	XMFLOAT3 GetCoordinates() { return position; }
	float GetScale() { return scale; }
	void SetParent(int regionID) { parentRegion = regionID; }
	void Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
		SpecularLightShader* shader, Light* light, XMFLOAT3 cameraPosition, Texture* tex, bool tile, XMFLOAT3 tileColour);
	bool altTexture;

private:
	float scale;
	XMFLOAT3 position;
	ProcCubeMesh* m_SurfaceModel;
	ProcCubeMesh* m_AltSurfaceModel;
	//CubeMesh* fallbackMesh;
	int parentRegion;
	bool occupied;
	
};

#endif // !_CELL_H
