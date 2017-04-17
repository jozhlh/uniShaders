#include "Region.h"

Region::Region()
{
	nodeSphere = nullptr;
	centreSphere = nullptr;
	hasMajorBuilding = false;
}

Region::~Region()
{
}

void Region::Init(ID3D11Device * device, ID3D11DeviceContext * deviceContext, int regionNum)
{
	nodeSphere = new SphereMesh(device, deviceContext, L"../res/bunny.png", 20);
	centreSphere = new SphereMesh(device, deviceContext, L"../res/white.png", 20);
	sphereScale = 0.2f;
	numOfCells = 0;
	id = regionNum;
	for each (Cell* cell in m_ChildCells)
	{
		cell->SetParent(id);
	}
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

	// Put centre point as a centre cell
	for each (Cell* cell in m_ChildCells)
	{
		//cell->altTexture = true;
		// if cell.contains( targetCoords)
		if (cell->CellContainsPoint(centreOfRegion))
		{
			centreOfRegion.x = cell->GetCoordinates().x;
			centreOfRegion.z = cell->GetCoordinates().z;
			break;
		}
	}

	for each (Cell* cell in m_ChildCells)
	{
		//cell->altTexture = true;
		// if cell.contains( targetCoords)
		if (cell->CellContainsPoint(XMFLOAT3(nodeCoords.x, 0.0f, nodeCoords.y)))
		{
			nodeCoords.x = cell->GetCoordinates().x;
			nodeCoords.y = cell->GetCoordinates().z;
			break;
		}
	}
}

void Region::DifferentiateCells(float r)
{
	yOff = 0.5f + r;
	for each (Cell* cell in m_ChildCells)
	{
		cell->SetCoordinates(cell->GetCoordinates().x, r, cell->GetCoordinates().z);
	}
}

void Region::GiveCell(Cell* m_Cell)
{
	m_ChildCells.push_back(m_Cell);
	numOfCells++;
}

void Region::AssignMajorBuilding(Building * building, float cellSize)
{
	centralBuilding = building;
	// Calculate building placement / rotation
		// Get Central Cell Coordinates
			// validPlacement = true
	bool validPlacement = true;
	// for xIterator = 0; xIterator < dimensions.x
	for (int x = 0; x < building->dimensions.x; x++)
	{
		// for zIterator = 0; zIterator < dimensions.z
		for (int z = 0; z < building->dimensions.z; z++)
		{
			if (!CheckOrientation(centreOfRegion, x, z, building->dimensions.x, building->dimensions.z, cellSize))
			{
				validPlacement = false;
				break;
			}
		}
		if (!validPlacement)
		{
			break;
		}
	}
	if (!validPlacement)
	{
		for each (Cell* cell in m_ChildCells)
		{
			cell->altTexture = false;
		}
		validPlacement = true;
		for (int x = 0; x < building->dimensions.z; x++)
		{
			// for zIterator = 0; zIterator < dimensions.z
			for (int z = 0; z < building->dimensions.x; z++)
			{
				if (!CheckOrientation(centreOfRegion, x, z, building->dimensions.z, building->dimensions.x, cellSize))
				{
					validPlacement = false;
					break;
				}
			}
			if (!validPlacement)
			{
				break;
			}
		}
		if (!validPlacement)
		{
			for each (Cell* cell in m_ChildCells)
			{
				cell->altTexture = false;
			}
			// building cannot be placed on central location
			hasMajorBuilding = false;
		}
		else
		{
			// Building could be placed in the second orientation
			buildingLocation = centreOfRegion;
			buildingRotation = XM_PIDIV2;
			hasMajorBuilding = true;
		}
	}
	else
	{
		// Building could be placed in the first orientation
		buildingLocation = centreOfRegion;
		buildingRotation = 0.0f;
		hasMajorBuilding = true;
	}
	
}

void Region::PlaceDerrick(Building * derrickModel, float cellSize)
{
	derrick = derrickModel;
	bool validPlacement = true;
	for (int x = 0; x < derrick->dimensions.x; x++)
	{
		// for zIterator = 0; zIterator < dimensions.z
		for (int z = 0; z < derrick->dimensions.z; z++)
		{
			if (!CheckOrientation(XMFLOAT3(nodeCoords.x, 0, nodeCoords.y), x, z, derrick->dimensions.x, derrick->dimensions.z, cellSize))
			{
				validPlacement = false;
				break;
			}
		}
		if (!validPlacement)
		{
			break;
		}
	}
	if (!validPlacement)
	{
		for each (Cell* cell in m_ChildCells)
		{
			cell->altTexture = false;
		}
		// building cannot be placed on central location
		hasDerrick = false;
	}
	else
	{
		// Building could be placed in the second orientation
		hasDerrick = true;
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

	RenderCentralBuilding(deviceContext, world, view, projection, shader, light, cameraPosition);

	if (hasDerrick)
	{
		worldMatrix = XMMatrixMultiply(XMMatrixScaling(derrick->scale, derrick->scale, derrick->scale), XMMatrixTranslation(nodeCoords.x, yOff, nodeCoords.y));
		//// Send geometry data (from mesh)
		derrick->asset->SendData(deviceContext);
		//// Set shader parameters (matrices and texture)
		shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, derrick->asset->GetTexture(), light, cameraPosition);
		//// Render object (combination of mesh geometry and shader process
		shader->Render(deviceContext, derrick->asset->GetIndexCount());
		// Reset world matrix
		deScale = 1.0f;
		if (deScale > 0) deScale = 1.0f / derrick->scale;
		worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-nodeCoords.x, -yOff, -nodeCoords.y), XMMatrixScaling(deScale, deScale, deScale));
	}
	
}

bool Region::CheckOrientation(XMFLOAT3 centre, int xIterator, int zIterator, float x, float z, float cellSize)
{
	// targetCoords = ((-(dimensions.x / 2 (rounded down))+ xIterator) * cellSize, (-(dimensions.y / 2 (rounded down))+ zIterator) * cellSize)
	//XMFLOAT3 targetCoords = XMFLOAT3(((centreOfRegion.x-(x / 2) + xIterator) * cellSize), (1 / 2) * cellSize, ((centreOfRegion.z-(z / 2) + zIterator) * cellSize));
	XMFLOAT3 targetCoords = centre;
	if (x < 2)
	{

	}
	else if ((int)x % 2)
	{
		targetCoords.x = (targetCoords.x - ((x - 1) * cellSize * 0.5)) + (xIterator*cellSize);
	}
	else
	{
		targetCoords.x = (targetCoords.x - (x * cellSize * 0.5)) + (xIterator*cellSize);
	}

	if (z < 2)
	{

	}
	else if ((int)z % 2)
	{
		targetCoords.z = (targetCoords.z - ((z - 1) * cellSize * 0.5)) + (zIterator*cellSize);
	}
	else
	{
		targetCoords.z = (targetCoords.z - (z * cellSize * 0.5)) + (zIterator*cellSize);
	}
	
	


	//targetCoords.x = (targetCoords.x) + (xIterator*cellSize);
	//targetCoords.z = (targetCoords.z) + (zIterator*cellSize);

	// validCell = false
	bool validCell = false;
	// for each cell in child cells
	for each (Cell* cell in m_ChildCells)
	{
		// check to see if the cell is occupied
		if (!cell->altTexture)
		{
			//cell->altTexture = true;
			// if cell.contains( targetCoords)
			if (cell->CellContainsPoint(targetCoords))
			{
				validCell = true;
				cell->altTexture = true;
				break;
			}
		}
	}
	// if validCell == false rotate and start over
	if (!validCell)
	{
		return false;
	}

	return true;
}



void Region::RenderCentralBuilding(ID3D11DeviceContext * deviceContext, const XMMATRIX & world, const XMMATRIX & view, const XMMATRIX & projection, SpecularLightShader * shader, Light * light, XMFLOAT3 cameraPosition)
{
	if (hasMajorBuilding)
	{
		XMMATRIX worldMatrix = world;
		// Translate sphere mesh
		XMVECTOR axis = { 0.0f, 1.0f, 0.0f };

		XMMATRIX tempMatrix = XMMatrixMultiply(XMMatrixScaling(centralBuilding->scale, centralBuilding->scale, centralBuilding->scale), XMMatrixRotationAxis(axis, buildingRotation));
		//worldMatrix = XMMatrixScaling(centralBuilding->scale, centralBuilding->scale, centralBuilding->scale);
		worldMatrix = XMMatrixMultiply(tempMatrix, XMMatrixTranslation(buildingLocation.x, yOff, buildingLocation.z));

		//// Send geometry data (from mesh)
		centralBuilding->asset->SendData(deviceContext);
		//// Set shader parameters (matrices and texture)
		shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, centralBuilding->asset->GetTexture(), light, cameraPosition);
		//// Render object (combination of mesh geometry and shader process
		shader->Render(deviceContext, centralBuilding->asset->GetIndexCount());
		// Reset world matrix
		float deScale = 1.0f;
		if (deScale > 0) deScale = 1.0f / centralBuilding->scale;
		worldMatrix = XMMatrixMultiply(XMMatrixTranslation(-buildingLocation.x, -yOff, -buildingLocation.z), XMMatrixRotationAxis(axis, -buildingRotation));
		worldMatrix = XMMatrixScaling(deScale, deScale, deScale);
	}
}

