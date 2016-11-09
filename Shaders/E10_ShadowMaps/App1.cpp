// Lab1.cpp
// Lab 1 example, simple textured quad
#include "App1.h"

App1::App1()
{
	m_Mesh = nullptr;
	m_PlaneMesh = nullptr;
	m_LightShader = nullptr;
	m_DepthShader = nullptr;
	m_ShadowShader = nullptr;
	m_TextureShader = nullptr;
	m_RenderTexture = nullptr;
	m_OrthoMesh = nullptr;
	m_Light = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	const int SHADOWMAP_WIDTH = 1024;
	const int SHADOWMAP_HEIGHT = 1024;

	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create Mesh object
	m_Mesh = new SphereMesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/checkerboard.png");
	m_PlaneMesh = new PlaneMesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/checkerboard.png");
	m_Model = new Model(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/checkerboard.png", L"../res/teapot.obj");

	m_LightShader = new SpecularLightShader(m_Direct3D->GetDevice(), hwnd);

	m_DepthShader = new DepthShader(m_Direct3D->GetDevice(), hwnd);

	m_ShadowShader = new ShadowShader(m_Direct3D->GetDevice(), hwnd);

	m_TextureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);

	// RenderTexture, OrthoMesh and shader set for different renderTarget
	m_RenderTexture = new RenderTexture(m_Direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);

	

	m_OrthoMesh = new OrthoMesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 0, 0);

	// Create Light object
	m_Light = new Light;

	// Initialise light properties
	m_Light->SetAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);

	m_Light->SetDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);

	m_Light->SetDirection(0.5f, -0.5f, 0.0f);

	m_Light->SetPosition(50.0f, 5.0f, 50.0f);

	m_Light->SetSpecularPower(10.0f);

	m_Light->SetSpecularColour(0.0f, 0.0f, 1.0f, 1.0f);

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (m_Mesh)
	{
		delete m_Mesh;
		m_Mesh = 0;
	}

	// Release the Direct3D object.
	if (m_PlaneMesh)
	{
		delete m_PlaneMesh;
		m_PlaneMesh = 0;
	}

	if (m_Model)
	{
		delete m_Model;
		m_Model = 0;
	}

	// Release the shader
	if (m_LightShader)
	{
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the shader
	if (m_DepthShader)
	{
		delete m_DepthShader;
		m_DepthShader = 0;
	}

	// Release the shader
	if (m_ShadowShader)
	{
		delete m_ShadowShader;
		m_ShadowShader = 0;
	}

	// Release the Light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (m_OrthoMesh)
	{
		delete m_OrthoMesh;
		m_OrthoMesh = 0;
	}

	if (m_TextureShader)
	{
		delete m_TextureShader;
		m_TextureShader = 0;
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
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	/*
	result = Render();
	if (!result)
	{
		return false;
	}
	*/
	RenderToTexture();
	RenderScene();

	return true;
}

bool App1::Render()
{
	iterator += 0.2f;
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	//// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	//worldMatrix = XMMatrixTranslation(0.0, -10.0, 30.0);
	/// For Model
	//// Send geometry data (from mesh)
	/*
	m_Model->SendData(m_Direct3D->GetDeviceContext());
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(),
		m_Mesh->GetIndexCount());

	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
		m_RenderTexture->GetShaderResourceView(), m_Light);
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(),
		m_Mesh->GetIndexCount());
		*/
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	m_DepthShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	m_DepthShader->Render(m_Direct3D->GetDeviceContext(),
		m_PlaneMesh->GetIndexCount());
	/*
	m_ShadowShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(),
		m_RenderTexture->GetShaderResourceView(), m_Light);
	m_ShadowShader->Render(m_Direct3D->GetDeviceContext(),
		m_PlaneMesh->GetIndexCount());
		*/
/*
	//// Set shader parameters (matrices and texture)
	m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_Light, m_Camera->GetPosition());
	//// Render object (combination of mesh geometry and shader process
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount());
	*/
	/*
	//// Send geometry data (from mesh)
	m_Mesh->SendData(m_Direct3D->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Mesh->GetTexture(), m_Light, m_Camera->GetPosition());
	//// Render object (combination of mesh geometry and shader process
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());
	*/
/*
	worldMatrix = XMMatrixTranslation(-30.0, -17.0, -30.0);
	//// Send geometry data (from mesh)
	m_PlaneMesh->SendData(m_Direct3D->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_PlaneMesh->GetTexture(), m_Light, m_Camera->GetPosition());
	//// Render object (combination of mesh geometry and shader process
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_PlaneMesh->GetIndexCount());
*/

	//worldMatrix = XMMatrixTranslation(2.0, 0.0, 0.0);
	worldMatrix = XMMatrixMultiply(XMMatrixRotationRollPitchYaw(0.0, 0.0, iterator), XMMatrixTranslation(2.0, 0.0, 0.0));

	//// Send geometry data (from mesh)
	//	m_Mesh_1->SendData(m_Direct3D->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	//	m_TextureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Mesh_1->GetTexture());
	//// Render object (combination of mesh geometry and shader process
	//	m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Mesh_1->GetIndexCount());

	//// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}

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
