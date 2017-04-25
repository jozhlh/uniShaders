// Voronoi Boomtown - UiManager.h
// Josh Hale - 2017
// An interface between the ImGui UI system and the application

#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include "../DXFramework/imgui-master\imgui.h"
#include "../DXFramework/imgui-master\imgui_impl_dx11.h"
#include "../DXFramework/baseapplication.h"

#include <vector>

// Slider parameters
#define MAX_REGIONS 11
#define MAX_CELLS 100
#define MAX_CELL_SIZE 15.0f
#define MAX_NOISE_RESOLUTION 100.0f
#define MAX_NOISE_HEIGHT 5.0f
#define MAX_SPECULARITY 100.0f

class UiManager
{
public:
	UiManager();
	~UiManager();

	void ShowUi(bool* p_open);
	void ShowHelpMarker(const char* desc);

	float frameRate;

	// Aesthetic Variables
	XMFLOAT4 ambientColour;
	XMFLOAT4 diffuseColour;
	XMFLOAT3 direction;
	XMFLOAT4 specularColour;
	float specularPower;
	bool lightChanged;

	XMFLOAT3 basePlateColour;
	XMFLOAT3 nodeColour;
	XMFLOAT3 centreOfRegionColour;
	XMFLOAT3 regionColour;
	bool identifyRegions;
	bool showNodes;
	float yOffset;

	// Algortihm Variables
	XMINT2 gridDimensions;
	float cellSize;
	float cellBorder;
	int numberOfRegions;

	// Noise Variables
	float noiseResolution;
	float noiseHeight;

	// Clumping Variable
	int clumpingDistance;
	bool generate;

	// Blur Variables
	int blurLoops;
	XMFLOAT3 blurWeightings;
};

#endif