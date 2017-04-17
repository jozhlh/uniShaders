#include "Cell.h"

Cell::Cell(CubeMesh *m_TemplateModel, CubeMesh *m_TemplateModel2, float size)
{
	m_SurfaceModel = new CubeMesh(*m_TemplateModel);
	m_AltSurfaceModel = new CubeMesh(*m_TemplateModel2);
	scale = 0.5*size;
	occupied = false;
	altTexture = false;
}

Cell::~Cell()
{

}

bool Cell::CellContainsPoint(XMFLOAT3 point)
{
	if (point.x < (position.x - (scale)))
	{
		return false;
	}
	if (point.x > (position.x + (scale)))
	{
		return false;
	}
	if (point.z < (position.z - (scale)))
	{
		return false;
	}
	if (point.z > (position.z + (scale)))
	{
		return false;
	}
	return true;
}

void Cell::Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
	SpecularLightShader* shader, Light* light, XMFLOAT3 cameraPosition)
{
	XMMATRIX worldMatrix = world;
	// Translate sphere mesh
	//worldMatrix = XMMatrixTranslation(scale*position.x, scale*position.y, scale *position.z) + XMMatrixScaling(scale, scale, scale);
	worldMatrix = XMMatrixMultiply(XMMatrixScaling(scale, scale, scale), XMMatrixTranslation(position.x, position.y, position.z));
	
	
	if (altTexture)
	{
		//// Send geometry data (from mesh)
		m_AltSurfaceModel->SendData(deviceContext);
		//// Set shader parameters (matrices and texture)
		shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, m_AltSurfaceModel->GetTexture(), light, cameraPosition);
		//// Render object (combination of mesh geometry and shader process
		shader->Render(deviceContext, m_AltSurfaceModel->GetIndexCount());
	}
	else
	{
		//// Send geometry data (from mesh)
		m_SurfaceModel->SendData(deviceContext);
		//// Set shader parameters (matrices and texture)
		shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, m_SurfaceModel->GetTexture(), light, cameraPosition);
		//// Render object (combination of mesh geometry and shader process
		shader->Render(deviceContext, m_SurfaceModel->GetIndexCount());
	}
	
	// Reset world matrix
	float deScale = 1.0f;
	if (deScale > 0) deScale = 1.0f / scale;
	worldMatrix = XMMatrixScaling(deScale, deScale, deScale) + XMMatrixTranslation(-position.x, -position.y, -position.z);
}