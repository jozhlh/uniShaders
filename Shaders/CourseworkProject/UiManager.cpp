#include "UiManager.h"

UiManager::UiManager()
{
	tessellationSetup.edgeSplitting = XMINT3(1, 1, 1);
	tessellationSetup.innerSplitting = 1;

	tessellationWarp.baseColour = XMFLOAT3(0.4f, 1.0f, 0.3f);
	tessellationWarp.powers = 4;
	tessellationWarp.repeats = 10.0f;
	tessellationWarp.severity = 5.0f;
	tessellationWarp.lerpAmount = 1.0f;
	tessellationWarp.targetSin = true;

	playAnimation = true;
	animationSpeed = 10.0f;
	sphereSize = 5.0f;
	spherePosition = XMFLOAT3(0.0f, 0.0f, 30.0f);
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
	if (!ImGui::Begin("Shader Coursework UI", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::Spacing();

	///
	// Tessellation Controls
	///

	if (ImGui::CollapsingHeader("Tessellation"))
	{
		if (ImGui::TreeNode("Sphere Mesh"))
		{
			ImGui::DragFloat("Size of Base Mesh", &sphereSize, 0.1f, 0.5f, 200.f, "%.05f ns");
			if (ImGui::TreeNode("Position of Sphere"))
			{
				ImGui::DragFloat("X Position", &spherePosition.x, 1.0f, -200.f, 200.f, "%.5f ns");
				ImGui::DragFloat("Y Position", &spherePosition.y, 1.0f, -200.f, 200.f, "%.5f ns");
				ImGui::DragFloat("Z Position", &spherePosition.z, 1.0f, -200.f, 200.f, "%.5f ns");
				
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Amount of Tessellation"))
		{
			ImGui::DragInt("Edge Subdivisions", &tessellationSetup.edgeSplitting.x, 0.1, 1, 50);
			ImGui::DragInt("Inner Subdivisions", &tessellationSetup.innerSplitting, 0.1, 0, 100);
			
			tessellationSetup.edgeSplitting.y = tessellationSetup.edgeSplitting.x;
			tessellationSetup.edgeSplitting.z = tessellationSetup.edgeSplitting.x;

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Tessellation Warping"))
		{
			if (ImGui::TreeNode("Surface Warping"))
			{
				ImGui::DragInt("To What Order is The Algorithm", &tessellationWarp.powers, 0.5, 1, 50);
				ImGui::DragFloat("Frequency", &tessellationWarp.repeats, 0.1, 1, 100);
				ImGui::DragFloat("Intensity", &tessellationWarp.severity, 0.1, 1, 100);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Colour"))
			{
				float baseCol[3] = { tessellationWarp.baseColour.x, tessellationWarp.baseColour.y, tessellationWarp.baseColour.z };

				ImGui::ColorEdit3("Base Colour", baseCol);
				ImGui::SameLine(); ShowHelpMarker("Click on the colored square to change edit mode.\nCTRL+click on individual component to input value.\n");

				tessellationWarp.baseColour = XMFLOAT3(baseCol[0], baseCol[1], baseCol[2]);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Animation"))
			{
				ImGui::Checkbox("Play Animation", &playAnimation);
				ImGui::DragFloat("Lerp Between Cos Surface and Sin Surface", &tessellationWarp.lerpAmount, 0.01, 0.0f, 1.0f, "%.06f ns");
				ImGui::DragFloat("Animation Spees", &animationSpeed, 0.1, 1, 100);

				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

	}

	if (ImGui::CollapsingHeader("Post Processing"))
	{
		if (ImGui::TreeNode("Some Effect 1"))
		{
			static bool selected[2] = { false, true };
			ImGui::Selectable("1. I am selectable", &selected[0]);


			if (selected[0])
			{
				if (ImGui::TreeNode("Allow some changes to 1"))
				{
					// line 608 of imgui demo
					static int i1 = 50, i2 = 42;
					ImGui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%.0f%%");

					static float f1 = 1.00f, f2 = 0.0067f;
					ImGui::DragFloat("drag float", &f1, 0.005f);
					ImGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");

					ImGui::TreePop();
				}
			}

			ImGui::Selectable("2. I am selectable", &selected[1]);

			if (selected[1])
			{
				if (ImGui::TreeNode("Allow some changes to 2"))
				{
					static float col1[3] = { 1.0f,0.0f,0.2f };
					static float col2[4] = { 0.4f,0.7f,0.0f,0.5f };
					ImGui::ColorEdit3("color 1", col1);
					ImGui::SameLine(); ShowHelpMarker("Click on the colored square to change edit mode.\nCTRL+click on individual component to input value.\n");

					ImGui::ColorEdit4("color 2", col2);
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}

	ImGui::End();

}