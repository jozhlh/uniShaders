#include "Cell.h"

Cell::Cell(CubeMesh *m_TemplateModel, float size)
{
	m_CellModel = new CubeMesh(*m_TemplateModel);
	scale = size;
}

Cell::~Cell()
{

}

void Cell::Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
	SpecularLightShader* shader, Light* light, XMFLOAT3 cameraPosition)
{
	XMMATRIX worldMatrix = world;
	// Translate sphere mesh
	worldMatrix = XMMatrixScaling(scale, scale, scale);
	worldMatrix += XMMatrixTranslation(position.x, position.y, position.z);
	//// Send geometry data (from mesh)
	m_CellModel->SendData(deviceContext);
	//// Set shader parameters (matrices and texture)
	shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, m_CellModel->GetTexture(), light, cameraPosition);
	//// Render object (combination of mesh geometry and shader process
	shader->Render(deviceContext, m_CellModel->GetIndexCount());
	// Reset world matrix
	float deScale = 1.0f;
	if (deScale > 0) deScale = 1.0f / scale;
	worldMatrix = XMMatrixScaling(deScale, deScale, deScale) + XMMatrixTranslation(-position.x, -position.y, -position.z);
}