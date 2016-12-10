#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "BaseMesh.h"


#define _USE_MATH_DEFINES

#include <math.h>

using namespace DirectX;

class Sphere : public BaseMesh
{

public:
	Sphere(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int resolution = 20);
	~Sphere();

protected:
	void InitBuffers(ID3D11Device* device);
	int m_resolution;

	/// PRIVATE FUNCTIONS

	XMFLOAT3 Calculate_Coordinates(float t, float d);
	XMFLOAT3 Normalise(XMFLOAT3 vec);


	/// PRIVATE VARIABLES

	//XMFLOAT3 vertices, normals, tex_coords;

	float radius, theta, delta;

	float latitude_angle, longitude_angle;

	//float x, y, z;

	XMFLOAT3 temp;
};

#endif