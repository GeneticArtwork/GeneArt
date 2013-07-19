#include "Mesh.h"


// These structs are used to import data.
struct FaceTypes
{
	int vIndex[3];
	int tIndex[3];
	int nIndex[3];
};

struct TextureCoords
{
	float u, v;
};

struct NormalCoords
{
	float x, y, z;
};



Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

// TODO: ERASE. This method is most likely not ever necessary.
bool Mesh::Initialize()
{
	bool result;
	return ImportOBJ("sphere.obj");
	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	//result = InitializeBuffers();
	if(!result)
	{
	//	return false;
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

// TODO: This is kinda messy and probably buggy. Clean it up.
bool Mesh::ImportOBJ(char* fileName)
{
	// Compile the object file path.
	stringstream ss;
	ss << OBJECT_DIRECTORY << fileName;

	ifstream file;
	// Open the file.
	file.open(ss.str(), ios::in);

	// Check if the file exists.
	if(!file)
	{
		stringstream em;
		em << "The file " << ss.str() << " does not exist or is restricted.";
		ErrorMessage((char*)em.str().c_str(), ERROR_GRAPHICS);
		return false;
	}

	int vertexCount, indexCount, texCount, normalCount;

	// Get the number of vertices and faces in the file.
	GetOBJFileCounts(file, vertexCount, indexCount, texCount, normalCount);

	// The file has incomplete data.
	if(vertexCount == 0 || indexCount == 0)
	{
		ErrorMessage("File counts do not match file type", ERROR_GRAPHICS);
		file.close();
		return false;
	}

	int faceCount = indexCount / 3;

	VertexType* vertices = new VertexType[vertexCount];
	TextureCoords* texCoords = 0;
	NormalCoords* normalCoords = 0;

	if(texCount > 0)
	{
		texCoords = new TextureCoords[texCount];
		indexCount = faceCount * 3;
	}

	if(normalCount > 0)
	{
		normalCoords = new NormalCoords[normalCount];
	}

	FaceTypes* faceTypes = new FaceTypes[faceCount];

	unsigned long* indices = new unsigned long[indexCount];

	int vertexIndex = 0;
	int normalIndex = 0;
	int vertexTextureIndex = 0;
	int faceIndex = 0;

	// Reset file to beginning.
	file.clear();
	file.seekg(0, ios::beg);

	char input;

	bool leftHandConversion = false;

	file.get(input);
	while(!file.eof())
	{
		// Comment found. Skip to the end of the line.
		if(input == '#')
		{
			while(input != '\n')
			{
				file.get(input);
			}
		}

		if(input == 'v')
		{
			// Next two numbers are the UV coord.
			file.get(input);
			if(input == 't')
			{
				// Texture coordinates.
				file >> texCoords[vertexTextureIndex].u >> texCoords[vertexTextureIndex].v;
				if(leftHandConversion)
					texCoords[vertexTextureIndex].v = 1.0f - texCoords[vertexTextureIndex].v;
				vertexTextureIndex++;
			}
			// Next three numbers are the normals.
			else if(input == 'n')
			{
				file >> normalCoords[normalIndex].x >> normalCoords[normalIndex].y >> normalCoords[normalIndex].z;
				if(leftHandConversion)
					normalCoords[normalIndex].z *= -1.0f;
				normalIndex++;
			}
			// Next three numbers are the vertex.
			else if(input == ' ')
			{
				// Position coordinates.
				file >> vertices[vertexIndex].position.x >> vertices[vertexIndex].position.y >> vertices[vertexIndex].position.z;
				if(leftHandConversion)
					vertices[vertexIndex].position.z *= -1.0f;
				vertexIndex++;
			}
		}

		// Face information. This could have multiple entries per line.
		if(input == 'f')
		{
			file.get(input);
			if(input == ' ')
			{
				char slash;
				if(texCoords)
				{
					// With texture coordinates.
					file >> faceTypes[faceIndex].vIndex[0] >> slash >> faceTypes[faceIndex].tIndex[0] >>
						faceTypes[faceIndex].vIndex[1] >> slash >> faceTypes[faceIndex].tIndex[1] >>
						faceTypes[faceIndex].vIndex[2] >> slash >> faceTypes[faceIndex].tIndex[2];
				}
				else if(normalCoords) // load texture and normal coords.
				{
					// With normal coordinates.
					if(!leftHandConversion)
					{
						file >> faceTypes[faceIndex].vIndex[0] >> slash >> faceTypes[faceIndex].tIndex[0] >> slash >> faceTypes[faceIndex].nIndex[0] >>
							faceTypes[faceIndex].vIndex[1] >> slash >> faceTypes[faceIndex].tIndex[1] >> slash >> faceTypes[faceIndex].nIndex[1] >>
							faceTypes[faceIndex].vIndex[2] >> slash >> faceTypes[faceIndex].tIndex[2] >> slash >> faceTypes[faceIndex].nIndex[2];
					}
					else
					{
						file >> faceTypes[faceIndex].vIndex[2] >> slash >> faceTypes[faceIndex].tIndex[2] >> slash >> faceTypes[faceIndex].nIndex[2] >>
							faceTypes[faceIndex].vIndex[1] >> slash >> faceTypes[faceIndex].tIndex[1] >> slash >> faceTypes[faceIndex].nIndex[1] >>
							faceTypes[faceIndex].vIndex[0] >> slash >> faceTypes[faceIndex].tIndex[0] >> slash >> faceTypes[faceIndex].nIndex[0];
					}
				}
				else
				{
					// No texture coordinates.
					file >> faceTypes[faceIndex].vIndex[0] >> faceTypes[faceIndex].vIndex[1] >>
						faceTypes[faceIndex].vIndex[2];
				}
				faceIndex++;
			}
		}
		// Read remainder of line.
		while(input != '\n')
		{
			file.get(input);
		}

		file.get(input);
	}

	// Close the file.
	file.close();

	VertexType* finalVertices = 0;
	
	if(texCoords)
	{
		vertexCount = faceCount * 3;
		finalVertices = new VertexType[vertexCount];
	}
	int finalIndex = 0;
	for(int face = 0; face < faceCount; face++)
	{
		for(int i = 0; i < 3; i++)
		{
			int vIndex = faceTypes[face].vIndex[i] - 1;
			int tIndex = faceTypes[face].tIndex[i] - 1;
			int nIndex = faceTypes[face].nIndex[i] - 1;

			if(normalCoords)
			{
				vertices[vIndex].normal = vector3d(normalCoords[nIndex].x, normalCoords[nIndex].y, normalCoords[nIndex].z);
				vertices[vIndex].color = vector3d(0.5f, 1.0f, 0.5f);
			}

			if(!texCoords)
			{
				vertices[vIndex].texture.x = 0.0f;
				vertices[vIndex].texture.y = 0.0f;
				vertices[vIndex].color = vector3d(0.5f, 1.0f, 0.5f);
				indices[i + face * 3] = vIndex;
			}
			else
			{
				finalVertices[finalIndex].position = vertices[vIndex].position;
				finalVertices[finalIndex].texture.x = texCoords[tIndex].u;
				finalVertices[finalIndex].texture.y = texCoords[tIndex].v;
				finalVertices[finalIndex].color = vector3d(0.5f, 1.0f, 0.5f);
				finalIndex++;
			}
		}
	}

	if(texCoords)
	{
		for(int i = 0; i < vertexCount; i++)
		{
			indices[i] = i;
		}
	}
	
	// Data has been gathered, load into buffers.
	InitializeBuffers(texCoords ? finalVertices : vertices, indices, vertexCount, indexCount);

	// Free memory.
	if(vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	if(finalVertices)
	{
		delete[] finalVertices;
		finalVertices = 0;
	}

	if(indices)
	{
		delete[] indices;
		indices = 0;
	}
	delete[] texCoords;
	delete[] faceTypes;

	return true;
}

void Mesh::GetOBJFileCounts(ifstream& file, int& vertexCount, int& indexCount, int& texCount, int& normalCount)
{
	char input;

	vertexCount = 0;
	indexCount = 0;
	texCount = 0;
	normalCount = 0;

	file.get(input);
	while(!file.eof())
	{
		// Comment found. Skip to the end of the line.
		if(input == '#')
		{
			while(input != '\n')
			{
				file.get(input);
			}
		}
		// Vertex found.
		if(input == 'v')
		{
			file.get(input);
			// Make sure this isn't a texture coordinate being read.
			if(input == ' ')
				vertexCount++;
			else if(input == 't')
				texCount++;
			else if(input == 'n')
				normalCount++;
		}
		// Face found.
		if(input == 'f')
		{
			indexCount += 3;
		}
		// Read remainder of line.
		while(input != '\n')
		{
			file.get(input);
		}
		// Read beginning of next line.
		file.get(input);
	}
}

bool Mesh::LoadTriangle()
{
	// Set the number of vertices in the vertex array.
	m_vertexCount = 3;

	// Set the number of indices in the index array.
	m_indexCount = 3;

	// Create the vertex array.
	VertexType* vertices = new VertexType[m_vertexCount];

	// Create the index array.
	unsigned long* indices = new unsigned long[m_indexCount];

	// Load the vertex array with data.

	// Bottom left.
	vertices[0].position = vector3d(-1.0f, -1.0f, 0.0f);
	vertices[0].color = vector3d(0.5f, 1.0f, 0.5f);

	// Top middle.
	vertices[1].position = vector3d(0.0f, 1.0f, 0.0f);
	vertices[1].color = vector3d(0.5f, 1.0f, 0.5f);

	// Bottom right.
	vertices[2].position = vector3d(1.0f, -1.0f, 0.0f);
	vertices[2].color = vector3d(0.5f, 1.0f, 0.5f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	bool result = InitializeBuffers(vertices, indices, m_vertexCount, m_indexCount);

	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	return result;
}

bool Mesh::InitializeBuffers(VertexType* vertices, unsigned long* indices, int vertexCount, int indexCount)
{
	m_vertexCount = vertexCount;
	m_indexCount = indexCount;

	// Allocate an g_openGL vertex array object.
	g_openGL->glGenVertexArrays(1, &m_vertexArrayId);

	// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	g_openGL->glBindVertexArray(m_vertexArrayId);

	// Generate an ID for the vertex buffer.
	g_openGL->glGenBuffers(1, &m_vertexBufferId);

	// Bind the vertex buffer and load the vertex including all vertex attributes data into the vertex buffer.
	g_openGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	g_openGL->glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexType), vertices, GL_STATIC_DRAW);

	// Enable the two vertex array attributes.
	g_openGL->glEnableVertexAttribArray(0);  // Vertex position.
	g_openGL->glEnableVertexAttribArray(1);  // Vertex normal.
	g_openGL->glEnableVertexAttribArray(2);  // Vertex color.
	g_openGL->glEnableVertexAttribArray(3);  // Vertex texture uv.

	// Specify the location and format of the position portion of the vertex buffer.
	g_openGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	g_openGL->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType), 0);

	// Specify the location and format of the normal portion of the vertex buffer.
	g_openGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	g_openGL->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (1 * sizeof(vector3d)));

	// Specify the location and format of the color portion of the vertex buffer.
	g_openGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	g_openGL->glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (2 * sizeof(vector3d)));

	// Specify the location and format of the texture uv portion of the vertex buffer.
	g_openGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	g_openGL->glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (3 * sizeof(vector3d)));

	// Generate an ID for the index buffer.
	g_openGL->glGenBuffers(1, &m_indexBufferId);

	// Bind the index buffer and load the index data into it.
	g_openGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	g_openGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount* sizeof(unsigned int), indices, GL_STATIC_DRAW);

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
