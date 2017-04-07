#include "Region.h"

Region::Region()
{
	nodeSphere = nullptr;
	centreSphere = nullptr;
}

Region::~Region()
{
}

void Region::Init(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	nodeSphere = new SphereMesh(device, deviceContext, L"../res/bunny.png", 20);
	centreSphere = new SphereMesh(device, deviceContext, L"../res/white.png", 20);
	sphereScale = 0.2f;
	numOfCells = 0;
}

void Region::CalculateCentre(float cellArea)
{
	float sumOfCentreX = 0.0f;
	float sumOfCentreZ = 0.0f;
	float sumOfAreas = 0.0f;
	for each (Cell* cell in m_ChildCells)
	{
		sumOfCentreX += (cell->GetCoordinates().x * cellArea);
		sumOfCentreZ += (cell->GetCoordinates().z * cellArea);
		sumOfAreas += cellArea;
	}
	centreOfRegion.x = sumOfCentreX / sumOfAreas;
	centreOfRegion.z = sumOfCentreZ / sumOfAreas;
	centreOfRegion.y = 0.0f;
}

void Region::DifferentiateCells(float r)
{
	yOff = 0.5f + r;
	for each (Cell* cell in m_ChildCells)
	{
		cell->SetCoordinates(cell->GetCoordinates().x, r, cell->GetCoordinates().z);
	}
}

void Region::Render(ID3D11DeviceContext * deviceContext, const XMMATRIX & world, const XMMATRIX & view, const XMMATRIX & projection, SpecularLightShader * shader, Light * light, XMFLOAT3 cameraPosition)
{
	XMMATRIX worldMatrix = world;
	// Translate sphere mesh
	worldMatrix = XMMatrixMultiply(XMMatrixScaling(sphereScale, sphereScale, sphereScale), XMMatrixTranslation(nodeCoords.x, yOff, nodeCoords.y));

	//// Send geometry data (from mesh)
	nodeSphere->SendData(deviceContext);
	//// Set shader parameters (matrices and texture)
	shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, nodeSphere->GetTexture(), light, cameraPosition);
	//// Render object (combination of mesh geometry and shader process
	shader->Render(deviceContext, nodeSphere->GetIndexCount());
	// Reset world matrix
	float deScale = 1.0f;
	if (deScale > 0) deScale = 1.0f / sphereScale;
	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-nodeCoords.x, -yOff, -nodeCoords.y), XMMatrixScaling(deScale, deScale, deScale));

	// Translate sphere mesh
	worldMatrix = XMMatrixMultiply(XMMatrixScaling(sphereScale * 1.2f, sphereScale * 1.2f, sphereScale * 1.2f), XMMatrixTranslation(centreOfRegion.x, yOff, centreOfRegion.z));

	//// Send geometry data (from mesh)
	centreSphere->SendData(deviceContext);
	//// Set shader parameters (matrices and texture)
	shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, centreSphere->GetTexture(), light, cameraPosition);
	//// Render object (combination of mesh geometry and shader process
	shader->Render(deviceContext, centreSphere->GetIndexCount());
	// Reset world matrix
	deScale = 1.0f;
	if (deScale > 0) deScale = 1.0f / sphereScale;
	worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-centreOfRegion.x, -yOff, -centreOfRegion.z), XMMatrixScaling(deScale, deScale, deScale));
}
