// TriangleMesh.h
// Simple single triangle mesh (for example purposes).

#ifndef _TRIMESH_H_
#define _TRIMESH_H_

#include "BaseMesh.h"

using namespace DirectX;

#define UNIT 2.0f

class TriangleMesh : public BaseMesh
{

public:
	TriangleMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename);
	~TriangleMesh();

protected:
	void InitBuffers(ID3D11Device* device);
	
};

#endif