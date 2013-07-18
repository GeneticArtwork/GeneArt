////////////////////////////////////////////////////////////////////////////////
// Filename: graphics.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphics.h"


Graphics::Graphics()
{
	m_camera = 0;
	m_renderShader = 0;
}

Graphics::~Graphics()
{
}


bool Graphics::Initialize()
{
	// The camera for viewing the scene.
	m_camera = new Camera;
	m_camera->SetPosition(0.0f, 0.0f, -10.0f);

	// The shaders used to render the scene.
	m_renderShader = new RenderShader;
	if(!m_renderShader->Initialize())
	{
		MessageBox(g_hwnd, L"Could not initialize renderShader", L"GRAPHICS ERROR", MB_OK);
		return false;
	}

	// TODO: Create loading method for meshes.
	Mesh* newMesh = new Mesh;
	newMesh->Initialize();

	m_meshes.push_back(newMesh);

	return true;
}

void Graphics::Shutdown()
{
	if(m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}

	if(m_renderShader)
	{
		m_renderShader->Shutdown();
		delete m_renderShader;
		m_renderShader = 0;
	}

	ShutdownMeshes();
}

void Graphics::ShutdownMeshes()
{
	for(int i = 0; i < m_meshes.size(); i++)
	{
		if(m_meshes[i])
		{
			m_meshes[i]->Shutdown();
			delete m_meshes[i];
			m_meshes[i] = 0;
		}
	}

	m_meshes.clear();
}

bool Graphics::Frame()
{
	if(!Render())
	{
		return false;
	}

	return true;
}

bool Graphics::Render()
{
	if(g_openGL)
	{
		float worldMatrix[16];
		float viewMatrix[16];
		float projectionMatrix[16];

		// Clear the buffers to begin the scene.
		g_openGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

		// Generate the view matrix based on the camera's position.
		m_camera->Render();

		// Get the world, view, and projection matrices from the opengl and camera objects.
		g_openGL->GetWorldMatrix(worldMatrix);
		m_camera->GetViewMatrix(viewMatrix);
		g_openGL->GetProjectionMatrix(projectionMatrix);

		// Set the color shader as the current shader program and set the matrices that it will use for rendering.
		m_renderShader->SetShader();
		m_renderShader->SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix);

		// Render all of the loaded meshes.
		RenderMeshes();

		g_openGL->EndScene();

		return true;
	}
	return false;
}

void Graphics::RenderMeshes()
{
	for(unsigned int i = 0; i < m_meshes.size(); i++)
	{
		// TODO: Visiblity and culling check.
		m_meshes[i]->Render();
	}
}