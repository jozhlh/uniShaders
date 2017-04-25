// VoronoiBoomtown - Cell.h
// Josh Hale - 2017
// A container for the cell which holds its model, region owner and occupation status

#ifndef _CELL_H
#define _CELL_H

#include "../DXFramework/ProcCubeMesh.h"
#include "SpecularTerrainShader.h"

class Cell
{
public:
	Cell(ID3D11Device *device, ID3D11DeviceContext *deviceContext, int cellX, int cellZ, int xSegments, int zSegments, NoiseData* noise, float size, float border, ImprovedNoise* noiseEngine);
	~Cell();

	// Getters and Setters
	void SetOccupied(bool occupy) { occupied = occupy; }
	bool IsOccupied() { return occupied; }
	void SetCoordinates(float x, float y, float z) { position = XMFLOAT3(x, y, z); }
	XMFLOAT3 GetCoordinates() { return position; }
	float GetScale() { return scale; }
	void SetParent(int regionID) { parentRegion = regionID; }

	// Methods
	bool CellContainsPoint(XMFLOAT3 point);
	void Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
		SpecularTerrainShader* shader, Light* light, XMFLOAT3 cameraPosition, Texture* tex, XMFLOAT3 terrainBaseColor, bool identify, int numOfRegions);

private:
	float scale;
	XMFLOAT3 position;
	ProcCubeMesh* m_SurfaceModel;
	int parentRegion;
	bool occupied;
	
};

#endif // !_CELL_H
