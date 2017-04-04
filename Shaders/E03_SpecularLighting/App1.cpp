// Specular Shader - App1.cpp
#include "App1.h"

App1::App1()
{
	m_Mesh = nullptr;
	m_LightShader = nullptr;
	m_Light = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);

	// Create Mesh object
	m_Mesh = new SphereMesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/checkerboard.png");

	m_testMesh = new Model(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), L"../res/testObject0.png", L"../res/testObject0.obj");
	

	m_LightShader = new SpecularLightShader(m_Direct3D->GetDevice(), hwnd);

	// Create Light object
	m_Light = new Light;


	// Initialise light properties
	m_Light->SetAmbientColour(0.5f, 0.5f, 0.5f, 1.0f);

	m_Light->SetDiffuseColour(0.0f, 0.5f, 0.0f, 1.0f);

	m_Light->SetDirection(0.5f, -0.5f, 0.0f);

	m_Light->SetSpecularPower(25.0f);

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
	if (m_testMesh)
	{
		delete m_testMesh;
		m_testMesh = 0;
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
	result = Render();
	if (!result)
	{
		return false;
	}

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

	//// Send geometry data (from mesh)
	//m_Mesh->SendData(m_Direct3D->GetDeviceContext());
	////// Set shader parameters (matrices and texture)
	//m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Mesh->GetTexture(), m_Light, m_Camera->GetPosition());
	////// Render object (combination of mesh geometry and shader process
	//m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Mesh->GetIndexCount());

	//// Send geometry data (from mesh)
	m_testMesh->SendData(m_Direct3D->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	m_LightShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_testMesh->GetTexture(), m_Light, m_Camera->GetPosition());
	//// Render object (combination of mesh geometry and shader process
	m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_testMesh->GetIndexCount());


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


