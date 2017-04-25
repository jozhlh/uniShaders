// VoronoiBoomtown - AppBoomtown.h
// Josh Hale - 2017
// Application which generates a town using voronoi regiones on a noise terrain with clumped asset placement and Gaussian Blur

#ifndef _APPBOOMTOWN_H
#define _APPBOOMTOWN_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "../DXFramework/orthomesh.h"
#include "../DXFramework/rendertexture.h"
#include "../DXFramework/Timer.h"
#include "../DXFramework/Model.h"
#include "SpecularLightShader.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"
#include "TextureShader.h"
#include "UiManager.h"
#include "VoronoiMap.h"
#include "ModelBank.h"

class AppBoomtown : public BaseApplication
{
public:
	AppBoomtown();
	~AppBoomtown();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);

	// Logic Methods
	bool Frame();

protected:
	// Rendering Methods
	bool RenderToTexture();
	void DownSample(RenderTexture* sourceTexture);
	void HorizontalBlur();
	void VerticalBlur();
	void UpSample();
	bool Render();

private:
	// System Objects
	UiManager* m_UiManager;
	Timer* m_Timer;

	// Render Targets
	RenderTexture* m_RenderTexture;
	RenderTexture* m_downScaledTexture;
	RenderTexture* m_horizontalBlurTexture;
	RenderTexture* m_verticalBlurTexture;
	RenderTexture* m_blurredTexture;

	// Shader Objects
	SpecularLightShader* m_LightShader;
	SpecularTerrainShader* m_TerrainShader;
	TextureShader* m_TextureShader;
	VerticalBlurShader* m_VerticalBlurShader;
	HorizontalBlurShader* m_HorizontalBlurShader;

	// Geometry
	OrthoMesh* m_OrthoMesh;

	// Lights
	Light* m_Light;

	// Voronoi
	ModelBank* m_ModelBank;
	VoronoiMap* m_VoronoiMap;

	// Other variables
	XMFLOAT2 screenDimensions;
};

#endif