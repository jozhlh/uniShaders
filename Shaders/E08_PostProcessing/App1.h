// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/spheremesh.h"
#include "../DXFramework/planemesh.h"
#include "../DXFramework/rendertexture.h"
#include "../DXFramework/orthomesh.h"
#include "../DXFramework/Timer.h"
#include "../DXFramework/Input.h"
#include "../DXFramework/imgui-master/imgui.h"
#include "../DXFramework/imgui-master/imgui_impl_dx11.h"
#include "LightShader.h"
#include "TextureShader.h"

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
	void RenderToTexture();
	void RenderScene();
	void DownSample();
	void HorizontalBlur();
	void VerticalBlur();
	void UpSample();

private:
	LightShader* m_LightShader;
	TextureShader* m_TextureShader;
	SphereMesh* m_Mesh;
	PlaneMesh* m_Plane;
	RenderTexture* m_RenderTexture;
	OrthoMesh* m_OrthoMesh;
	Light* m_Light;
	Timer* m_Timer;
	Input* m_Input;
	float deltaTime = 0;
	float iterator = 0;
	float height = 1.0f;
	float frequency = 1.0f;


};

#endif