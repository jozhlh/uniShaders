#ifndef _REGION_H
#define _REGION_H

#include "Cell.h"
#include "Building.h"
#include <list>
#include "../DXFramework/SphereMesh.h"


//using namespace std;

class Region
{
public:
	Region();
	~Region();

	void Init(ID3D11Device* device, ID3D11DeviceContext *deviceContext, int regionNum);
	void GiveCell(Cell* m_Cell);
	void AssignMajorBuilding(Building* building, float cellSize);
	void PlaceDerrick(Building* derrickModel, float cellSize);
	void DifferentiateCells(float r);
	void CalculateCentre(float cellArea);
	
	//bool operator<(const Region* rightHandSide) const { return this->GetCellCount() > rightHandSide->GetCellCount(); }
	void Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
		SpecularLightShader* shader, Light* light, XMFLOAT3 cameraPosition, Texture* tex, bool tile, XMFLOAT3 tileColour);

	XMFLOAT2 GetNode() { return nodeCoords; }
	void SetNodeCoordinates(float x, float z) { nodeCoords = XMFLOAT2(x, z); }
	int GetCellCount() const { return m_ChildCells.size(); }

private:
	bool CheckOrientation(XMFLOAT3 centre, int xIterator, int zIterator, float x, float z, float cellSize);
	void RenderCentralBuilding(ID3D11DeviceContext * deviceContext, const XMMATRIX & world, const XMMATRIX & view, const XMMATRIX & projection,
		SpecularLightShader * shader, Light * light, XMFLOAT3 cameraPosition, Texture* tex, bool tile, XMFLOAT3 tileColour);

	std::list<Cell*> m_ChildCells;
	XMFLOAT2 nodeCoords;
	XMFLOAT3 centreOfRegion;
	SphereMesh* nodeSphere;
	SphereMesh* centreSphere;
	Building* centralBuilding;
	Building* derrick;
	XMFLOAT3 buildingLocation;
	float buildingRotation;
	float sphereScale;
	float yOff;
	int numOfCells;
	int id;
	bool hasMajorBuilding;
	bool hasDerrick;
};

#endif // !_REGION_H
