#ifndef _CELL_H
#define _CELL_H

#include "../DXFramework/CubeMesh.h"
#include "SpecularLightShader.h"


class Cell
{
public:
	Cell(CubeMesh *m_TemplateModel, CubeMesh *m_TemplateModel2, float size);
	~Cell();

	bool CellContainsPoint(XMFLOAT3 point);
	void SetTexture(string filename) {}
	void SetCoordinates(float x, float y, float z) { position = XMFLOAT3(x, y, z); }
	XMFLOAT3 GetCoordinates() { return position; }
	float GetScale() { return scale; }
	void SetParent(int regionID) { parentRegion = regionID; }
	void Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
		SpecularLightShader* shader, Light* light, XMFLOAT3 cameraPosition);
	bool altTexture;

private:
	float scale;
	XMFLOAT3 position;
	CubeMesh* m_SurfaceModel;
	CubeMesh* m_AltSurfaceModel;
	int parentRegion;
	bool occupied;
	
};

#endif // !_CELL_H
