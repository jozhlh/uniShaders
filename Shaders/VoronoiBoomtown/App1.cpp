// VoronoiBoomtown - App1.cpp
// Draws a sphere, which is manipulated through various graphics pipeline stages
#include "App1.h"

App1::App1()
{
	// System Objects
	m_UiManager = nullptr;
	m_Timer = nullptr;

	// Render Targets
	m_RenderTexture = nullptr;
	m_downScaledTexture = nullptr;
	m_horizontalBlurTexture = nullptr;
	m_verticalBlurTexture = nullptr;
	m_blurredTexture = nullptr;

	// Shader Controllers
	m_TessellationShader = nullptr;
	m_TextureShader = nullptr;
	m_HorizontalBlurShader = nullptr;
	m_VerticalBlurShader = nullptr;

	// Geometry
	m_SphereMesh = nullptr;
	m_OrthoMesh = nullptr;
	m_testModel0 = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Set screen variables
	float halfWidth = screenWidth * 0.5f;
	float halfHeight = screenHeight * 0.5f;

	screenDimensions.x = screenWidth;
	screenDimensions.y = screenHeight;

	// System Objects
	m_UiManager = new UiManager;
	m_Timer = new Timer;

	// Create Render Targets
	// Half Scale
	m_downScaledTexture = new RenderTexture(m_Direct3D->GetDevice(), halfWidth, halfHeight, SCREEN_NEAR, SCREEN_DEPTH);
	m_horizontalBlurTexture = new RenderTexture(m_Direct3D->GetDevice(), halfWidth, halfHeight, SCREEN_NEAR, SCREEN_DEPTH);
	m_verticalBlurTexture = new RenderTexture(m_Direct3D->GetDevice(), halfWidth, halfHeight, SCREEN_NEAR, SCREEN_DEPTH);
	
	// Full Scale
	m_RenderTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	m_blurredTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);;


	// Create Shader Objects
	m_TessellationShader = new TessellationShader(m_Direct3D->GetDevice(), hwnd);
	m_VerticalBlurShader = new VerticalBlurShader(m_Direct3D->GetDevice(), hwnd);
	m_HorizontalBlurShader = new HorizontalBlurShader(m_Direct3D->GetDevice(), hwnd);
	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);

	// Create Geometry with checkerboard texture
	m_SphereMesh = new TessellationSphere(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/checkerboard.png", 20.0f);

	m_testModel0 = new Model(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/testObject0.png", L"../res/testObject0.obj");

	// ortho size and position set based on window size
	// Position default at 0x0 centre window, to offset change values (pixel)
	m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 0, 0);

	// Initialise sphere animation
	//sphereLerp = m_UiManager->animationSpeed;
}

App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release System Objects
	if (m_UiManager)
	{
		delete m_UiManager;
		m_UiManager = 0;
	}
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}
	// Release Render Targets
	if (m_RenderTexture)
	{
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}
	if (m_downScaledTexture)
	{
		delete m_downScaledTexture;
		m_downScaledTexture = 0;
	}
	if (m_horizontalBlurTexture)
	{
		delete m_horizontalBlurTexture;
		m_horizontalBlurTexture = 0;
	}
	if (m_verticalBlurTexture)
	{
		delete m_verticalBlurTexture;
		m_verticalBlurTexture = 0;
	}
	if (m_blurredTexture)
	{
		delete m_blurredTexture;
		m_blurredTexture = 0;
	}

	// Release Shader Objects
	if (m_TessellationShader)
	{
		delete m_TessellationShader;
		m_TessellationShader = 0;
	}
	if (m_TextureShader)
	{
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	if (m_VerticalBlurShader)
	{
		delete m_VerticalBlurShader;
		m_VerticalBlurShader = 0;
	}
	if (m_HorizontalBlurShader)
	{
		delete m_HorizontalBlurShader;
		m_HorizontalBlurShader = 0;
	}

	// Release Geometry
	/*if (m_spheremesh)
	{
		delete m_spheremesh;
		m_spheremesh = 0;
	}*/
	if (m_testModel0)
	{
		delete m_testModel0;
		m_testModel0 = 0;
	}
	if (m_OrthoMesh)
	{
		delete m_OrthoMesh;
		m_OrthoMesh = 0;
	}
}


bool App1::Frame()
{
	// Get delta time and calculate frame rate
	m_Timer->Frame();
	float dt = m_Timer->GetTime();
	m_UiManager->frameRate = 1.0f / dt;

	bool result;
	result = BaseApplication::Frame();

	if (!result) return false;

	//// Get animation position from UI
	//sphereLerp = m_UiManager->tessellationWarp.lerpAmount * ANIM_CAP;

	//// If the sphere is being animated
	//if (m_UiManager->playAnimation)
	//{
	//	// Choose which direction the animation is going
	//	if (m_UiManager->tessellationWarp.targetSin)
	//	{
	//		if (sphereLerp < ANIM_CAP)
	//		{
	//			sphereLerp += (dt * m_UiManager->animationSpeed);
	//		}
	//		else if (m_UiManager->playAnimation)
	//		{
	//			m_UiManager->tessellationWarp.targetSin = false;
	//		}
	//	}
	//	else
	//	{
	//		if (sphereLerp > 0.0f)
	//		{
	//			sphereLerp -= (dt * m_UiManager->animationSpeed);
	//		}
	//		else if (m_UiManager->playAnimation)
	//		{
	//			m_UiManager->tessellationWarp.targetSin = true;
	//		}
	//	}
	//}
	//// Lerp between surface modifiers
	//m_UiManager->tessellationWarp.lerpAmount = sphereLerp / ANIM_CAP;

	// Display UI window
	bool show_test_window = true;
	//ImGui::ShowTestWindow(&show_test_window);
	m_UiManager->ShowUi(&show_test_window);
	
	// Render the scene to a texture
	RenderToTexture();
	
	// Blur the texture the required number of times
	for (int i = 0; i < m_UiManager->blurLoops; i++)
	{
		DownSample(m_RenderTexture);
		HorizontalBlur();
		VerticalBlur();
		UpSample();;
	}

	// Render the texture to screen
	Render();

	return true;
}


void App1::DownSample(RenderTexture* sourceTexture)
{
	XMMATRIX worldMatrix, orthoMatrix, baseViewMatrix;
	
	// Set the render target to be the render to texture.
	m_downScaledTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture. use the RGBA value to set background of texture // WHITE
	m_downScaledTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	// Get the world, view and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	m_Direct3D->TurnZBufferOff();

	// Ortho matrix for 2D rendering
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	m_OrthoMesh->SendData(m_Direct3D->GetDeviceContext());

	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, sourceTexture->GetShaderResourceView()); //m_RenderTexture
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_OrthoMesh->GetIndexCount());
}

void App1::HorizontalBlur()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, baseViewMatrix;

	// Set the render target to be the render to texture.
	m_horizontalBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture. use the RGBA value to set background of texture // WHITE
	m_horizontalBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	// Get the world, view and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	m_OrthoMesh->SendData(m_Direct3D->GetDeviceContext());

	m_HorizontalBlurShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_downScaledTexture->GetShaderResourceView(), screenDimensions.x, m_UiManager->blurWeightings); //m_RenderTexture
	m_HorizontalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_OrthoMesh->GetIndexCount());
}

void App1::VerticalBlur()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, baseViewMatrix;

	// Set the render target to be the render to texture.
	m_verticalBlurTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture. use the RGBA value to set background of texture // WHITE
	m_verticalBlurTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	//m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	m_OrthoMesh->SendData(m_Direct3D->GetDeviceContext());

	m_VerticalBlurShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_horizontalBlurTexture->GetShaderResourceView(), screenDimensions.y, m_UiManager->blurWeightings); //m_RenderTexture
	m_VerticalBlurShader->Render(m_Direct3D->GetDeviceContext(), m_OrthoMesh->GetIndexCount());
}

void App1::UpSample()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, baseViewMatrix;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture. use the RGBA value to set background of texture // WHITE
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	m_OrthoMesh->SendData(m_Direct3D->GetDeviceContext());

	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, m_verticalBlurTexture->GetShaderResourceView()); //m_RenderTexture
	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_OrthoMesh->GetIndexCount());

	m_Direct3D->TurnZBufferOn();

	m_Direct3D->SetBackBufferRenderTarget();
}

bool App1::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture. use the RGBA value to set background of texture // WHITE
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	//// Translate sphere mesh
	//worldMatrix = XMMatrixScaling(m_UiManager->sphereSize, m_UiManager->sphereSize, m_UiManager->sphereSize) + XMMatrixTranslation(m_UiManager->spherePosition.x, m_UiManager->spherePosition.y, m_UiManager->spherePosition.z);
	////// Send geometry data (from mesh)
	//m_SphereMesh->SendData(m_Direct3D->GetDeviceContext());
	////// Set shader parameters (matrices and texture)
	//m_TessellationShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_SphereMesh->GetTexture(),
	//	m_UiManager->tessellationSetup, m_UiManager->tessellationWarp, m_UiManager->explode);
	////// Render object (combination of mesh geometry and shader process
	//m_TessellationShader->Render(m_Direct3D->GetDeviceContext(), m_SphereMesh->GetIndexCount());
	//// Reset world matrix
	//float deScale = 1.0f;
	//if (deScale > 0) deScale = 1.0f / m_UiManager->sphereSize;
	//worldMatrix = XMMatrixScaling(deScale, deScale, deScale) + XMMatrixTranslation(-m_UiManager->spherePosition.x, -m_UiManager->spherePosition.y, -m_UiManager->spherePosition.z);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	return true;
}

bool App1::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMatrix;

	//// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

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