// VoronoiBoomtown -  VerticalBlurShader.h
// Josh Hale - 2017
// Loads vertical blur shaders (vs and ps) and passes screen height to shaders, for sample coordinate calculation

#ifndef _VERTICALBLURSHADER_H_
#define _VERTICALBLURSHADER_H_

#include "../DXFramework/BaseShader.h"

using namespace std;
using namespace DirectX;


class VerticalBlurShader : public BaseShader
{
private:
	struct ScreenSizeBufferType
	{
		float screenHeight;
		XMFLOAT3 blurWeights;
	};

public:

	VerticalBlurShader(ID3D11Device* device, HWND hwnd);
	~VerticalBlurShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float width, XMFLOAT3 blurWeights);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_ScreenSizeBuffer;
};

#endif