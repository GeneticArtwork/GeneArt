////////////////////////////////////////////////////////////////////////////////
// Filename: mesh.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MESH_H_
#define _MESH_H_


///////////////////////
// INCLUDES //
///////////////////////
#include "Global.h"

using namespace std;

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
		vector3d position;						// Position
		vector3d normal;						// Normals
		vector3d color;							// Color
		vector2d texture;						// Texture UV
	};

public:
	Mesh();
	~Mesh();

	bool Initialize();
	void Shutdown();
	void Render();								// Render the mesh.
	bool LoadTriangle();						// Test method to load a triangle.
	bool ImportOBJ(char* fileName);				// Import a wavefront object.
private:
	bool InitializeBuffers(VertexType* vertices, unsigned long* indices, int vertexCount, int indexCount);
	void ShutdownBuffers();
	void RenderBuffers();
	void GetOBJFileCounts(ifstream& file,		// Retrieve vertex count information
		int& vertexCount, int& indexCount,		// for the wavefront object.
		int& texCount, int& normalCount);

private:
	int m_vertexCount, m_indexCount;
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
};

#endif
