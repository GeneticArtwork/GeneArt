////////////////////////////////////////////////////////////////////////////////
// Filename: mesh.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MESH_H_
#define _MESH_H_


///////////////////////
// INCLUDES //
///////////////////////
#include "Global.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: Mesh
//
// The base class for all meshes loaded by opengl.
////////////////////////////////////////////////////////////////////////////////
class Mesh
{
private:
	struct VertexType
	{
		float x, y, z;				// Position
		float nx, ny, nz;			// Normals
		float u, v;					// Texture UV
	};

public:
	Mesh();
	~Mesh();

	bool Initialize();
	void Shutdown();
	void Render();

private:
	bool InitializeBuffers();
	void ShutdownBuffers();
	void RenderBuffers();

private:
	int m_vertexCount, m_indexCount;
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
};

#endif
