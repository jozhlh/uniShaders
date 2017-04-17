#include "Cell.h"

Cell::Cell(ID3D11Device *device, ID3D11DeviceContext *deviceContext, int cellX, int cellZ, int xSegments, int zSegments, NoiseData* noise, float size, ImprovedNoise* noiseEngine)
{
	m_SurfaceModel = new ProcCubeMesh(device, deviceContext, L"../res/checkerboard.png", cellX, cellZ, xSegments, zSegments, noise, noiseEngine);
	m_AltSurfaceModel = new ProcCubeMesh(device, deviceContext, L"../res/DefaultDiffuse.png", cellX, cellZ, xSegments, zSegments, noise, noiseEngine);
	//fallbackMesh = new CubeMesh(device, deviceContext, L"../res/checkerboard.png", 1);
	scale = 0.5*size;
	occupied = false;
	altTexture = false;
}

Cell::~Cell()
{
	if (m_SurfaceModel)
	{
		delete m_SurfaceModel;
		m_SurfaceModel = nullptr;
	}

	if (m_AltSurfaceModel)
	{
		delete m_AltSurfaceModel;
		m_AltSurfaceModel = nullptr;
	}
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
	SpecularLightShader* shader, Light* light, XMFLOAT3 cameraPosition, Texture* tex, bool tile, XMFLOAT3 tileColour)
{
	XMMATRIX worldMatrix = world;
	// Translate sphere mesh
	//worldMatrix = XMMatrixTranslation(scale*position.x, scale*position.y, scale *position.z) + XMMatrixScaling(scale, scale, scale);
	worldMatrix = XMMatrixMultiply(XMMatrixScaling(scale, scale, scale), XMMatrixTranslation(position.x, position.y, position.z));
	/*
	//// Send geometry data (from mesh)
	fallbackMesh->SendData(deviceContext);
	//// Set shader parameters (matrices and texture)
	shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, fallbackMesh->GetTexture(), light, cameraPosition);
	//// Render object (combination of mesh geometry and shader process
	shader->Render(deviceContext, fallbackMesh->GetIndexCount());
	*/
	if (altTexture)
	{
		//// Send geometry data (from mesh)
		m_AltSurfaceModel->SendData(deviceContext);
		//// Set shader parameters (matrices and texture)
		shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, tex->GetTexture(), light, cameraPosition, tile, tileColour);
		//// Render object (combination of mesh geometry and shader process
		shader->Render(deviceContext, m_AltSurfaceModel->GetIndexCount());
	}
	else
	{
		//// Send geometry data (from mesh)
		m_SurfaceModel->SendData(deviceContext);
		//// Set shader parameters (matrices and texture)
		shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, tex->GetTexture(), light, cameraPosition, tile, tileColour);
		//// Render object (combination of mesh geometry and shader process
		shader->Render(deviceContext, m_SurfaceModel->GetIndexCount());
	}
	
	// Reset world matrix
	//float deScale = 1.0f;
	//if (deScale > 0) deScale = 1.0f / scale;
	//worldMatrix = XMMatrixScaling(deScale, deScale, deScale) + XMMatrixTranslation(-position.x, -position.y, -position.z);
}