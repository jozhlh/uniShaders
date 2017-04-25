// Voronoi Boomtown - Region.h
// Josh Hale - 2017
// Contains a node and pointers to its cells and model data

#ifndef _REGION_H
#define _REGION_H

#include "Cell.h"
#include "Building.h"
#include "SpecularLightShader.h"
#include <list>
#include <cctype>
#include <random>
#include "../DXFramework/SphereMesh.h"

class Region
{
public:
	Region();
	~Region();

	// Initialization methods
	void Init(ID3D11Device* device, ID3D11DeviceContext *deviceContext, int regionNum);
	void GiveCell(Cell* m_Cell);
	void AssignMajorBuilding(Building* building, float cellSize, int zCells);
	void PlaceDerrick(Building* derrickModel, float cellSize);
	void PlaceMinorAsset(Building* building, float cellSize);
	void CalculateCentre(float cellArea);
	
	void Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
		SpecularLightShader* shader, Light* light, XMFLOAT3 cameraPosition, Texture* tex, XMFLOAT3 centreColour, XMFLOAT3 nodeColour,
		bool showNodes, float yOff);

	// Getters and Setters
	XMFLOAT2 GetNode() { return nodeCoords; }
	void SetNodeCoordinates(float x, float z) { nodeCoords = XMFLOAT2(x, z); }
	int GetCellCount() const { return m_ChildCells.size(); }
	void SetCellIDs(int order);

private:
	bool CheckBuildingPlacement(Building* building, XMFLOAT3 centralCell, float cellSize);
	vector<XMFLOAT3> GetBuildingCoordinates(float xDimension, float zDimension, XMFLOAT3 centralCell, float cellSize);
	void RenderCentralBuilding(ID3D11DeviceContext * deviceContext, const XMMATRIX & world, const XMMATRIX & view, const XMMATRIX & projection,
		SpecularLightShader * shader, Light * light, XMFLOAT3 cameraPosition, Texture* tex, bool tile, XMFLOAT3 tileColour);

	std::list<Cell*> m_ChildCells;
	vector<Cell*> m_OccupiedCells;
	std::list<Building> m_MinorAssets;
	SphereMesh* nodeSphere;
	SphereMesh* centreSphere;
	Building* centralBuilding;
	Building derrick;
	XMFLOAT2 nodeCoords;
	XMFLOAT3 centreOfRegion;
	XMFLOAT3 buildingLocation;
	float buildingRotation;
	float sphereScale;
	float buildingOffset;
	int numOfCells;
	int id;
	bool hasMajorBuilding;
	bool hasDerrick;
};

#endif // !_REGION_H
