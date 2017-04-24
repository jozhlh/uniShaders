#include "VoronoiMap.h"

VoronoiMap::VoronoiMap(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ModelBank* bank,
	XMINT2 dimensions, float border, float noiseHeight, float noiseResolution, float size, float numberOfRegions, XMFLOAT3 baseColour)
{
	random_device rd;
	mt19937 mt(rd());

	uniform_int_distribution<int> noiseLocationSelection(1, 100);

	//basePlateColour = baseColour;
	yPos = 0;
	cellSize = size;
	xCells = dimensions.x;
	zCells = dimensions.y;
	cellBorder = border;
	numOfRegions = numberOfRegions;
	noise.Height = noiseHeight;
	noise.Resolution = noiseResolution;
	noise.Location = XMFLOAT2(noiseLocationSelection(mt), noiseLocationSelection(mt));
	noise.scaleModifier = cellSize;
	noiseEngine = new ImprovedNoise();
	modelBank = bank;
	modelBank->Init(device, deviceContext);
	stockTexture = new Texture(device, deviceContext, L"../res/palette.png");
	GenerateRegions(numberOfRegions);
	AssignCellsToRegions(device, deviceContext);

	basePlate = new CubeMesh(device, deviceContext, L"../res/palette.png", 2);

	uniform_int_distribution<int> minorAssetSelection(1, MINOR_BUILDINGS-1);

	int iterator = 0;
	//int r = 0.0;
	for each (Region* region in regions)
	{
		region->Init(device, deviceContext, iterator);
		region->CalculateCentre(cellSize*cellSize);
		
		iterator++;
		//region->DifferentiateCells(r * .2f);
		//r++;
	}
	SortRegionsBySize();
	int r = 0.0f;
	for each (Region* region in regions)
	{
		region->DifferentiateCells(r);
		region->AssignMajorBuilding(&modelBank->majorBuildings[(int)r], cellSize, zCells);
		region->PlaceDerrick(&modelBank->minorBuildings[0], cellSize);
		region->PlaceMinorAsset(&modelBank->minorBuildings[minorAssetSelection(mt)], cellSize);
		for each (Region* assetRegion in regions)
		{
			if (assetRegion == region)
			{
				break;
			}
			assetRegion->PlaceMinorAsset(&modelBank->minorBuildings[minorAssetSelection(mt)], cellSize);
		}
		r++;
	}
}

VoronoiMap::~VoronoiMap()
{
	if (noiseEngine)
	{
		delete noiseEngine;
		noiseEngine = nullptr;
	}

	if (stockTexture)
	{
		delete stockTexture;
		stockTexture = nullptr;
	}

	if (basePlate)
	{
		delete basePlate;
		basePlate = nullptr;
	}

	for each (Cell* cell in unassignedCells)
	{
		delete cell;
		cell = nullptr;
	}

	for each (Region* region in regions)
	{
		delete region;
		region = nullptr;
	}
}

void VoronoiMap::Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
	SpecularLightShader *shader, SpecularTerrainShader* terrainShader, Light *light, XMFLOAT3 cameraPosition, XMFLOAT3 nodeColour, XMFLOAT3 centreColour, XMFLOAT3 regionColour,
	bool showNodes, bool identifyRegions, XMFLOAT3 baseColour, float yOff)
{
	for each (Cell* cell in unassignedCells)
	{
		cell->Render(deviceContext, world, view, projection, terrainShader, light, cameraPosition, stockTexture, regionColour, identifyRegions, numOfRegions);
	}
	for each (Region* region in regions)
	{
		region->Render(deviceContext, world, view, projection, shader, light, cameraPosition, stockTexture, centreColour, nodeColour, showNodes, yOff);
	}

	XMMATRIX worldMatrix = XMMatrixMultiply(XMMatrixScaling((cellSize * 0.5f * xCells) + 0.2f, 0.25f, (cellSize * 0.5f * zCells) + 0.2f), XMMatrixTranslation(0, -0.75f, 0));
	
	//// Send geometry data (from mesh)
	basePlate->SendData(deviceContext);
	//// Set shader parameters (matrices and texture)
	shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, basePlate->GetTexture(), light, cameraPosition, true, baseColour);
	//// Render object (combination of mesh geometry and shader process
	shader->Render(deviceContext, basePlate->GetIndexCount());
}

void VoronoiMap::GenerateRegions(int num)
{
	xWidth = xCells * cellSize;
	zWidth = zCells * cellSize;

	// seed:rd() <- can set as an actual seed... this is a non-deterministic 32-bit seed
	random_device rd;
	mt19937 mt(rd());
	
	for (int r = 0; r < num; r++)
	{
		uniform_int_distribution<int> xDist(1, (xCells *cellSize) - 1);
		uniform_int_distribution<int> zDist(1, (zCells *cellSize) - 1);
		Region* m_Region = new Region();
		m_Region->SetNodeCoordinates(xDist(mt)-(0.5* xWidth), zDist(mt) - (0.5* zWidth));
		regions.push_back(m_Region);
	}
}

void VoronoiMap::AssignCellsToRegions(ID3D11Device *device, ID3D11DeviceContext *deviceContext)
{
	
	for (int x = 0; x < xCells; x++)
	{
		for (int z = 0; z < zCells; z++)
		{
			Cell* m_Cell = new Cell(device, deviceContext, x, z, xCells, zCells, &noise, cellSize, cellBorder, noiseEngine);;
			m_Cell->SetCoordinates((xWidth * -0.5) + ((cellSize*0.5f) + (x*cellSize)), yPos, (zWidth * -0.5) + ((cellSize*0.5f) + (z*cellSize)));
			unassignedCells.push_back(m_Cell);
		}
	}

	for each (Cell* cell in unassignedCells)
	{
		Region* m_ClosestRegion = nullptr;
		float shortestRegionDistance = cellSize * xCells * zCells;

		for each (Region* region in regions)
		{
			if (cell->CellContainsPoint(XMFLOAT3(region->GetNode().x, 0, region->GetNode().y)))
			{
				region->SetNodeCoordinates(cell->GetCoordinates().x, cell->GetCoordinates().z);
			}
			if (RegionDistance(cell, region) < shortestRegionDistance)
			{
				shortestRegionDistance = RegionDistance(cell, region);
				m_ClosestRegion = region;
			}
		}

		// Catch if calculation failed
		if (m_ClosestRegion == nullptr)
		{
			m_ClosestRegion = regions.front();
		}

		m_ClosestRegion->GiveCell(cell);
	}


}

float VoronoiMap::RegionDistance(Cell* cell, Region* region)
{
	XMFLOAT2 regionNode = region->GetNode();
	XMFLOAT3 cellPosition = cell->GetCoordinates();

	float xComponent = regionNode.x - cellPosition.x;
	float yComponent = regionNode.y - cellPosition.z;
	return sqrtf((xComponent * xComponent) + (yComponent * yComponent));
}

void VoronoiMap::SortRegionsBySize()
{
	list<Region*> tempRegions;
	while (regions.size() > 0)
	{
		Region* largestRegion = regions.front();
		for each (Region* region in regions)
		{
			if (region->GetCellCount() > largestRegion->GetCellCount())
			{
				largestRegion = region;
			}
		}
		tempRegions.push_back(largestRegion);
		regions.remove(largestRegion);
	}
	regions = tempRegions;
	int order = 0;
	for each (Region* region in regions)
	{
		region->SetCellIDs(order);
		order++;
	}
}