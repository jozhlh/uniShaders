// Lab1.cpp
// Lab 1 example, simple textured quad
#include "App1.h"

App1::App1()
{
	m_SphereMesh = nullptr;
	m_PlaneMesh = nullptr;
	m_LightShader = nullptr;
	m_Light = nullptr;

	m_UiManager = nullptr;
	m_Timer = nullptr;
	m_RenderTexture = nullptr;
	m_DepthTexture = nullptr;
	m_PrimaryShadowMap = nullptr;

	// Shader Controllers
	m_TessellationShader = nullptr;
	m_TessellationDepthShader = nullptr;
	m_TextureShader = nullptr;
	m_DepthShader = nullptr;
	m_ShadowShader = nullptr;

	// Geometry
	m_testTesMesh = nullptr;
	m_SphereMesh = nullptr;
	m_OrthoMesh = nullptr;
	m_PlaneMesh = nullptr;
	m_Spaceship = nullptr;

	// Lights
	m_PrimaryLight = nullptr;
	m_Light = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	const int SHADOWMAP_WIDTH = 1024;
	const int SHADOWMAP_HEIGHT = 1024;

	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create UI Object
	m_UiManager = new UiManager;

	// Create Mesh objects with checkerboard texture
	m_SphereMesh = new TessellationSphere(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/checkerboard.png", 20.0f);
	m_PlaneMesh = new PlaneMesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/checkerboard.png");
	m_Spaceship = new Model(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/checkerboard.png", L"../res/smallaxe.obj");
	//m_testTesMesh = new TessellationMesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/checkerboard.png");

	// Create light shader object
	m_LightShader = new SpecularLightShader(m_Direct3D->GetDevice(), hwnd);

	// Create Tesselation Shader Object
	m_TessellationShader = new TessellationShader(m_Direct3D->GetDevice(), hwnd);

	m_TessellationDepthShader = new DepthShader(m_Direct3D->GetDevice(), hwnd);

	m_ShadowShader = new ShadowShader(m_Direct3D->GetDevice(), hwnd);

	// Create Light object
	m_Light = new Light;

	// Initialise light properties
	m_Light->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.5f, -0.5f, 0.0f);
	m_Light->SetPosition(50.0f, 5.0f, 50.0f);
	m_Light->SetSpecularPower(10.0f);
	m_Light->SetSpecularColour(0.0f, 0.0f, 1.0f, 1.0f);

	m_PrimaryLight = new Light;
	m_PrimaryLight->SetAmbientColour(0.1f, 0.0f, 0.0f, 1.0f);
	m_PrimaryLight->SetDiffuseColour(1.0f, 1.0f, 0.0f, 1.0f);
	m_PrimaryLight->SetDirection(0.5f, -0.5f, 0.0f);
	m_PrimaryLight->SetSpecularPower(25.0f);
	m_PrimaryLight->SetSpecularColour(1.0f, 0.0f, 0.0f, 1.0f);

	// RenderTexture, OrthoMesh and shader set for different renderTarget
	m_DepthTexture = new RenderTexture(m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);
	m_RenderTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	m_PrimaryShadowMap = new RenderTexture(m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);
	// ortho size and position set based on window size
	// 400x400 pixels (standard would be matching window size for fullscreen mesh
	// Position default at 0x0 centre window, to offset change values (pixel)
	m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 0, 0);
	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);

	m_Timer = new Timer;

	sphereLerp = m_UiManager->animationSpeed;

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the UI object.
	if (m_UiManager)
	{
		delete m_UiManager;
		m_UiManager = 0;
	}

	// Release the Direct3D object.
	if (m_SphereMesh)
	{
		delete m_SphereMesh;
		m_SphereMesh = 0;
	}
	if (m_testTesMesh)
	{
		delete m_testTesMesh;
		m_testTesMesh = 0;
	}

	// Release the Direct3D object.
	if (m_PlaneMesh)
	{
		delete m_PlaneMesh;
		m_PlaneMesh = 0;
	}

	if (m_Spaceship)
	{
		delete m_Spaceship;
		m_Spaceship = 0;
	}

	// Release the shader
	if (m_LightShader)
	{
		delete m_LightShader;
		m_LightShader = 0;
	}
	if (m_TessellationShader)
	{
		delete m_TessellationShader;
		m_TessellationShader = 0;
	}

	// Release the Light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (m_PrimaryLight)
	{
		delete m_PrimaryLight;
		m_PrimaryLight = 0;
	}

	if (m_TextureShader)
	{
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_OrthoMesh)
	{
		delete m_OrthoMesh;
		m_OrthoMesh = 0;
	}

	if (m_RenderTexture)
	{
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	if (m_DepthTexture)
	{
		delete m_DepthTexture;
		m_DepthTexture = 0;
	}

	if (m_DepthShader)
	{
		delete m_DepthShader;
		m_DepthShader = 0;
	}

	if (m_ShadowShader)
	{
		delete m_ShadowShader;
		m_ShadowShader = 0;
	}

	if (m_PrimaryShadowMap)
	{
		delete m_PrimaryShadowMap;
		m_PrimaryShadowMap = 0;
	}
}


bool App1::Frame()
{
	m_Timer->Frame();
	float dt = m_Timer->GetTime();
	m_UiManager->frameRate = 1.0f / dt;
	float animCap = 20.0f;

	bool result;

	result = BaseApplication::Frame();

	if (result)
	{
		SetLightParameters(m_PrimaryLight, m_UiManager->primaryLight);
		//m_PrimaryLight->SetPosition(-30.0f, 0.0f, 30.0f);
		//m_PrimaryLight->SetLookAt(1.0f, 0.0f, 0.0f);

		sphereLerp = m_UiManager->tessellationWarp.lerpAmount * animCap;
		if (m_UiManager->playAnimation)
		{
			if (m_UiManager->tessellationWarp.targetSin)
			{
				if (sphereLerp < animCap)
				{
					sphereLerp += (dt * m_UiManager->animationSpeed);
				}
				else if (m_UiManager->playAnimation)
				{
					m_UiManager->tessellationWarp.targetSin = false;
				}
			}
			else
			{
				if (sphereLerp > 0.0f)
				{
					sphereLerp -= (dt * m_UiManager->animationSpeed);
				}
				else if (m_UiManager->playAnimation)
				{
					m_UiManager->tessellationWarp.targetSin = true;
				}
			}
		}
		
		// Lerp between surface modifiers
		m_UiManager->tessellationWarp.lerpAmount = sphereLerp / animCap;


		bool show_test_window = true;
		//ImGui::ShowTestWindow(&show_test_window);
		m_UiManager->ShowUi(&show_test_window);
	}
	else
	{
		return false;
	}

	// Render the graphics.
	RenderLightToShadowMap(m_PrimaryLight, m_DepthTexture);
	//RenderDepth();
	RenderToTexture();
	Render();
	//RenderDepth();
	/*result = Render();
	if (!result)
	{
		return false;
	}*/
	

	return true;
}

void App1::RenderLightToShadowMap(Light* light, RenderTexture* shadowMap)
{
	XMMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix, orthoMatrix;

	// Set the render target to be the render to texture.
	shadowMap->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture. use the RGBA value to set background of texture // BLACK
	shadowMap->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	//m_Camera->Update();
	//m_Camera->GetViewMatrix(lightViewMatrix);
	light->SetLookAt(0, 0, 30);
	light->GenerateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);

	//// Get the world, view, projection, and ortho matrices from the light's perspective
	light->GenerateViewMatrix();
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	//light->GenerateProjectionMatrix(SCREEN_NEAR, SCREEN_DEPTH);
	//lightViewMatrix = m_Light->GetViewMatrix();
	lightProjectionMatrix = light->GetProjectionMatrix();
	m_Direct3D->GetWorldMatrix(worldMatrix);


	// Translate sphere mesh
	worldMatrix = XMMatrixScaling(m_UiManager->sphereSize, m_UiManager->sphereSize, m_UiManager->sphereSize) + XMMatrixTranslation(m_UiManager->spherePosition.x, m_UiManager->spherePosition.y, m_UiManager->spherePosition.z);
	//// Send geometry data (from mesh)
	m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	m_TessellationDepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix, orthoMatrix, m_SphereMesh->GetTexture(),
		m_UiManager->tessellationSetup, m_UiManager->tessellationWarp, m_PrimaryLight, m_Camera->GetPosition());
	//// Render object (combination of mesh geometry and shader process
	m_TessellationDepthShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());
	// Reset world matrix
	float deScale = 1.0f;
	if (deScale > 0) deScale = 1.0f / m_UiManager->sphereSize;
	worldMatrix = XMMatrixScaling(deScale, deScale, deScale) + XMMatrixTranslation(-m_UiManager->spherePosition.x, -m_UiManager->spherePosition.y, -m_UiManager->spherePosition.z);

	//// translate plane mesh
	//worldMatrix = XMMatrixTranslation(-30.0, -17.0, -30.0);
	////// Send geometry data (from mesh)
	//m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	////// Set shader parameters (matrices and texture)
	//m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix, lightOrthographicMatrix);
	////// Render object (combination of mesh geometry and shader process
	//m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
	//// translate plane mesh
	//worldMatrix = XMMatrixTranslation(30.0, 17.0, 30.0);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();
	//// Present the rendered scene to the screen.
	//m_Direct3D->EndScene();

}

void App1::RenderDepth()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	m_DepthTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());
	//m_DepthTexture

	// Clear the render to texture. use the RGBA value to set background of texture // BLACK
	m_DepthTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	
	// Translate sphere mesh
	worldMatrix = XMMatrixScaling(m_UiManager->sphereSize, m_UiManager->sphereSize, m_UiManager->sphereSize) + XMMatrixTranslation(m_UiManager->spherePosition.x, m_UiManager->spherePosition.y, m_UiManager->spherePosition.z);
	//// Send geometry data (from mesh)
	m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	m_TessellationDepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture(),
		m_UiManager->tessellationSetup, m_UiManager->tessellationWarp, m_PrimaryLight, m_Camera->GetPosition());
	//// Render object (combination of mesh geometry and shader process
	m_TessellationDepthShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());
	// Reset world matrix
	float deScale = 1.0f;
	if (deScale > 0) deScale = 1.0f / m_UiManager->sphereSize;
	worldMatrix = XMMatrixScaling(deScale, deScale, deScale) + XMMatrixTranslation(-m_UiManager->spherePosition.x, -m_UiManager->spherePosition.y, -m_UiManager->spherePosition.z);
	
	//// translate plane mesh
	//worldMatrix = XMMatrixTranslation(-30.0, -17.0, -30.0);
	////// Send geometry data (from mesh)
	//m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	////// Set shader parameters (matrices and texture)
	//m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	////// Render object (combination of mesh geometry and shader process
	//m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
	//// translate plane mesh
	//worldMatrix = XMMatrixTranslation(30.0, 17.0, 30.0);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	//// Present the rendered scene to the screen.
	//m_Direct3D->EndScene();

}

bool App1::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	//// Clear the scene. (default blue colour)
	//m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Clear the render to texture. use the RGBA value to set background of texture // WHITE
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	//// Turn on wireframe mode
	//m_Direct3D->TurnOnWireframe();

	//// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	

	// Translate sphere mesh
	worldMatrix = XMMatrixScaling(m_UiManager->sphereSize, m_UiManager->sphereSize, m_UiManager->sphereSize) + XMMatrixTranslation(m_UiManager->spherePosition.x, m_UiManager->spherePosition.y, m_UiManager->spherePosition.z);
	//// Send geometry data (from mesh)
	m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	m_TessellationShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture(),
		m_UiManager->tessellationSetup, m_DepthTexture->GetShaderResourceView(), m_UiManager->tessellationWarp, m_PrimaryLight, m_Camera->GetPosition());
	//// Render object (combination of mesh geometry and shader process
	m_TessellationShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());
	// Reset world matrix
	float deScale = 1.0f;
	if (deScale > 0) deScale = 1.0f / m_UiManager->sphereSize;
	worldMatrix = XMMatrixScaling(deScale, deScale, deScale) + XMMatrixTranslation(-m_UiManager->spherePosition.x, -m_UiManager->spherePosition.y, -m_UiManager->spherePosition.z);


	// translate rock mesh
	
	//worldMatrix = XMMatrixTranslation(10, -5.0, 10.0);
	////worldMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	////// Send geometry data (from mesh)
	////m_Spaceship->SendData(m_Direct3D->GetDeviceContext());
	////// Set shader parameters (matrices and texture)
	////m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Spaceship->GetTexture(), m_Light, m_Camera->GetPosition());
	////// Render object (combination of mesh geometry and shader process
	////m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Spaceship->GetIndexCount());
	//// translate back to origin
	////worldMatrix = XMMatrixScaling(100, 100, 100);
	//worldMatrix = XMMatrixTranslation(-10.0, 5.0, -10.0);

	//
	//// translate plane mesh
	//worldMatrix = XMMatrixTranslation(-30.0, -17.0, -30.0);
	////// Send geometry data (from mesh)
	//m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	////// Set shader parameters (matrices and texture)
	//m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(), m_PrimaryLight, m_Camera->GetPosition());
	////// Render object (combination of mesh geometry and shader process
	//m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
	//m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());

	//m_ShadowShader->SetShaderParameters(
	//	m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(), m_RenderTexture->GetShaderResourceView(), m_PrimaryLight);

	//m_ShadowShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
	//// translate plane mesh
	//worldMatrix = XMMatrixTranslation(30.0, 17.0, 30.0);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	//// Present the rendered scene to the screen.
	//m_Direct3D->EndScene();

	//Render();

	return true;
}

bool App1::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;

	//// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Turn off wireframe mode
	//m_Direct3D->TurnOffWireframe();

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	//// Translate sphere mesh
	//worldMatrix = XMMatrixScaling(m_UiManager->sphereSize, m_UiManager->sphereSize, m_UiManager->sphereSize) + XMMatrixTranslation(m_UiManager->spherePosition.x, m_UiManager->spherePosition.y, m_UiManager->spherePosition.z);
	////// Send geometry data (from mesh)
	//m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
	////// Set shader parameters (matrices and texture)
	//m_TessellationShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture(),
	//	m_UiManager->tessellationSetup, m_UiManager->tessellationWarp, m_PrimaryLight, m_Camera->GetPosition());
	////// Render object (combination of mesh geometry and shader process
	//m_TessellationShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());
	//// Reset world matrix
	//float deScale = 1.0f;
	//if (deScale > 0) deScale = 1.0f / m_UiManager->sphereSize;
	//worldMatrix = XMMatrixScaling(deScale, deScale, deScale) + XMMatrixTranslation(-m_UiManager->spherePosition.x, -m_UiManager->spherePosition.y, -m_UiManager->spherePosition.z);


	//// translate rock mesh

	//worldMatrix = XMMatrixTranslation(10, -5.0, 10.0);
	////worldMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	////// Send geometry data (from mesh)
	////m_Spaceship->SendData(m_Direct3D->GetDeviceContext());
	////// Set shader parameters (matrices and texture)
	////m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Spaceship->GetTexture(), m_Light, m_Camera->GetPosition());
	////// Render object (combination of mesh geometry and shader process
	////m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Spaceship->GetIndexCount());
	//// translate back to origin
	////worldMatrix = XMMatrixScaling(100, 100, 100);
	//worldMatrix = XMMatrixTranslation(-10.0, 5.0, -10.0);


	//// translate plane mesh
	////worldMatrix = XMMatrixTranslation(-30.0, -17.0, -30.0);
	//////// Send geometry data (from mesh)
	////m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	//////// Set shader parameters (matrices and texture)
	////m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(), m_PrimaryLight, m_Camera->GetPosition());
	//////// Render object (combination of mesh geometry and shader process
	////m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());

	//m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());

	//m_ShadowShader->SetShaderParameters(
	//	m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(),
	//	m_PrimaryShadowMap->GetShaderResourceView(), m_PrimaryLight);

	//m_ShadowShader->Render(m_Direct3D->GetDeviceContext(),
	//	m_PlaneMesh->GetIndexCount());

	//worldMatrix = XMMatrixTranslation(30.0, 17.0, 30.0);

	// To render ortho mesh
	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	m_Direct3D->GetOrthoMatrix(orthoMatrix);// ortho matrix for 2D rendering
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	m_OrthoMesh->SendData(m_Direct3D->GetDeviceContext());

	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_RenderTexture->GetShaderResourceView()); //m_RenderTexture
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_OrthoMesh->GetIndexCount());

	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

void App1::SetLightParameters(Light* light, LightParameterType params)
{
	light->SetAmbientColour(params.ambientColour.x, params.ambientColour.y, params.ambientColour.z, params.ambientColour.w);
	light->SetDiffuseColour(params.diffuseColour.x, params.diffuseColour.y, params.diffuseColour.z, params.diffuseColour.w);
	light->SetSpecularColour(params.specularColour.x, params.specularColour.y, params.specularColour.z, params.specularColour.w);
	light->SetPosition(params.position.x, params.position.y, params.position.z);
	light->SetDirection(params.direction.x, params.direction.y, params.direction.z);
	light->SetSpecularPower(params.specularPower);
}
/*
void App1::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix;

	
	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture. use the RGBA value to set background of texture
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 1.f, 0.f, 0.f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	//m_Direct3D->GetWorldMatrix(worldMatrix);
	//m_Camera->GetViewMatrix(viewMatrix);
	//m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Light->GenerateViewMatrix();
	lightViewMatrix = m_Light->GetViewMatrix();
	m_Light->GenerateProjectionMatrix(1.f, 100.f);
	lightProjectionMatrix = m_Light->GetProjectionMatrix();
	m_Direct3D->GetWorldMatrix(worldMatrix);


	//worldMatrix = XMMatrixMultiply(XMMatrixRotationRollPitchYaw(0.0, 0.0, 0), XMMatrixTranslation(0.0, 0.0, 0.0));
	//// Send geometry data (from plane)
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	m_Direct3D->ResetViewport();

}


void App1::RenderScene()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;

	// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	/*
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(),
		m_RenderTexture->GetShaderResourceView(), m_Light);
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(),
		m_PlaneMesh->GetIndexCount());
	*/
/*
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(), m_Light, m_Camera->GetPosition());
	//// Render object (combination of mesh geometry and shader process
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
	
	// To render ortho mesh
	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	m_Direct3D->GetOrthoMatrix(orthoMatrix);// ortho matrix for 2D rendering
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	m_OrthoMesh->SendData(m_Direct3D->GetDeviceContext());

	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_RenderTexture->GetShaderResourceView());
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_OrthoMesh->GetIndexCount());

	m_Direct3D->TurnZBufferOn();
	
	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

}
*/