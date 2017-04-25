// Voronoi Boomtown - VoronoiMap.h
// Josh Hale - 2017
// Creates a grid, split into regions which are sorted and have models placed in their centres

#ifndef _VORONOIMAP_H
#define _VORONOIMAP_H

#include "Region.h"
#include "ModelBank.h"
#include "../DXFramework/CubeMesh.h"

using namespace std;

class VoronoiMap
{
public:
	VoronoiMap(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ModelBank* bank, 
		XMINT2 dimensions, float border, float noiseHeight, float noiseResolution, float size, float numberOfRegions, XMFLOAT3 baseColour);
	~VoronoiMap();

	void Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
		SpecularLightShader* shader, SpecularTerrainShader* terrainShader, Light* light, XMFLOAT3 cameraPosition, XMFLOAT3 nodeColour, XMFLOAT3 centreColour, XMFLOAT3 regionColour,
		bool showNodes, bool identifyRegions, XMFLOAT3 baseColour, float yOff);

private:
	// Calculation Methods
	void GenerateRegions(int num);
	void SortRegionsBySize();
	void AssignCellsToRegions(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
	float RegionDistance(Cell* cell, Region* region);

	list<Region*> regions;
	list<Cell*> unassignedCells;
	ModelBank* modelBank;
	ImprovedNoise* noiseEngine;
	Texture* stockTexture;
	CubeMesh* basePlate;
	NoiseData noise;
	XMFLOAT3 basePlateColour;
	int numOfRegions;
	float cellSize;
	float xWidth;
	float zWidth;
	float yPos;
	int xCells;
	int zCells;
	float cellBorder;


};

#endif