#include "Region.h"

Region::Region()
{
	nodeSphere = nullptr;
	centreSphere = nullptr;
	hasMajorBuilding = false;
}

Region::~Region()
{
	if (nodeSphere)
	{
		delete nodeSphere;
		nodeSphere = nullptr;
	}
	if (centreSphere)
	{
		delete centreSphere;
		centreSphere = nullptr;
	}
	/*if (centralBuilding)
	{
		delete centralBuilding;
		centralBuilding = nullptr;
	}*/
	/*if (derrick)
	{
		delete derrick;
		derrick = nullptr;
	}*/
}

void Region::Init(ID3D11Device * device, ID3D11DeviceContext * deviceContext, int regionNum)
{
	nodeSphere = new SphereMesh(device, deviceContext, L"../res/bunny.png", 20);
	centreSphere = new SphereMesh(device, deviceContext, L"../res/white.png", 20);
	sphereScale = 0.2f;
	numOfCells = 0;
	id = regionNum;
	yOff = 0.5f;
	buildingOffset = -0.2f;
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
}

void Region::DifferentiateCells(float r)
{
	//yOff = 0.5f + r;
	for each (Cell* cell in m_ChildCells)
	{
		//cell->SetCoordinates(cell->GetCoordinates().x, r, cell->GetCoordinates().z);
		cell->SetParent((int)r);
	}
}

void Region::GiveCell(Cell* m_Cell)
{
	m_ChildCells.push_back(m_Cell);
	numOfCells++;
}

void Region::AssignMajorBuilding(Building * building, float cellSize, int zCells)
{
	centralBuilding = building;
	
	hasMajorBuilding = CheckBuildingPlacement(centralBuilding, centreOfRegion, cellSize);
	if (centralBuilding->location.z < 0)
	{
		if (centralBuilding->rotation < 1)
		{
			centralBuilding->rotation = XM_PI;
		}
	}
	
}

void Region::PlaceDerrick(Building * derrickModel, float cellSize)
{
	derrick = *derrickModel;

	XMFLOAT3 nodePos = XMFLOAT3(nodeCoords.x, 0, nodeCoords.y);
	hasDerrick = CheckBuildingPlacement(&derrick, nodePos, cellSize);

}

void Region::PlaceMinorAsset(Building* building, float cellSize)
{
	if (m_OccupiedCells.size() > 0)
	{
		// how far away to try to place the minor asset
		int displacementFromTestCell = 2;

		// number of times to look for placement before giving up
		int testAttempts = 10;

		Building mAsset = *building;

		// seed:rd() <- can set as an actual seed... this is a non-deterministic 32-bit seed
		random_device rd;
		mt19937 mt(rd());

		uniform_int_distribution<int> occupiedCellSelection(0, m_OccupiedCells.size() - 1);
		uniform_int_distribution<int> directionSelection(0, 3);

		bool canPlaceAsset = false;

		int iterator = 0;
		while (iterator < testAttempts)
		{
			int cellNum = occupiedCellSelection(mt);
			int direction = directionSelection(mt);

			Cell* testCell = m_OccupiedCells[cellNum];

			int attempts = 0;
			canPlaceAsset = false;

			while (attempts < 4)
			{
				XMFLOAT3 testCoordinates = testCell->GetCoordinates();
				switch (direction)
				{
				case 0:
					// look at -x
					testCoordinates.x -= (displacementFromTestCell * cellSize);
					break;
				case 1:
					// look at +z
					testCoordinates.z += (displacementFromTestCell * cellSize);
					break;
				case 2:
					// look at +x
					testCoordinates.x += (displacementFromTestCell * cellSize);
					break;
				case 3:
					// look at -z
					testCoordinates.z -= (displacementFromTestCell * cellSize);
					break;
				}

				if (CheckBuildingPlacement(&mAsset, testCoordinates, cellSize))
				{
					break;
				}
				else
				{
					attempts++;
					direction++;
					if (direction > 3)
					{
						direction = 0;
					}
				}
			}

			if (attempts < 4)
			{
				canPlaceAsset = true;
				break;
			}
			else
			{
				iterator++;
			}
		}

		if (canPlaceAsset)
		{
			//success
			m_MinorAssets.push_back(mAsset);
		}
		else
		{
			// gave up looking for a place
		}
	}
}

void Region::SetCellIDs(int order)
{
	id = order;
	for each(Cell* cell in m_ChildCells)
	{
		cell->SetParent(order);
	}
}

bool Region::CheckBuildingPlacement(Building* building, XMFLOAT3 centralCell, float cellSize)
{
	bool canBePlaced = false;
	vector<XMFLOAT3> buildingCoordinates = GetBuildingCoordinates(building->dimensions.x, building->dimensions.z, centralCell, cellSize); // to check rotation, switch z and x
	list<Cell*> tempOccupied;
	for each (Cell* cell in m_ChildCells)
	{
		if (!cell->IsOccupied())
		{
			int iterator = 0;
			bool validCell = false;
			for each (XMFLOAT3 testPosition in buildingCoordinates)
			{
				if (cell->CellContainsPoint(testPosition))
				{
					validCell = true;
					cell->SetOccupied(true);
					tempOccupied.push_back(cell);
					break;
				}
				else
				{
					iterator++;
				}
			}
			if (validCell)
			{
				buildingCoordinates.erase(buildingCoordinates.begin() + iterator);
			}
		}
	}
	if (buildingCoordinates.size() < 1)
	{
		for each (Cell* occupiedCell in tempOccupied)
		{
			m_OccupiedCells.push_back(occupiedCell);
		}
		canBePlaced = true;
		building->location = centralCell;
		building->rotation = 0;
		return true;
	}
	else
	{
		for each (Cell* occupiedCell in tempOccupied)
		{
			occupiedCell->SetOccupied(false);
		}
		tempOccupied.clear();
		buildingCoordinates.clear();
		// reorientate and try again
		buildingCoordinates = GetBuildingCoordinates(building->dimensions.z, building->dimensions.x, centralCell, cellSize); // to check rotation, switch z and x
		//list<Cell*> tempOccupied;
		for each (Cell* cell in m_ChildCells)
		{
			if (!cell->IsOccupied())
			{
				int iterator = 0;
				bool validCell = false;
				for each (XMFLOAT3 testPosition in buildingCoordinates)
				{
					if (cell->CellContainsPoint(testPosition))
					{
						validCell = true;
						cell->SetOccupied(true);
						tempOccupied.push_back(cell);
						break;
					}
					else
					{
						iterator++;
					}
				}
				if (validCell)
				{
					buildingCoordinates.erase(buildingCoordinates.begin() + iterator);
				}
			}
		}

		if (buildingCoordinates.size() < 1)
		{
			for each (Cell* occupiedCell in tempOccupied)
			{
				m_OccupiedCells.push_back(occupiedCell);
			}
			canBePlaced = true;
			building->location = centralCell;
			building->rotation = XM_PIDIV2;
			return true;
		}
		else
		{
			for each (Cell* occupiedCell in tempOccupied)
			{
				occupiedCell->SetOccupied(false);
			}
			// cannot be placed in this location
		}
	}


	return false;
}

vector<XMFLOAT3> Region::GetBuildingCoordinates(float xDimension, float zDimension, XMFLOAT3 centralCell, float cellSize)
{
	vector<XMFLOAT3> buildingCoordinates;
	for (int x = 0; x < xDimension; x++)
	{
		// for zIterator = 0; zIterator < dimensions.z
		for (int z = 0; z < zDimension; z++)
		{
			XMFLOAT3 targetCoords = centralCell;
			if (xDimension > 1)
			{
				if ((int)xDimension % 2)
				{
					targetCoords.x = (targetCoords.x - ((xDimension - 1) * cellSize * 0.5)) + (x*cellSize);
				}
				else
				{
					targetCoords.x = (targetCoords.x - (xDimension * cellSize * 0.5)) + (x*cellSize);
				}
			}

			if (zDimension > 1)
			{
				if ((int)zDimension % 2)
				{
					targetCoords.z = (targetCoords.z - ((zDimension - 1) * cellSize * 0.5)) + (z*cellSize);
				}
				else
				{
					targetCoords.z = (targetCoords.z - (zDimension * cellSize * 0.5)) + (z*cellSize);
				}
			}

			buildingCoordinates.push_back(targetCoords);
		}
	}
	return buildingCoordinates;
}

void Region::Render(ID3D11DeviceContext * deviceContext, const XMMATRIX & world, const XMMATRIX & view, const XMMATRIX & projection,
	SpecularLightShader * shader, Light * light, XMFLOAT3 cameraPosition, Texture* tex, XMFLOAT3 centreColour, XMFLOAT3 nodeColour,
	bool showNodes)
{
	XMMATRIX worldMatrix = world;
	


	//// Send geometry data (from mesh)
	if (showNodes)
	{
		// Translate sphere mesh
		worldMatrix = XMMatrixMultiply(XMMatrixScaling(sphereScale, sphereScale, sphereScale), XMMatrixTranslation(nodeCoords.x, yOff, nodeCoords.y));

		nodeSphere->SendData(deviceContext);
		//// Set shader parameters (matrices and texture)
		shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, tex->GetTexture(), light, cameraPosition, showNodes, nodeColour);
		//// Render object (combination of mesh geometry and shader process
		shader->Render(deviceContext, nodeSphere->GetIndexCount());

		// Translate sphere mesh
		worldMatrix = XMMatrixMultiply(XMMatrixScaling(sphereScale * 1.2f, sphereScale * 1.2f, sphereScale * 1.2f), XMMatrixTranslation(centreOfRegion.x, yOff, centreOfRegion.z));

		//// Send geometry data (from mesh)
		centreSphere->SendData(deviceContext);
		//// Set shader parameters (matrices and texture)
		shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, tex->GetTexture(), light, cameraPosition, showNodes, centreColour);
		//// Render object (combination of mesh geometry and shader process
		shader->Render(deviceContext, centreSphere->GetIndexCount());
	}
	else
	{
		if (hasMajorBuilding)
		{
			RenderCentralBuilding(deviceContext, world, view, projection, shader, light, cameraPosition, tex, showNodes, nodeColour);
		}

		if (hasDerrick)
		{
			worldMatrix = XMMatrixMultiply(XMMatrixScaling(derrick.scale, derrick.scale, derrick.scale), XMMatrixTranslation(derrick.location.x, buildingOffset, derrick.location.z));
			//// Send geometry data (from mesh)
			derrick.asset->SendData(deviceContext);
			//// Set shader parameters (matrices and texture)
			shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, tex->GetTexture(), light, cameraPosition, showNodes, nodeColour);
			//// Render object (combination of mesh geometry and shader process
			shader->Render(deviceContext, derrick.asset->GetIndexCount());
		}

		if (m_MinorAssets.size() > 0)
		{
			for each (Building mAsset in m_MinorAssets)
			{
				XMVECTOR axis = { 0.0f, 1.0f, 0.0f };

				XMMATRIX tempMatrix = XMMatrixMultiply(XMMatrixScaling(mAsset.scale, mAsset.scale, mAsset.scale), XMMatrixRotationAxis(axis, mAsset.rotation));
				worldMatrix = XMMatrixMultiply(tempMatrix, XMMatrixTranslation(mAsset.location.x, buildingOffset, mAsset.location.z));

				//// Send geometry data (from mesh)
				mAsset.asset->SendData(deviceContext);
				//// Set shader parameters (matrices and texture)
				shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, tex->GetTexture(), light, cameraPosition, showNodes, nodeColour);
				//// Render object (combination of mesh geometry and shader process
				shader->Render(deviceContext, mAsset.asset->GetIndexCount());
			}
		}
	}
	

	

}

void Region::RenderCentralBuilding(ID3D11DeviceContext * deviceContext, const XMMATRIX & world, const XMMATRIX & view, const XMMATRIX & projection,
	SpecularLightShader * shader, Light * light, XMFLOAT3 cameraPosition, Texture* tex, bool tile, XMFLOAT3 tileColour)
{

	XMMATRIX worldMatrix = world;
	// Translate sphere mesh
	XMVECTOR axis = { 0.0f, 1.0f, 0.0f };

	XMMATRIX tempMatrix = XMMatrixMultiply(XMMatrixScaling(centralBuilding->scale, centralBuilding->scale, centralBuilding->scale), XMMatrixRotationAxis(axis, centralBuilding->rotation));
	//worldMatrix = XMMatrixScaling(centralBuilding->scale, centralBuilding->scale, centralBuilding->scale);
	worldMatrix = XMMatrixMultiply(tempMatrix, XMMatrixTranslation(centralBuilding->location.x, buildingOffset, centralBuilding->location.z));

	//// Send geometry data (from mesh)
	centralBuilding->asset->SendData(deviceContext);
	//// Set shader parameters (matrices and texture)
	shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, tex->GetTexture(), light, cameraPosition, tile, tileColour);
	//// Render object (combination of mesh geometry and shader process
	shader->Render(deviceContext, centralBuilding->asset->GetIndexCount());
}