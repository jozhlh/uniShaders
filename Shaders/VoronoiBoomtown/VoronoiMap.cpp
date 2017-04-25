// Voronoi Boomtown - VoronoiMap.h
// Josh Hale - 2017
// Creates a grid, split into regions which are sorted and have models placed in their centres
// References: mersenne twister engine random number generator implementation https://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful

#include "VoronoiMap.h"

// Constructor - Stores calculation variables and runs voronoi region algorithm
/// PARAMETERS
// device, deviceContext - pointers to the D3D device
// bank - pointer to the model data
// dimensions - grid dimensions
// border - gep between cell mesh and cell border for gridlines
// noiseHeight, noiseResolution - noise parameters
// size - size of a cell
// numberOfRegions - how many nodes to create for voronoi algorithm
// baseColour - colour of the base plate cube
VoronoiMap::VoronoiMap(ID3D11Device *device, ID3D11DeviceContext *deviceContext, ModelBank* bank,
	XMINT2 dimensions, float border, float noiseHeight, float noiseResolution, float size, float numberOfRegions, XMFLOAT3 baseColour)
{
	// Initialise Mersenne Twister engine for random number generation
	random_device rd;
	mt19937 mt(rd());

	uniform_int_distribution<int> noiseLocationSelection(1, 100);
	uniform_int_distribution<int> minorAssetSelection(1, MINOR_BUILDINGS - 1);

	// Set calculation variables
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

	// Load texture shared between models
	stockTexture = new Texture(device, deviceContext, L"../res/palette.png");
	basePlate = new CubeMesh(device, deviceContext, L"../res/palette.png", 2);

	// Split grid into regions
	GenerateRegions(numberOfRegions);
	AssignCellsToRegions(device, deviceContext);
	
	// Initialise each region and calculate it's centre of mass
	int iterator = 0;
	for each (Region* region in regions)
	{
		region->Init(device, deviceContext, iterator);
		region->CalculateCentre(cellSize*cellSize);
		iterator++;
	}

	// Sort regions in the list by size order
	SortRegionsBySize();
	int r = 0.0f;
	// Place buildings on regions accordind to size
	for each (Region* region in regions)
	{
		region->AssignMajorBuilding(&modelBank->majorBuildings[(int)r], cellSize, zCells);
		region->PlaceDerrick(&modelBank->minorBuildings[0], cellSize);
		region->PlaceMinorAsset(&modelBank->minorBuildings[minorAssetSelection(mt)], cellSize);

		// Place an additional asset in each region than the regions smaller than it
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

// Destructor
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

// Render all cells, render all region's assets, render base plate cube
/// PARAMETERS
// deviceContext - pointer to D3D device data
// world, view, projection - spatial matrices for positioning
// shader, terrrainShader - pointers to shaders used for models and cells
// light - pointer to light colour and direction information
// cameraPosition - location of camera for specular calculations
// nodeColour, centreColour, regionColour, baseColour - colours set by the user in the UI
// showNodes - prevents models from being rendered and renders the voronoi node and centre of the region markers
// identifyRegions - replaces cell mesh texture with a gradient indicating order of region size
// yOff - distance from the cells the models are rendered (to prevent hovering)
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

	// Render base plate cube
	XMMATRIX worldMatrix = XMMatrixMultiply(XMMatrixScaling((cellSize * 0.5f * xCells) + 0.2f, 0.25f, (cellSize * 0.5f * zCells) + 0.2f), XMMatrixTranslation(0, -0.75f, 0));
	//// Send geometry data (from mesh)
	basePlate->SendData(deviceContext);
	//// Set shader parameters (matrices and texture)
	shader->SetShaderParameters(deviceContext, worldMatrix, view, projection, basePlate->GetTexture(), light, cameraPosition, true, baseColour);
	//// Render object (combination of mesh geometry and shader process
	shader->Render(deviceContext, basePlate->GetIndexCount());
}

// Chooses random points on the grid for voronoi nodes to be placed and creates a region for each
/// PARAMETERS
// num - how many nodes to place
void VoronoiMap::GenerateRegions(int num)
{
	xWidth = xCells * cellSize;
	zWidth = zCells * cellSize;

	// seed:rd() <- can set as an actual seed... this is a non-deterministic 32-bit seed
	random_device rd;
	mt19937 mt(rd());
	
	uniform_int_distribution<int> xDist(1, (xCells *cellSize) - 1);
	uniform_int_distribution<int> zDist(1, (zCells *cellSize) - 1);

	for (int r = 0; r < num; r++)
	{
		Region* m_Region = new Region();
		m_Region->SetNodeCoordinates(xDist(mt)-(0.5* xWidth), zDist(mt) - (0.5* zWidth));
		regions.push_back(m_Region);
	}
}

// Calculates the closest region node to each cell and creates a cell in that region
/// PARAMETERS
// device, deviceContext - pointers to the D3D device
void VoronoiMap::AssignCellsToRegions(ID3D11Device *device, ID3D11DeviceContext *deviceContext)
{
	// Create a cell with procedurally generated mesh for each space in the grid
	for (int x = 0; x < xCells; x++)
	{
		for (int z = 0; z < zCells; z++)
		{
			Cell* m_Cell = new Cell(device, deviceContext, x, z, xCells, zCells, &noise, cellSize, cellBorder, noiseEngine);;
			m_Cell->SetCoordinates((xWidth * -0.5) + ((cellSize*0.5f) + (x*cellSize)), yPos, (zWidth * -0.5) + ((cellSize*0.5f) + (z*cellSize)));
			unassignedCells.push_back(m_Cell);
		}
	}

	// Place cells in regions based on distance
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

// Returns distance between a cell and a region
/// PARAMETERS
// cell, region - the cell to be tested against the region
/// OUTPUT
// float - distance between the cell and the region's node
float VoronoiMap::RegionDistance(Cell* cell, Region* region)
{
	XMFLOAT2 regionNode = region->GetNode();
	XMFLOAT3 cellPosition = cell->GetCoordinates();

	float xComponent = regionNode.x - cellPosition.x;
	float yComponent = regionNode.y - cellPosition.z;
	return sqrtf((xComponent * xComponent) + (yComponent * yComponent));
}

// Orders the list of regions by size
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

	// Give each cell it's new parent ID according to sorted order
	int order = 0;
	for each (Region* region in regions)
	{
		region->SetCellIDs(order);
		order++;
	}
}