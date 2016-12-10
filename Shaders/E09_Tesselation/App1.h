// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/TessellationMesh.h"
#include "../DXFramework/TessellationSphere.h"
#include "../DXFramework/spheremesh.h"
#include "../DXFramework/planemesh.h"
#include "../DXFramework/Timer.h"
#include "../DXFramework/Input.h"
#include "../DXFramework/imgui-master/imgui.h"
#include "../DXFramework/imgui-master/imgui_impl_dx11.h"
#include "LightShader.h"
#include "TessellationShader.h"

#define MINFREQUENCY 0.1f
#define MINHEIGHT 0.1f

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);

	bool Frame();

protected:
	bool Render();

private:
	LightShader* m_LightShader;
	TessellationShader* m_TessellationShader;
	TessellationMesh* m_Mesh;
	PlaneMesh* m_Plane;
	Light* m_Light;
	Timer* m_Timer;
	Input* m_Input;
	float deltaTime = 0;
	float iterator = 0;
	float height = 1.0f;
	float frequency = 1.0f;


};

#endif