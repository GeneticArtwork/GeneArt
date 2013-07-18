#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

bool Mesh::Initialize()
{
	bool result;

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers();
	if(!result)
	{
		return false;
	}

	return true;
}

void Mesh::Shutdown()
{
	// Release the vertex and index buffers.
	ShutdownBuffers();
}

void Mesh::Render()
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers();
}

bool Mesh::InitializeBuffers()
{
	VertexType* vertices;
	unsigned int* indices;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 3;

	// Set the number of indices in the index array.
	m_indexCount = 3;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned int[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array with data.

	// Bottom left.
	vertices[0].x = -1.0f;  // Position.
	vertices[0].y = -1.0f;
	vertices[0].z =  0.0f;

	// Top middle.
	vertices[1].x = 0.0f;  // Position.
	vertices[1].y = 1.0f;
	vertices[1].z = 0.0f;

	// Bottom right.
	vertices[2].x =  1.0f;  // Position.
	vertices[2].y = -1.0f;
	vertices[2].z =  0.0f;

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// Allocate an g_openGL vertex array object.
	g_openGL->glGenVertexArrays(1, &m_vertexArrayId);

	// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	g_openGL->glBindVertexArray(m_vertexArrayId);

	// Generate an ID for the vertex buffer.
	g_openGL->glGenBuffers(1, &m_vertexBufferId);

	// Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.
	g_openGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	g_openGL->glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexType), vertices, GL_STATIC_DRAW);

	// Enable the two vertex array attributes.
	g_openGL->glEnableVertexAttribArray(0);  // Vertex position.
	g_openGL->glEnableVertexAttribArray(1);  // Vertex color.

	// Specify the location and format of the position portion of the vertex buffer.
	g_openGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	g_openGL->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType), 0);

	// Specify the location and format of the color portion of the vertex buffer.
	g_openGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	g_openGL->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (3 * sizeof(float)));

	// Generate an ID for the index buffer.
	g_openGL->glGenBuffers(1, &m_indexBufferId);

	// Bind the index buffer and load the index data into it.
	g_openGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	g_openGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount* sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Now that the buffers have been loaded we can release the array data.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void Mesh::ShutdownBuffers()
{
	// Disable the two vertex array attributes.
	g_openGL->glDisableVertexAttribArray(0);
	g_openGL->glDisableVertexAttribArray(1);
	
	// Release the vertex buffer.
	g_openGL->glBindBuffer(GL_ARRAY_BUFFER, 0);
	g_openGL->glDeleteBuffers(1, &m_vertexBufferId);

	// Release the index buffer.
	g_openGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	g_openGL->glDeleteBuffers(1, &m_indexBufferId);

	// Release the vertex array object.
	g_openGL->glBindVertexArray(0);
	g_openGL->glDeleteVertexArrays(1, &m_vertexArrayId);
}

void Mesh::RenderBuffers()
{
	// Bind the vertex array object that stored all the information about the vertex and index buffers.
	g_openGL->glBindVertexArray(m_vertexArrayId);

	// Render the vertex buffer using the index buffer.
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}
