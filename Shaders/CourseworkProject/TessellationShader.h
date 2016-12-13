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
};

struct GeometryBufferType
{
	float explode;
	XMFLOAT3 padding;
};

class TessellationShader : public BaseShader
{

public:

	TessellationShader(ID3D11Device* device, HWND hwnd);
	~TessellationShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture,
		TessellationSetupType setup, TessellationWarpType warp, float explode);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
	void InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* gsFilename, WCHAR* dsFilename, WCHAR* psFilename);

	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_tessellationSetupBuffer;
	ID3D11Buffer* m_tessellationWarpBuffer;
	ID3D11Buffer* m_geometryBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif