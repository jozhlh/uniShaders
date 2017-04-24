#include "Cell.h"

Cell::Cell(ID3D11Device *device, ID3D11DeviceContext *deviceContext, int cellX, int cellZ, int xSegments, int zSegments, NoiseData* noise, float size, float border, ImprovedNoise* noiseEngine)
{
	m_SurfaceModel = new ProcCubeMesh(device, deviceContext, L"../res/checkerboard.png", cellX, cellZ, xSegments, zSegments, border, noise, noiseEngine);
	scale = 0.5*size;
	occupied = false;
}

Cell::~Cell()
{
	if (m_SurfaceModel)
	{
		delete m_SurfaceModel;
		m_SurfaceModel = nullptr;
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
	SpecularTerrainShader* shader, Light* light, XMFLOAT3 cameraPosition, Texture* tex, XMFLOAT3 terrainBaseColor, bool identify, int numOfRegions)
{
	XMMATRIX worldMatrix = world;
	// Translate sphere mesh
	//worldMatrix = XMMatrixTranslation(scale*position.x, scale*position.y, scale *position.z) + XMMatrixScaling(scale, scale, scale);
	worldMatrix = XMMatrixMultiply(XMMatrixScaling(scale, scale, scale), XMMatrixTranslation(position.x, position.y, position.z));

	//// Send geometry data (from mesh)
	m_SurfaceModel->SendData(deviceContext);
	//// Set shader parameters (matrices and texture)
	shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, tex->GetTexture(), light, cameraPosition, parentRegion, terrainBaseColor, identify, numOfRegions);
	//// Render object (combination of mesh geometry and shader process
	shader->Render(deviceContext, m_SurfaceModel->GetIndexCount());

}