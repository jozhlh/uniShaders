// Tessellation Mesh, point list for tessellation
#ifndef _TESSELLATIONSPHERE_H_
#define _TESSELLATIONSPHERE_H_

#include "BaseMesh.h"

using namespace DirectX;

class TessellationSphere : public BaseMesh
{

public:
	TessellationSphere(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, float res);
	~TessellationSphere();

	void SendData(ID3D11DeviceContext*);

protected:
	void InitBuffers(ID3D11Device* device);
	int m_resolution;
};

#endif