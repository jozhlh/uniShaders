// VoronoiBoomtown - VoronoiMap.h
// References: https://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful
// https://view.officeapps.live.com/op/view.aspx?src=http%3a%2f%2fvideo.ch9.ms%2fsessions%2fgonat%2f2013%2fSTLGN13rand.pptx
#ifndef _VORONOIMAP_H
#define _VORONOIMAP_H

#include "Region.h"
#include "ModelBank.h"
#include <cctype>
#include <random>

using namespace std;

class VoronoiMap
{
public:
	VoronoiMap(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
	~VoronoiMap();

	void Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
		SpecularLightShader* shader, Light* light, XMFLOAT3 cameraPosition);

private:
	void GenerateRegions(int num);
	//bool CompareRegionSize(const Region* first, const Region* second);
	void SortRegionsBySize();
	void AssignCellsToRegions(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
	float RegionDistance(Cell* cell, Region* region);

	list<Region*> regions;
	list<Cell*> unassignedCells;
	//ProcCubeMesh* m_CellModelTemplate1;
	//ProcCubeMesh* m_CellModelTemplate2;
	ModelBank* modelBank;
	ImprovedNoise* noiseEngine;
	Texture* stockTexture;
	NoiseData noise;
	XMFLOAT3 desertColour;
	int numOfRegions;
	float cellSize;
	float mapSize;
	float yPos;
	int xCells;
	int zCells;


};

#endif