// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/tessellationsphere.h"
#include "../DXFramework/tessellationmesh.h"
#include "../DXFramework/orthomesh.h"
#include "../DXFramework/rendertexture.h"
#include "../DXFramework/spheremesh.h"
#include "../DXFramework/planemesh.h"
#include "../DXFramework/model.h"
#include "../DXFramework/Sphere.h"
#include "../DXFramework/Timer.h"
#include "DepthShader.h"
#include "ShadowShader.h"
#include "SpecularLightShader.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"
#include "GeometryShader.h"
#include "TextureShader.h"
#include "tessellationshader.h"
#include "UiManager.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);

	bool Frame();

protected:
	//void RenderLightToShadowMap(Light* light, RenderTexture* shadowMap);
	//void RenderDepth();
	bool RenderToTexture();
	void DownSample(RenderTexture* sourceTexture);
	void HorizontalBlur();
	void VerticalBlur();
	void UpSample();
	bool Render();
	void GaussianBlur(RenderTexture* sourceTexture);

private:
	//void SetLightParameters(Light* light, LightParameterType params);

	// System Objects
	UiManager* m_UiManager;
	Timer* m_Timer;
	RenderTexture* m_RenderTexture;
	//RenderTexture* m_DepthTexture;
	//RenderTexture* m_PrimaryShadowMap;
	RenderTexture* m_downScaledTexture;
	RenderTexture* m_horizontalBlurTexture;
	RenderTexture* m_verticalBlurTexture;
	RenderTexture* m_blurredTexture;

	// Shader Controllers
	SpecularLightShader* m_LightShader;
	TessellationShader* m_TessellationShader;
	DepthShader* m_TessellationDepthShader;
	TextureShader* m_TextureShader;
	DepthShader* m_DepthShader;
	ShadowShader* m_ShadowShader;
	VerticalBlurShader* m_VerticalBlurShader;
	HorizontalBlurShader* m_HorizontalBlurShader;
	GeometryShader* m_GeometryShader;

	// Geometry
	//TessellationMesh* m_testTesMesh;
	TessellationSphere* m_SphereMesh;
	OrthoMesh* m_OrthoMesh;
	PlaneMesh* m_PlaneMesh;
	//Model* m_Spaceship;

	// Lights
	//Light* m_PrimaryLight;
	//Light* m_Light;




	// Other variables
	//float iterator = 0;
	float sphereLerp;
	XMFLOAT2 screenDimensions;

};

#endif