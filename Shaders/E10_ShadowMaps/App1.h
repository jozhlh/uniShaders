// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/orthomesh.h"
#include "../DXFramework/rendertexture.h"
#include "../DXFramework/spheremesh.h"
#include "../DXFramework/planemesh.h"
#include "../DXFramework/model.h"
#include "DepthShader.h"
#include "ShadowShader.h"
#include "SpecularLightShader.h"
#include "TextureShader.h"

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

private:
	SpecularLightShader* m_LightShader;
	TextureShader* m_TextureShader;
	DepthShader* m_DepthShader;
	ShadowShader* m_ShadowShader;
	RenderTexture* m_RenderTexture;
	OrthoMesh* m_OrthoMesh;
	Model* m_Model;
	SphereMesh* m_Mesh;
	PlaneMesh* m_PlaneMesh;
	Light* m_Light;
	float iterator = 0;


};

#endif