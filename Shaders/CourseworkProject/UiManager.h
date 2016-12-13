#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include "../DXFramework/imgui-master\imgui.h"
#include "../DXFramework/imgui-master\imgui_impl_dx11.h"

#include "TessellationShader.h"

struct LightParameterType
{
	XMFLOAT4 ambientColour;
	XMFLOAT4 diffuseColour;
	XMFLOAT4 specularColour;
	XMFLOAT3 direction;
	XMFLOAT3 position;
	float specularPower;

};

class UiManager
{
public:
	UiManager();
	~UiManager();

	void ShowUi(bool* p_open);
	void ShowHelpMarker(const char* desc);

	float frameRate;

	TessellationSetupType tessellationSetup;
	TessellationWarpType tessellationWarp;
	LightParameterType primaryLight;
	bool playAnimation;
	float animationSpeed;
	float sphereSize;
	XMFLOAT3 spherePosition;
};

#endif