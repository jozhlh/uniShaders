// VoronoiBoomtown - AppBoomtown.cpp
// Draws a sphere, which is manipulated through various graphics pipeline stages
#include "AppBoomtown.h"

AppBoomtown::AppBoomtown()
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
	m_LightShader = nullptr;
	m_TerrainShader = nullptr;
	m_TextureShader = nullptr;
	m_HorizontalBlurShader = nullptr;
	m_VerticalBlurShader = nullptr;

	// Geometry
	m_OrthoMesh = nullptr;
	m_testModel0 = nullptr;

	// Lights
	m_Light = nullptr;

	// Voronoi
	m_VoronoiMap = nullptr;
}

void AppBoomtown::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
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
	m_LightShader = new SpecularLightShader(m_Direct3D->GetDevice(), hwnd);
	m_TerrainShader = new SpecularTerrainShader(m_Direct3D->GetDevice(), hwnd);
	m_VerticalBlurShader = new VerticalBlurShader(m_Direct3D->GetDevice(), hwnd);
	m_HorizontalBlurShader = new HorizontalBlurShader(m_Direct3D->GetDevice(), hwnd);
	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);

	// Create Geometry with checkerboard texture
	m_testModel0 = new Model(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/palette.png", L"../res/testObject0.obj");

	// ortho size and position set based on window size
	// Position default at 0x0 centre window, to offset change values (pixel)
	m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 0, 0);

	// Create Light object
	m_Light = new Light;

	m_ModelBank = new ModelBank();

	// Initialise light properties
	m_Light->SetAmbientColour(m_UiManager->ambientColour.x, m_UiManager->ambientColour.y, m_UiManager->ambientColour.z, m_UiManager->ambientColour.w);

	m_Light->SetDiffuseColour(m_UiManager->diffuseColour.x, m_UiManager->diffuseColour.y, m_UiManager->diffuseColour.z, m_UiManager->diffuseColour.w);

	m_Light->SetDirection(m_UiManager->direction.x, m_UiManager->direction.y, m_UiManager->direction.z);

	m_Light->SetSpecularPower(m_UiManager->specularPower);

	m_Light->SetSpecularColour(m_UiManager->specularColour.x, m_UiManager->specularColour.y, m_UiManager->specularColour.z, m_UiManager->specularColour.w);

	m_VoronoiMap = new VoronoiMap(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), m_ModelBank,
		m_UiManager->gridDimensions, m_UiManager->cellBorder, m_UiManager->noiseHeight, m_UiManager->noiseResolution, m_UiManager->cellSize, m_UiManager->numberOfRegions, m_UiManager->regionColour);
}

AppBoomtown::~AppBoomtown()
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
	if (m_LightShader)
	{
		delete m_LightShader;
		m_LightShader = 0;
	}
	if (m_TerrainShader)
	{
		delete m_TerrainShader;
		m_TerrainShader = 0;
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

	// Release the Light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the Voronoi Map object
	if (m_VoronoiMap)
	{
		delete m_VoronoiMap;
		m_VoronoiMap = 0;
	}

	if (m_ModelBank)
	{
		delete m_ModelBank;
		m_ModelBank = 0;
	}
}


bool AppBoomtown::Frame()
{
	// Get delta time and calculate frame rate
	m_Timer->Frame();
	float dt = m_Timer->GetTime();
	m_UiManager->frameRate = 1.0f / dt;

	bool result;
	result = BaseApplication::Frame();

	if (!result) return false;

	if (m_UiManager->generate)
	{
		m_UiManager->generate = false;
		// Release the Voronoi Map object
		if (m_VoronoiMap)
		{
			delete m_VoronoiMap;
			m_VoronoiMap = nullptr;
		}
		m_VoronoiMap = new VoronoiMap(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), m_ModelBank,
			m_UiManager->gridDimensions, m_UiManager->cellBorder, m_UiManager->noiseHeight, m_UiManager->noiseResolution, m_UiManager->cellSize, m_UiManager->numberOfRegions, m_UiManager->regionColour);
	}
	if (m_UiManager->lightChanged)
	{
		m_UiManager->lightChanged = false;
		m_Light->SetAmbientColour(m_UiManager->ambientColour.x, m_UiManager->ambientColour.y, m_UiManager->ambientColour.z, m_UiManager->ambientColour.w);
		m_Light->SetDiffuseColour(m_UiManager->diffuseColour.x, m_UiManager->diffuseColour.y, m_UiManager->diffuseColour.z, m_UiManager->diffuseColour.w);
		m_Light->SetDirection(m_UiManager->direction.x, m_UiManager->direction.y, m_UiManager->direction.z);
		m_Light->SetSpecularPower(m_UiManager->specularPower);
		m_Light->SetSpecularColour(m_UiManager->specularColour.x, m_UiManager->specularColour.y, m_UiManager->specularColour.z, m_UiManager->specularColour.w);
	}

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

bool AppBoomtown::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture. use the RGBA value to set background of texture // WHITE
	//m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 1.0f, 0.95f, 0.6f, 1.0f);
	//(1.0f, 0.95f, 0.6f);
	//// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Translate sphere mesh
	//worldMatrix = XMMatrixScaling(m_UiManager->sphereSize, m_UiManager->sphereSize, m_UiManager->sphereSize) + XMMatrixTranslation(m_UiManager->spherePosition.x, m_UiManager->spherePosition.y, m_UiManager->spherePosition.z);
	////// Send geometry data (from mesh)
	//m_testModel0->SendData(m_Direct3D->GetDeviceContext());
	////// Set shader parameters (matrices and texture)
	//m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_testModel0->GetTexture(), m_Light, m_Camera->GetPosition());
	////// Render object (combination of mesh geometry and shader process
	//m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_testModel0->GetIndexCount());
	//// Reset world matrix
	//float deScale = 1.0f;
	//if (deScale > 0) deScale = 1.0f / m_UiManager->sphereSize;
	//worldMatrix = XMMatrixScaling(deScale, deScale, deScale) + XMMatrixTranslation(-m_UiManager->spherePosition.x, -m_UiManager->spherePosition.y, -m_UiManager->spherePosition.z);

	m_VoronoiMap->Render(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_LightShader, m_TerrainShader, m_Light, m_Camera->GetPosition(), m_UiManager->nodeColour,
		m_UiManager->centreOfRegionColour, m_UiManager->regionColour, m_UiManager->showNodes, m_UiManager->identifyRegions, m_UiManager->basePlateColour, m_UiManager->yOffset);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	return true;
}

bool AppBoomtown::Render()
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


void AppBoomtown::DownSample(RenderTexture* sourceTexture)
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

void AppBoomtown::HorizontalBlur()
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

void AppBoomtown::VerticalBlur()
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

void AppBoomtown::UpSample()
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



