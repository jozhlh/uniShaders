#include "Cell.h"

Cell::Cell(CubeMesh *m_TemplateModel, float size)
{
	m_SurfaceModel = new CubeMesh(*m_TemplateModel);
	scale = 0.5*size;
	occupied = false;
}

Cell::~Cell()
{

}

void Cell::Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
	SpecularLightShader* shader, Light* light, XMFLOAT3 cameraPosition)
{
	XMMATRIX worldMatrix = world;
	// Translate sphere mesh
	//worldMatrix = XMMatrixTranslation(scale*position.x, scale*position.y, scale *position.z) + XMMatrixScaling(scale, scale, scale);
	worldMatrix = XMMatrixMultiply(XMMatrixScaling(scale, scale, scale), XMMatrixTranslation(position.x, position.y, position.z));
	
	

	//// Send geometry data (from mesh)
	m_SurfaceModel->SendData(deviceContext);
	//// Set shader parameters (matrices and texture)
	shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, m_SurfaceModel->GetTexture(), light, cameraPosition);
	//// Render object (combination of mesh geometry and shader process
	shader->Render(deviceContext, m_SurfaceModel->GetIndexCount());
	// Reset world matrix
	float deScale = 1.0f;
	if (deScale > 0) deScale = 1.0f / scale;
	worldMatrix = XMMatrixScaling(deScale, deScale, deScale) + XMMatrixTranslation(-position.x, -position.y, -position.z);
}