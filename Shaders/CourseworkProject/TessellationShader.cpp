// tessellation shader.cpp
#include "tessellationshader.h"


TessellationShader::TessellationShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	InitShader(L"shaders/tessellation_vs.hlsl", L"shaders/tessellation_hs.hlsl", L"shaders/tessellation_ds.hlsl", L"shaders/tessellation_ps.hlsl");
}


TessellationShader::~TessellationShader()
{
	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the matrix constant buffer.
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

	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = 0;
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

	// Required a CLAMPED sampler for sampling the depth map
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	// Create the texture sampler state.
	m_device->CreateSamplerState(&samplerDesc, &m_sampleStateClamp);

	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);

	// Setup camera buffer
	// Setup the description of the camera dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
}

void TessellationShader::InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	InitShader(vsFilename, psFilename);

	D3D11_BUFFER_DESC tessellationSetupBufferDesc;
	D3D11_BUFFER_DESC tessellationWarpBufferDesc;

	// Load other required shaders.
	loadHullShader(hsFilename);
	loadDomainShader(dsFilename);

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
}


void TessellationShader::SetShaderParameters(
	ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture,
	TessellationSetupType setup, ID3D11ShaderResourceView* depthMap, TessellationWarpType warp, Light* light, XMFLOAT3 cam)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	TessellationSetupType* tessSetupPtr;
	TessellationWarpType* tessWarpPtr;
	LightBufferType* lightPtr;
	CameraBufferType* cameraPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj, tLightViewMatrix, tLightProjectionMatrix;

	///
	// Set Matrix Buffer
	///

	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);
	tLightViewMatrix = XMMatrixTranspose(light->GetViewMatrix());
	tLightProjectionMatrix = XMMatrixTranspose(light->GetOrthographicMatrix());

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	dataPtr->lightView = tLightViewMatrix;
	dataPtr->lightProjection = tLightProjectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	deviceContext->DSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	// Set shader depth map texture resource.
	deviceContext->PSSetShaderResources(1, 1, &depthMap);
	///
	// Set Light Buffer
	///

	// Send light data to pixel shader
	deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	lightPtr->ambient = light->GetAmbientColour();
	lightPtr->diffuse = light->GetDiffuseColour();
	lightPtr->direction = light->GetDirection();
	lightPtr->specularPower = light->GetSpecularPower();
	lightPtr->specularColor = light->GetSpecularColour();
	lightPtr->position = light->GetPosition();
	lightPtr->padding = 1.0f;
	deviceContext->Unmap(m_lightBuffer, 0);
	bufferNumber = 4;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	///
	// Set Camera Buffer
	///

	// Send camera data to pixel shader
	deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	cameraPtr = (CameraBufferType*)mappedResource.pData;
	cameraPtr->lightPosition = light->GetPosition();
	cameraPtr->cameraPosition = cam;
	cameraPtr->padding = 0.0f;
	cameraPtr->padding1 = 0.0f;
	deviceContext->Unmap(m_cameraBuffer, 0);
	bufferNumber = 3;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);

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
	tessWarpPtr->lightPosition = light->GetPosition();
	tessWarpPtr->padding = 1.0f;

	// Unlock the buffer
	deviceContext->Unmap(m_tessellationWarpBuffer, 0);
	// Set buffer number (register value)
	bufferNumber = 2;
	// Set the constant buffer in the domain shader
	deviceContext->DSSetConstantBuffers(bufferNumber, 1, &m_tessellationWarpBuffer);
}

void TessellationShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	//deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	deviceContext->PSSetSamplers(1, 1, &m_sampleStateClamp);

	// Base render function.
	BaseShader::Render(deviceContext, indexCount);
}



