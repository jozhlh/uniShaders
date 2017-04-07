#ifndef _REGION_H
#define _REGION_H

#include "Cell.h"
#include <list>
#include "../DXFramework/SphereMesh.h"
//using namespace std;

class Region
{
public:
	Region();
	~Region();

	void Init(ID3D11Device* device, ID3D11DeviceContext *deviceContext);
	void SetNodeCoordinates(float x, float z) { nodeCoords = XMFLOAT2(x, z); }
	XMFLOAT2 GetNode() { return nodeCoords; }
	void GiveCell(Cell* m_Cell) { m_ChildCells.push_back(m_Cell); numOfCells++; }
	void DifferentiateCells(float r);
	void CalculateCentre(float cellArea);
	int GetCellCount() const { return m_ChildCells.size(); }
	//bool operator<(const Region* rightHandSide) const { return this->GetCellCount() > rightHandSide->GetCellCount(); }
	void Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
		SpecularLightShader* shader, Light* light, XMFLOAT3 cameraPosition);

private:
	std::list<Cell*> m_ChildCells;
	XMFLOAT2 nodeCoords;
	XMFLOAT3 centreOfRegion;
	SphereMesh* nodeSphere;
	SphereMesh* centreSphere;
	float sphereScale;
	float yOff;
	int numOfCells;
};

#endif // !_REGION_H
