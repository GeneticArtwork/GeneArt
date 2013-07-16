////////////////////////////////////////////////////////////////////////////////
// Filename: graphics.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphics.h"


Graphics::Graphics()
{
	m_openGL = 0;
}

Graphics::~Graphics()
{
}


bool Graphics::Initialize()
{
	m_openGL = new OpenGL;
	return true;
}

OpenGL* Graphics::GetOpenGL()
{
	return m_openGL;
}

void Graphics::Shutdown()
{
	if(m_openGL)
	{
		m_openGL->Shutdown();
		delete m_openGL;
		m_openGL = 0;
	}
}

bool Graphics::Frame()
{

	return true;
}

bool Graphics::Render()
{

	return true;
}