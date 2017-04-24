#include "UiManager.h"

UiManager::UiManager()
{
	// Initialise all UI variables
	regionColour = XMFLOAT3(1.0f, 0.95f, 0.6f);
	identifyRegions = false;

	// Aesthetic Variables
	ambientColour = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	diffuseColour = XMFLOAT4(0.9f, 0.8f, 0.3f, 1.0f);
	direction = XMFLOAT3(0.5f, -0.5f, 0.0f);
	specularColour = XMFLOAT4(1.0f, 1.0f, 0.6f, 1.0f);
	specularPower = 50.0f;
	lightChanged = false;
	showNodes = false;
	nodeColour = XMFLOAT3(0.7f, 0.98f, 1.0f);
	centreOfRegionColour = XMFLOAT3(0.8f, 0.4f, 0.64f);
	basePlateColour = XMFLOAT3(0.2f, 0.09, 0.06);
	float yOffset = 0.5f;

	// Algortihm Variables
	gridDimensions = XMINT2 (30, 30);
	cellSize = 1.0f;
	noiseResolution = 7.5f;
	noiseHeight = 0.6f;
	cellBorder = 0.05f;
	numberOfRegions = 10;
	clumpingDistance = 2;
	generate = false;

	// Blur Variables
	blurLoops = 0;
	blurWeightings.x = 0.4062f;
	blurWeightings.y = 0.2442f;
	blurWeightings.z = 0.0545f;
}

UiManager::~UiManager()
{

}


void UiManager::ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void UiManager::ShowUi(bool* p_open)
{
	ImGuiWindowFlags window_flags = 0;
	ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Voronoi Boomtown Controls", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::Spacing();

	///
	// fps
	///

	ImGui::Text("Framerate: %.1f fps", frameRate);
	
	ImGui::Spacing();

	///
	// Algorithm Variables
	///

	if (ImGui::CollapsingHeader("Procedural Generation"))
	{
		if (ImGui::TreeNode("Voronoi Algorithm"))
		{
			ImGui::DragInt("Grid X Dimensions", &gridDimensions.x, 0.1, 1, MAX_CELLS);
			ImGui::DragInt("Grid Z Dimensions", &gridDimensions.y, 0.1, 1, MAX_CELLS);
			ImGui::DragFloat("Cell Size", &cellSize, 0.01f, 0.01f, MAX_CELL_SIZE, "%.1f ns");
			ImGui::DragFloat("Border Size", &cellBorder, 0.01f, 0.01f, cellSize * 0.5f, "%.1f ns");
			ImGui::DragInt("Number Of Regions", &numberOfRegions, 0.1, 1, MAX_REGIONS);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Improved Noise Terrain"))
		{
			ImGui::DragFloat("Noise Resolution", &noiseResolution, 0.1f, 0.01f, MAX_NOISE_RESOLUTION, "%.1f ns");
			ImGui::DragFloat("Noise Height", &noiseHeight, 0.01f, 0.01f, MAX_NOISE_HEIGHT, "%.01f ns");
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Asset Placement Clumping"))
		{
			ImGui::DragInt("Clumping Distance", &clumpingDistance, 0.01, 1, MAX_CELLS);
			ImGui::TreePop();
		}
		ImGui::Checkbox("Generate", &generate);
	}

	///
	// Aesthetic Variables
	///

	if (ImGui::CollapsingHeader("Visuals"))
	{
		if (ImGui::TreeNode("Light"))
		{
			float ambCol[4] = { ambientColour.x, ambientColour.y, ambientColour.z, ambientColour.w };
			float difCol[4] = { diffuseColour.x, diffuseColour.y, diffuseColour.z, diffuseColour.w };
			float speCol[4] = { specularColour.x, specularColour.y, specularColour.z, specularColour.w };
			float dir[3] = { direction.x, direction.y, direction.z };

			if (ImGui::ColorEdit4("Ambient Colour", ambCol))
			{
				lightChanged = true;
			}
			ImGui::SameLine(); ShowHelpMarker("Click on the colored square to change edit mode.\nCTRL+click on individual component to input value.\n");
			if(ImGui::ColorEdit4("Diffuse Colour", difCol))
			{
				lightChanged = true;
			}
			if(ImGui::SliderFloat3("Diffuse Direction", dir, -1.0f, 1.0f, "%.1f"))
			{
				lightChanged = true;
			}
			if(ImGui::ColorEdit4("Specular Colour", speCol))
			{
				lightChanged = true;
			}
			if (ImGui::DragFloat("SpecularPower", &specularPower, 0.1f, 0.0f, MAX_SPECULARITY, "%.5f ns"))
			{
				lightChanged = true;
			}
			
			ambientColour = XMFLOAT4(ambCol[0], ambCol[1], ambCol[2], ambCol[3]);
			diffuseColour = XMFLOAT4(difCol[0], difCol[1], difCol[2], difCol[3]);
			specularColour = XMFLOAT4(speCol[0], speCol[1], speCol[2], speCol[3]);
				
			direction = XMFLOAT3(dir[0], dir[1], dir[2]);
			// need to normalise direction

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Region Visualisation"))
		{
			float bCol[3] = { basePlateColour.x, basePlateColour.y, basePlateColour.z };
			ImGui::ColorEdit3("Base Plate Colour", bCol);
			basePlateColour = XMFLOAT3(bCol[0], bCol[1], bCol[2]);

			ImGui::Checkbox("Show Nodes", &showNodes);
			if (showNodes)
			{
				ImGui::DragFloat("Node Offest", &yOffset, 0.01f, -0.5f, MAX_NOISE_HEIGHT, "%.1f");

				float nodeCol[3] = { nodeColour.x, nodeColour.y, nodeColour.z };
				float centCol[3] = { centreOfRegionColour.x, centreOfRegionColour.y, centreOfRegionColour.z };

				ImGui::ColorEdit3("Voronoi Node Colour", nodeCol);
				ImGui::ColorEdit3("Region Centre Colour", centCol);

				nodeColour = XMFLOAT3(nodeCol[0], nodeCol[1], nodeCol[2]);
				centreOfRegionColour = XMFLOAT3(centCol[0], centCol[1], centCol[2]);
			}

			float regCol[3] = { regionColour.x, regionColour.y, regionColour.z };
			ImGui::ColorEdit3("Base Colour", regCol);
			regionColour = XMFLOAT3(regCol[0], regCol[1], regCol[2]);

			ImGui::Checkbox("Identify Regions", &identifyRegions);

			ImGui::TreePop();
		}
	}

	///
	// Post Processing Controls
	///

	if (ImGui::CollapsingHeader("Post Processing"))
	{
		if (ImGui::TreeNode("Gaussian Blur"))
		{
			ImGui::DragInt("Blur Cycles", &blurLoops, 0.5f, 0, 20);

			float bw[3] = { blurWeightings.x, blurWeightings.y, blurWeightings.z };
			ImGui::DragFloat("Weighting of self", &bw[0], 0.001f, 0.0f, 1.0f, "%.04f ns");
			ImGui::DragFloat("Weighting of neighbour 1", &bw[1], 0.001f, 0.0f, 1.0f, "%.04f ns");
			ImGui::DragFloat("Weighting of neighbour 2", &bw[2], 0.001f, 0.0f, 1.0f, "%.04f ns");
			blurWeightings = XMFLOAT3(bw[0], bw[1], bw[2]);

			ImGui::TreePop();
		}
	}

	ImGui::End();

}