#ifndef _CELL_H
#define _CELL_H

#include "../DXFramework/CubeMesh.h"
#include "SpecularLightShader.h"

class Cell
{
public:
	Cell(CubeMesh *m_TemplateModel, float size);
	~Cell();

	void SetCoordinates(float x, float y, float z) { position = XMFLOAT3(x, y, z); }
	XMFLOAT3 GetCoordinates() { return position; }

	void Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
		SpecularLightShader* shader, Light* light, XMFLOAT3 cameraPosition);

private:
	float scale;
	XMFLOAT3 position;
	CubeMesh* m_CellModel;
};

#endif // !_CELL_H
