#include "VoronoiMap.h"

VoronoiMap::VoronoiMap(ID3D11Device *device, ID3D11DeviceContext *deviceContext)
{
	yPos = 0;
	cellSize = .4f;
	xCells = 80;
	zCells = 80;
	m_CellModelTemplate1 = new CubeMesh(device, deviceContext, L"../res/checkerboard.png", 2); // I think the resolution (2) refers to tris per face
	m_CellModelTemplate2 = new CubeMesh(device, deviceContext, L"../res/DefaultDiffuse.png", 2);

	GenerateRegions(30);
	AssignCellsToRegions();
	for each (Region* region in regions)
	{
		region->Init(device, deviceContext);
		region->CalculateCentre(cellSize*cellSize);
		//region->DifferentiateCells(r * .2f);
		//r++;
	}
	SortRegionsBySize();
	float r = 0.0f;
	for each (Region* region in regions)
	{
		region->DifferentiateCells(r * .05f);
		r++;
	}
}

VoronoiMap::~VoronoiMap()
{
	for each (Cell* cell in unassignedCells)
	{
		delete cell;
		cell = nullptr;
	}
}

void VoronoiMap::Render(ID3D11DeviceContext *deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection,
	SpecularLightShader *shader, Light *light, XMFLOAT3 cameraPosition)
{
	for each (Cell* cell in unassignedCells)
	{
		cell->Render(deviceContext, world, view, projection, shader, light, cameraPosition);
	}
	for each (Region* region in regions)
	{
		region->Render(deviceContext, world, view, projection, shader, light, cameraPosition);
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
		uniform_int_distribution<int> xDist(0, xCells *cellSize);
		uniform_int_distribution<int> zDist(0, zCells *cellSize);
		Region* m_Region = new Region();
		m_Region->SetNodeCoordinates(xDist(mt)-(0.5* mapSize), zDist(mt) - (0.5* mapSize));
		regions.push_back(m_Region);
	}

}

void VoronoiMap::AssignCellsToRegions()
{
	
	for (int x = 0; x < xCells; x++)
	{
		for (int z = 0; z < zCells; z++)
		{
			Cell* m_Cell;
			if (z % 2 > 0)
			{
				m_Cell = new Cell(m_CellModelTemplate1, cellSize);
			}
			else
			{
				m_Cell = new Cell(m_CellModelTemplate2, cellSize);
			}
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