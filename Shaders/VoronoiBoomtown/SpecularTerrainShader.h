// Voronoi Boomtown - SpecularTerrainShader.h
// Josh Hale - 2017
// Loads a shader for a single specular light used to render terrain

#ifndef _SPECULARTERRAINSHADER_H_
#define _SPECULARTERRAINSHADER_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"
#include "../DXFramework/baseapplication.h"

using namespace std;
using namespace DirectX;


class SpecularTerrainShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT3 direction;
		float specularPower;
		XMFLOAT4 specularColor;
		int regionNum;
		XMFLOAT3 terrainBaseColor;
		bool identify;
		int numOfRegions;
		XMFLOAT2 padding;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

public:

	SpecularTerrainShader(ID3D11Device* device, HWND hwnd);
	~SpecularTerrainShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture,
		Light* light, XMFLOAT3 cameraPos, int regionNum, XMFLOAT3 terrainBaseColor, bool identify, int numOfRegions);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_cameraBuffer;
};

#endif