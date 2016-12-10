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
	bool Render();

private:
	UiManager* m_UiManager;
	SpecularLightShader* m_LightShader;
	TessellationShader* m_TessellationShader;
	TessellationMesh* m_testTesMesh;
	TessellationSphere* m_SphereMesh;
	PlaneMesh* m_PlaneMesh;
	Model* m_Spaceship;
	Light* m_PrimaryLight;
	Light* m_Light;
	Timer* m_Timer;
	float iterator = 0;
	float sphereLerp;


};

#endif