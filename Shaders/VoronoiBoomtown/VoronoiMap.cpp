#include "VoronoiMap.h"

VoronoiMap::VoronoiMap(ID3D11Device *device, ID3D11DeviceContext *deviceContext)
{
	desertColour = XMFLOAT3(1.0f, 0.95f, 0.6f);
	yPos = 0;
	cellSize = 0.5f;
	xCells = 60;
	zCells = 25;
	noise.Height = 2.0f;
	noise.Resolution = 7.5f;
	noise.Location = XMFLOAT2(1.0f, 1.0f);
	noise.scaleModifier = cellSize;
	noiseEngine = new ImprovedNoise();
	modelBank = new ModelBank();
	modelBank->Init(device, deviceContext);
	stockTexture = new Texture(device, deviceContext, L"../res/palette.png");
	GenerateRegions(MAJOR_BUILDINGS);
	AssignCellsToRegions(device, deviceContext);

	random_device rd;
	mt19937 mt(rd());
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
		//region->DifferentiateCells(r * .05f);
		region->AssignMajorBuilding(&modelBank->majorBuildings[(int)r], cellSize);
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

	if (modelBank)
	{
		delete modelBank;
		modelBank = nullptr;
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
	SpecularLightShader *shader, Light *light, XMFLOAT3 cameraPosition)
{
	for each (Cell* cell in unassignedCells)
	{
		cell->Render(deviceContext, world, view, projection, shader, light, cameraPosition, stockTexture, true, desertColour);
	}
	for each (Region* region in regions)
	{
		region->Render(deviceContext, world, view, projection, shader, light, cameraPosition, stockTexture, false, desertColour);
	}
}

void VoronoiMap::GenerateRegions(int num)
{
	mapSize = xCells * cellSize;

	// seed:rd() <- can set as an actual seed... this is a non-deterministic 32-bit seed
	random_device rd;
	mt19937 mt(rd());
	
	for (int r = 0; r < num; r++)
	{
		uniform_int_distribution<int> xDist(1, (xCells *cellSize) - 1);
		uniform_int_distribution<int> zDist(1, (zCells *cellSize) - 1);
		Region* m_Region = new Region();
		m_Region->SetNodeCoordinates(xDist(mt)-(0.5* mapSize), zDist(mt) - (0.5* mapSize));
		regions.push_back(m_Region);
	}
}

void VoronoiMap::AssignCellsToRegions(ID3D11Device *device, ID3D11DeviceContext *deviceContext)
{
	
	for (int x = 0; x < xCells; x++)
	{
		for (int z = 0; z < zCells; z++)
		{
			Cell* m_Cell = new Cell(device, deviceContext, x, z, xCells, zCells, &noise, cellSize, noiseEngine);;
			m_Cell->SetCoordinates((mapSize * -0.5) + ((cellSize*0.5f) + (x*cellSize)), yPos, (mapSize * -0.5) + ((cellSize*0.5f) + (z*cellSize)));
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

//bool VoronoiMap::CompareRegionSize(const Region* first, const Region* second)
//{
//	if (first->GetCellCount() > second->GetCellCount())
//	{
//		return true;
//	}
//	return false;
//}

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
}