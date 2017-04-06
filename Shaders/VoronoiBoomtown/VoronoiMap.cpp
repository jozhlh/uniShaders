#include "VoronoiMap.h"

VoronoiMap::VoronoiMap(ID3D11Device *device, ID3D11DeviceContext *deviceContext)
{
	yPos = 0;
	cellSize = 1.f;
	xCells = 10;
	zCells = 10;
	m_CellModelTemplate1 = new CubeMesh(device, deviceContext, L"../res/checkerboard.png", 2); // I think the resolution (2) refers to tris per face
	m_CellModelTemplate2 = new CubeMesh(device, deviceContext, L"../res/DefaultDiffuse.png", 2);

	GenerateRegions(1);
	AssignCellsToRegions();
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
}

void VoronoiMap::GenerateRegions(int num)
{
	for (int r = 0; r < num; r++)
	{
		Region* m_Region = new Region();
		m_Region->SetNodeCoordinates(1, 1);
		regions.push_back(m_Region);
	}

}

void VoronoiMap::SortRegionsBySize()
{

}

void VoronoiMap::AssignCellsToRegions()
{
	float mapSize = xCells * 2 * cellSize;
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
			m_Cell->SetCoordinates((mapSize * -0.5) + (x*4*cellSize), yPos, (mapSize * -0.5) + (z*4*cellSize));
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