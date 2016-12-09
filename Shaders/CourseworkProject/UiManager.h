#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include "../DXFramework/imgui-master\imgui.h"
#include "../DXFramework/imgui-master\imgui_impl_dx11.h"

class UiManager
{
public:
	UiManager();
	~UiManager();

	void ShowUi(bool* p_open);
	void ShowHelpMarker(const char* desc);


};

#endif