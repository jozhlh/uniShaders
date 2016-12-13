// Light shader.h
// Basic single light shader setup
#ifndef _TESSELLATIONSHADER_H_
#define _TESSELLATIONSHADER_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"

using namespace std;
using namespace DirectX;

struct TessellationSetupType
{
	XMINT3 edgeSplitting;
	int innerSplitting;
};

struct TessellationWarpType
{
	int powers;
	float repeats;
	float severity;
	float lerpAmount;
	XMFLOAT3 baseColour;
	bool targetSin;
	XMFLOAT3 lightPosition;
	float padding;
};

struct LightBufferType
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT3 direction;
	float specularPower;
	XMFLOAT4 specularColor;
	XMFLOAT3 position;
	float padding;
};

struct CameraBufferType
{
	XMFLOAT3 lightPosition;
	float padding1;
	XMFLOAT3 cameraPosition;
	float padding;
};

class TessellationShader : public BaseShader
{

public:

	TessellationShader(ID3D11Device* device, HWND hwnd);
	~TessellationShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture,
		TessellationSetupType setup, ID3D11ShaderResourceView* depthMap, TessellationWarpType warp, Light* light, XMFLOAT3 cam);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
	void InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);

	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_tessellationSetupBuffer;
	ID3D11Buffer* m_tessellationWarpBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* m_sampleStateClamp;
};

#endif