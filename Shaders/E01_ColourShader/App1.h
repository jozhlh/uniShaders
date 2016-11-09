// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/trianglemesh.h"
#include "doubletrianglemesh.h"
#include "ColourShader.h"

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
	ColourShader* m_ColourShader;
	TriangleMesh* m_Mesh;
	DoubleTriangleMesh* m_double_Mesh;

	
};

#endif