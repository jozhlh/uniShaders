// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/spheremesh.h"
#include "SpecularLightShader.h"

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
	SpecularLightShader* m_LightShader;
	SphereMesh* m_Mesh;
	Light* m_Light;
	float iterator = 0;


};

#endif