// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/tessellationsphere.h"
#include "../DXFramework/orthomesh.h"
#include "../DXFramework/rendertexture.h"
#include "../DXFramework/Timer.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"
#include "TextureShader.h"
#include "tessellationshader.h"
#include "UiManager.h"

#define ANIM_CAP 20.0f

class App1 : public BaseApplication
{
public:
	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);

	bool Frame();

protected:
	bool RenderToTexture();
	void DownSample(RenderTexture* sourceTexture);
	void HorizontalBlur();
	void VerticalBlur();
	void UpSample();
	bool Render();
	float SmootherStep(float edge0, float edge1, float x);

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
	TessellationShader* m_TessellationShader;
	TextureShader* m_TextureShader;
	VerticalBlurShader* m_VerticalBlurShader;
	HorizontalBlurShader* m_HorizontalBlurShader;

	// Geometry
	TessellationSphere* m_SphereMesh;
	OrthoMesh* m_OrthoMesh;

	// Other variables
	float sphereLerp;
	XMFLOAT2 screenDimensions;
};

#endif