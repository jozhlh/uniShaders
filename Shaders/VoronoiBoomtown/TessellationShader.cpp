// tessellation shader.cpp
#include "tessellationshader.h"


TessellationShader::TessellationShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	InitShader(L"shaders/tessellation_vs.hlsl", L"shaders/tessellation_hs.hlsl", L"shaders/triangle_gs.hlsl", L"shaders/tessellation_noise_ds.hlsl", L"shaders/tessellation_ps.hlsl");
}


TessellationShader::~TessellationShader()
{
	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}
	// Release shader constant buffers
	if (m_tessellationSetupBuffer)
	{
		m_tessellationSetupBuffer->Release();
		m_tessellationSetupBuffer = 0;
	}
	if (m_tessellationWarpBuffer)
	{
		m_tessellationWarpBuffer->Release();
		m_tessellationWarpBuffer = 0;
	}
	if (m_geometryBuffer)
	{
		m_geometryBuffer->Release();
		m_geometryBuffer = 0;
	}
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}

void TessellationShader::InitShader(WCHAR* vsFilename,  WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	m_device->CreateSamplerState(&samplerDesc, &m_sampleState);
}

void TessellationShader::InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* gsFilename, WCHAR* dsFilename, WCHAR* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	InitShader(vsFilename, psFilename);

	D3D11_BUFFER_DESC tessellationSetupBufferDesc;
	D3D11_BUFFER_DESC tessellationWarpBufferDesc;
	D3D11_BUFFER_DESC geometryBufferDesc;

	// Load other required shaders.
	loadHullShader(hsFilename);
	loadDomainShader(dsFilename);
	loadGeometryShader(gsFilename);

	// Setup the description of the dynamic tessellation setup constant buffer that is in the hull shader.
	tessellationSetupBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tessellationSetupBufferDesc.ByteWidth = sizeof(TessellationSetupType);
	tessellationSetupBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tessellationSetupBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tessellationSetupBufferDesc.MiscFlags = 0;
	tessellationSetupBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the hull shader constant buffer from within this class.
	m_device->CreateBuffer(&tessellationSetupBufferDesc, NULL, &m_tessellationSetupBuffer);

	// Setup the description of the dynamic tessellation warp constant buffer that is in the hull shader.
	tessellationWarpBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tessellationWarpBufferDesc.ByteWidth = sizeof(TessellationWarpType);
	tessellationWarpBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tessellationWarpBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tessellationWarpBufferDesc.MiscFlags = 0;
	tessellationWarpBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the domain shader constant buffer from within this class.
	m_device->CreateBuffer(&tessellationWarpBufferDesc, NULL, &m_tessellationWarpBuffer);

	// Setup the description of the dynamic tessellation warp constant buffer that is in the hull shader.
	geometryBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	geometryBufferDesc.ByteWidth = sizeof(GeometryBufferType);
	geometryBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	geometryBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	geometryBufferDesc.MiscFlags = 0;
	geometryBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the domain shader constant buffer from within this class.
	m_device->CreateBuffer(&geometryBufferDesc, NULL, &m_geometryBuffer);
}


void TessellationShader::SetShaderParameters(
	ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture,
	TessellationSetupType setup, TessellationWarpType warp, float explode)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	TessellationSetupType* tessSetupPtr;
	TessellationWarpType* tessWarpPtr;
	GeometryBufferType* geometryPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;
	
	///
	// Set Matrix Buffer
	///

	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);
	
	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	
	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	deviceContext->DSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	deviceContext->GSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);

	///
	// Set Tessellation Setup Buffer
	///

	// Lock the tessellation setup constant buffer so it can be written to.
	deviceContext->Map(m_tessellationSetupBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer
	tessSetupPtr = (TessellationSetupType*)mappedResource.pData;

	// Copy tessellation setup data into the buffer
	tessSetupPtr->edgeSplitting = setup.edgeSplitting;
	tessSetupPtr->innerSplitting = setup.innerSplitting;

	// Unlock the buffer
	deviceContext->Unmap(m_tessellationSetupBuffer, 0);
	// Set buffer number (register value)
	bufferNumber = 1;
	// Set the constant buffer in the hull shader
	deviceContext->HSSetConstantBuffers(bufferNumber, 1, &m_tessellationSetupBuffer);

	///
	// Set Tessellation Warp Buffer
	///

	// Lock the tessellation setup constant buffer so it can be written to.
	deviceContext->Map(m_tessellationWarpBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer
	tessWarpPtr = (TessellationWarpType*)mappedResource.pData;

	// Copy tessellation setup data into the buffer
	tessWarpPtr->baseColour = warp.baseColour;
	tessWarpPtr->lerpAmount = warp.lerpAmount;
	tessWarpPtr->powers = warp.powers;
	tessWarpPtr->repeats = warp.repeats;
	tessWarpPtr->severity = warp.severity;
	tessWarpPtr->targetSin = warp.targetSin;
	
	// Unlock the buffer
	deviceContext->Unmap(m_tessellationWarpBuffer, 0);
	// Set buffer number (register value)
	bufferNumber = 2;
	// Set the constant buffer in the domain shader
	deviceContext->DSSetConstantBuffers(bufferNumber, 1, &m_tessellationWarpBuffer);

	///
	// Set Geometry Buffer
	///

	// Lock the tessellation setup constant buffer so it can be written to.
	deviceContext->Map(m_geometryBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer
	geometryPtr = (GeometryBufferType*)mappedResource.pData;

	// Copy tessellation setup data into the buffer
	geometryPtr->explode = explode;
	geometryPtr->padding = { 1.0f, 1.0f, 1.0f };

	// Unlock the buffer
	deviceContext->Unmap(m_geometryBuffer, 0);
	// Set buffer number (register value)
	bufferNumber = 1;
	// Set the constant buffer in the domain shader
	deviceContext->GSSetConstantBuffers(bufferNumber, 1, &m_geometryBuffer);
}

void TessellationShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	
	// Base render function.
	BaseShader::Render(deviceContext, indexCount);
}



