// VoronoiBoomtown - VoronoiMap.h
// References: https://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful
// https://view.officeapps.live.com/op/view.aspx?src=http%3a%2f%2fvideo.ch9.ms%2fsessions%2fgonat%2f2013%2fSTLGN13rand.pptx
#ifndef _VORONOIMAP_H
#define _VORONOIMAP_H

#include "Region.h"

using namespace std;

class VoronoiMap
{
public:
	VoronoiMap(ID3D11Device *device, ID3D11DeviceContext *deviceContext);
	~VoronoiMap();

	void Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
		SpecularLightShader* shader, Light* light, XMFLOAT3 cameraPosition);

	list<Region*> regions;
	list<Cell*> unassignedCells;
	CubeMesh* m_CellModelTemplate1;
	CubeMesh* m_CellModelTemplate2;
	int numOfRegions;
	float cellSize;
	float yPos;
	int xCells;
	int zCells;

	void GenerateRegions(int num);
	void SortRegionsBySize();
	void AssignCellsToRegions();
	float RegionDistance(Cell* cell, Region* region);
};

#endif