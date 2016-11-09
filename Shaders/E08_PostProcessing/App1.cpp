// Lab1.cpp
// Lab 1 example, simple textured quad
#include "App1.h"

App1::App1()
{
	m_Mesh = nullptr;
	m_Plane = nullptr;
	m_OrthoMesh = nullptr;
	m_LightShader = nullptr;
	m_TextureShader = nullptr;
	m_RenderTexture = nullptr;
	m_Light = nullptr;
	m_Timer = nullptr;
	m_Input = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create Mesh object
	m_Mesh = new SphereMesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/brick1.dds");

	m_Plane = new PlaneMesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/brick1.dds");
	
	m_LightShader = new LightShader(m_Direct3D->GetDevice(), hwnd);

	m_Timer = new Timer();

	m_Input = new Input();

	// Create Light object
	m_Light = new Light;


	// Initialise light properties
	m_Light->SetAmbientColour(0.4f, 0.0f, 0.0f, 1.0f);

	m_Light->SetDiffuseColour(0.0f, 1.0f, 1.0f, 1.0f);

	m_Light->SetDirection(0.5f, -0.5f, 0.0f);

	// RenderTexture, OrthoMesh and shader set for different renderTarget
	m_RenderTexture = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	// ortho size and position set based on window size
	// 200x200 pixels (standard would be matching window size for fullscreen mesh
	// Position default at 0x0 centre window, to offset change values (pixel)
	m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth / 2, screenHeight / 2, 0, 0);
	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);


	deltaTime = 0.0f;
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Input object.
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Release the Timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the Direct3D object.
	if (m_Mesh)
	{
		delete m_Mesh;
		m_Mesh = 0;
	}

	// Release the Direct3D object.
	if (m_Plane)
	{
		delete m_Plane;
		m_Plane = 0;
	}

	// Release the shader
	if (m_LightShader)
	{
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the Light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
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
}


bool App1::Frame()
{
	bool result;

	result = BaseApplication::Frame();

	//bool show_test_window = true;
	//ImGui::ShowTestWindow(&show_test_window);

	if (!result)
	{
		return false;
	}

	// 1 key
	if (m_Input->isKeyDown(0x31))
	{
		//decrease frequency
		frequency -= MINFREQUENCY;
		if (frequency < MINFREQUENCY)
		{
			frequency = MINFREQUENCY;
		}
	}
	// 2 key
	if (m_Input->isKeyDown(0x32))
	{
		//increase frequency
	}

	// 3 key
	if (m_Input->isKeyDown(0x33))
	{
		//decrease height
	}
	// 4 key
	if (m_Input->isKeyDown(0x34))
	{
		//increase height
	}

	// Render the graphics.
	/*result = Render();
	if (!result)
	{
		return false;
	}*/
	RenderToTexture();
	RenderScene();

	return true;
}

bool App1::Render()
{
	// Render scene to texture / render target
	//RenderToTexture();

	// Down sample prior to blurring, optimise
	//DownSample();

	// Apply horizontal blur stage
	//HorizontalBlur();

	// Apply vertical blur to the horizontal blur stage
	//VerticalBlur();

	// Up sample, return to screen size before outputting to screen
	//UpSample();

	// Render scene to buffer
	//RenderScene();

	//return true;

	iterator += 0.2f;
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	//m_Timer->Frame();
	//deltaTime += m_Timer->GetTime();
	deltaTime += 0.01f;

	//// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixMultiply(XMMatrixRotationRollPitchYaw(0.0, 0.0, 0), XMMatrixTranslation(-5.0, 5.0, 0.0));
	//// Send geometry data (from mesh)
	m_Mesh->SendData(m_Direct3D->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Mesh->GetTexture(), m_Light, deltaTime, 1);
	//// Render object (combination of mesh geometry and shader process
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());


	worldMatrix = XMMatrixMultiply(XMMatrixRotationRollPitchYaw(0.0, 0.0, 0), XMMatrixTranslation(0.0, 0.0, 0.0));
	//// Send geometry data (from plane)
	m_Plane->SendData(m_Direct3D->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Plane->GetTexture(), m_Light, deltaTime, 1);
	//// Render object (combination of mesh geometry and shader process
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Plane->GetIndexCount());


	//worldMatrix = XMMatrixTranslation(2.0, 0.0, 0.0);
	worldMatrix = XMMatrixMultiply(XMMatrixRotationRollPitchYaw(0.0, 0.0, iterator), XMMatrixTranslation(2.0, 0.0, 0.0));

	//// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

void App1::RenderToTexture()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	XMFLOAT3 targetPosition = XMFLOAT3(0.0f, -5.0f, 10.0f);
	XMFLOAT3 targetRotation = XMFLOAT3(0.0f, 90.0f, 0.0f);

	deltaTime += 0.01f;

	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the render to texture. use the RGBA value to set background of texture
//	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 1.0f, 0.0f, 0.0f, 1.0f);
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.39f, 0.58f, 0.92f, 1.0f);

	XMFLOAT3 currentPosition = m_Camera->GetPosition();
	XMVECTOR currentRotation = m_Camera->GetRotation();

	//m_Camera->SetPosition(targetPosition.x, targetPosition.y, targetPosition.z);
	//m_Camera->SetRotation(targetRotation.x, targetRotation.y, targetRotation.z);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Mesh->SendData(m_Direct3D->GetDeviceContext());

	m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Mesh->GetTexture(), m_Light, deltaTime, 1);
	// Render object (combination of mesh geometry and shader process
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());

	worldMatrix = XMMatrixMultiply(XMMatrixRotationRollPitchYaw(0.0, 0.0, 0), XMMatrixTranslation(0.0, 0.0, 0.0));
	//// Send geometry data (from plane)
	m_Plane->SendData(m_Direct3D->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Plane->GetTexture(), m_Light, deltaTime, 1);
	//// Render object (combination of mesh geometry and shader process
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Plane->GetIndexCount());

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	m_Camera->SetPosition(currentPosition.x, currentPosition.y, currentPosition.z);
	m_Camera->SetRotation(currentRotation.m128_f32[0], currentRotation.m128_f32[1], currentRotation.m128_f32[2]);
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

	// Send geometry data (from mesh)
	m_Mesh->SendData(m_Direct3D->GetDeviceContext());
	// Set shader parameters (matrices and texture)
	m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Mesh->GetTexture(), m_Light, deltaTime, 1);
	// Render object (combination of mesh geometry and shader process
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());

	worldMatrix = XMMatrixMultiply(XMMatrixRotationRollPitchYaw(0.0, 0.0, 0), XMMatrixTranslation(0.0, 0.0, 0.0));
	//// Send geometry data (from plane)
	m_Plane->SendData(m_Direct3D->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Plane->GetTexture(), m_Light, deltaTime, 1);
	//// Render object (combination of mesh geometry and shader process
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Plane->GetIndexCount());

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
