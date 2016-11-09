// DoubleTriangleMesh.h
// Simple double triangle mesh.

#ifndef _DOUBLETRIMESH_H_
#define _DOUBLETRIMESH_H_

#include "../DXFramework/BaseMesh.h"

using namespace DirectX;

#define UNIT 2.0f

class DoubleTriangleMesh : public BaseMesh
{

public:
	DoubleTriangleMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename);
	~DoubleTriangleMesh();

protected:
	void InitBuffers(ID3D11Device* device);

};

#endif