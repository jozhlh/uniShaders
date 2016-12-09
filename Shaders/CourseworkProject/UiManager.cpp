#include "UiManager.h"

UiManager::UiManager()
{

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

	if (ImGui::CollapsingHeader("Post Processing"))
	{
		if (ImGui::TreeNode("Some Effect 1"))
		{
			static bool selected[2] = { false, true};
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