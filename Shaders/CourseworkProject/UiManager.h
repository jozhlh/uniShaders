#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include "../DXFramework/imgui-master\imgui.h"
#include "../DXFramework/imgui-master\imgui_impl_dx11.h"

#include "TessellationShader.h"

class UiManager
{
public:
	UiManager();
	~UiManager();

	void ShowUi(bool* p_open);
	void ShowHelpMarker(const char* desc);

	TessellationSetupType tessellationSetup;
	TessellationWarpType tessellationWarp;
	bool playAnimation;
	float animationSpeed;
	float sphereSize;
	XMFLOAT3 spherePosition;

};

#endif