// Voronoi Boomtown - Region.cpp
// Josh Hale - 2017
// Contains a node and pointers to its cells and model data

#include "Region.h"

Region::Region()
{
	nodeSphere = nullptr;
	centreSphere = nullptr;
	centralBuilding = nullptr;
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
}

// Initialization, creates spheres at it's nodes
/// PARAMETERS
// device, deviceContext - pointers to D3D device
// regionNum - which region this is in the unordered list
void Region::Init(ID3D11Device * device, ID3D11DeviceContext * deviceContext, int regionNum)
{
	nodeSphere = new SphereMesh(device, deviceContext, L"../res/palette.png", 20);
	centreSphere = new SphereMesh(device, deviceContext, L"../res/palette.png", 20);
	sphereScale = 0.2f;
	numOfCells = 0;
	id = regionNum;
	buildingOffset = -0.2f;
}

// Calculates the centre of mass of the region
/// PARAMETERS
// cellArea - the area of a cell, used for calculation
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
		if (cell->CellContainsPoint(centreOfRegion))
		{
			centreOfRegion.x = cell->GetCoordinates().x;
			centreOfRegion.z = cell->GetCoordinates().z;
			break;
		}
	}
}

// Assign cell to the region
/// PARAMETERS
// m_Cell - cell to give to the region
void Region::GiveCell(Cell* m_Cell)
{
	m_ChildCells.push_back(m_Cell);
	numOfCells++;
}

// Try to place a building at the centre of the region
/// PARAMETERS
// building - asset to place
// cellSize - how wide a cell is
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

// Try to place a building at the node of the region
/// PARAMETERS
// building - asset to place
// cellSize - how wide a cell is
void Region::PlaceDerrick(Building * derrickModel, float cellSize)
{
	derrick = *derrickModel;

	XMFLOAT3 nodePos = XMFLOAT3(nodeCoords.x, 0, nodeCoords.y);
	hasDerrick = CheckBuildingPlacement(&derrick, nodePos, cellSize);
}

// Clumping algorithm for minor assets, try to place an asset near to another occupied cell in the region
/// PARAMETERS
// building - asset to place
// cellSize - how wide a cell is
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

			// Find a random occupied cell in the region
			Cell* testCell = m_OccupiedCells[cellNum];

			int attempts = 0;
			canPlaceAsset = false;

			// Test in a random direction, if it fails turn 90 deg clockwise and try again
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

// Give the cells the region's ID for colouring according to region size
/// PARAMETERS
// order - how large the region is
void Region::SetCellIDs(int order)
{
	id = order;
	for each(Cell* cell in m_ChildCells)
	{
		cell->SetParent(order);
	}
}

// See if an asset can be placed on the current cell, if not rotate and try agin
/// PARAMETERS
// building - asset to be placed
// centralCell - cell to place asset on
// cellSize - how large a cell is
/// OUTPUT
// bool - this asset can be placed on this cell
bool Region::CheckBuildingPlacement(Building* building, XMFLOAT3 centralCell, float cellSize)
{
	bool canBePlaced = false;
	vector<XMFLOAT3> buildingCoordinates = GetBuildingCoordinates(building->dimensions.x, building->dimensions.z, centralCell, cellSize); // to check rotation, switch z and x
	list<Cell*> tempOccupied;
	// Check all cells in the region against the building's coordinates
	// If all the coordinates have a matching cell that is unoccupied it can be placed
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
		// switch the building's z and x components to rotate it 90 degrees
		buildingCoordinates = GetBuildingCoordinates(building->dimensions.z, building->dimensions.x, centralCell, cellSize); // to check rotation, switch z and x
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
			// cannot be placed in this location
			for each (Cell* occupiedCell in tempOccupied)
			{
				occupiedCell->SetOccupied(false);
			}
		}
	}


	return false;
}

// Find a list of cell coordinates that this building wishes to occupy
/// PARAMETERS
// xDimension, zDimension - the size of the building in cells
// centralCell - which cell to place the asset on
// cellSize - how large the cells are
/// OUTPUT
// vector<XMFLOAT3> - vector of all coordinates the building wishes to occupy
vector<XMFLOAT3> Region::GetBuildingCoordinates(float xDimension, float zDimension, XMFLOAT3 centralCell, float cellSize)
{
	vector<XMFLOAT3> buildingCoordinates;
	for (int x = 0; x < xDimension; x++)
	{
		for (int z = 0; z < zDimension; z++)
		{
			XMFLOAT3 targetCoords = centralCell;
			if (xDimension > 1)
			{
				// if the dimension is an even number, the building must be offset by half a cell to be place uniformly over cells
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

// Render all the assets of the region or render the voronoi node and centre of the cell markers
/// PARAMETERS
// deviceContext - pointer to the D3D device
// world, view, projection - matrices for positional calculations
// shader - pointer to the model rendering shader
// light - pointer to the light parameter class
// cameraPosition - position for specularity calculations
// tex - the texture to be used on all models
// centreColour, nodeColour - colour of the sphere markers for the nodes
// shownodes - whether to render the nodes or assets
// yOff - how far to render the nodes from the terrain
void Region::Render(ID3D11DeviceContext * deviceContext, const XMMATRIX & world, const XMMATRIX & view, const XMMATRIX & projection,
	SpecularLightShader * shader, Light * light, XMFLOAT3 cameraPosition, Texture* tex, XMFLOAT3 centreColour, XMFLOAT3 nodeColour,
	bool showNodes, float yOff)
{
	XMMATRIX worldMatrix = world;
	
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

// Render the central asset of the region
/// PARAMETERS
// deviceContext - pointer to the D3D device
// world, view, projection - matrices for positional calculations
// shader - pointer to the model rendering shader
// light - pointer to the light parameter class
// cameraPosition - position for specularity calculations
// tex - the texture to be used on all models
// tile, tileColour - redundant variables that need to be sent to the shared shader
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