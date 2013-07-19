////////////////////////////////////////////////////////////////////////////////
// Filename: rendershader.cpp
////////////////////////////////////////////////////////////////////////////////
#include "rendershader.h"


RenderShader::RenderShader()
{
}


RenderShader::~RenderShader()
{
}

bool RenderShader::Initialize()
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader("../Genome/render.vs", "../Genome/render.ps");
	if(!result)
	{
		return false;
	}

	return true;
}

void RenderShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();
}

void RenderShader::SetShader()
{
	// Install the shader program as part of the current rendering state.
	g_openGL->glUseProgram(m_shaderProgram);
}

bool RenderShader::InitializeShader(char* vsFilename, char* fsFilename)
{
	const char* vertexShaderBuffer;
	const char* fragmentShaderBuffer;
	int status;

	// Load the vertex shader source file into a text buffer.
	vertexShaderBuffer = LoadShaderSourceFile(vsFilename);
	if(!vertexShaderBuffer)
	{
		return false;
	}

	// Load the fragment shader source file into a text buffer.
	fragmentShaderBuffer = LoadShaderSourceFile(fsFilename);
	if(!fragmentShaderBuffer)
	{
		return false;
	}

	// Create a vertex and fragment shader object.
	m_vertexShader = g_openGL->glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShader = g_openGL->glCreateShader(GL_FRAGMENT_SHADER);

	// Copy the shader source code strings into the vertex and fragment shader objects.
	g_openGL->glShaderSource(m_vertexShader, 1, &vertexShaderBuffer, NULL);
	g_openGL->glShaderSource(m_fragmentShader, 1, &fragmentShaderBuffer, NULL);

	// Release the vertex and fragment shader buffers.
	delete [] vertexShaderBuffer;
	vertexShaderBuffer = 0;
	
	delete [] fragmentShaderBuffer;
	fragmentShaderBuffer = 0;

	// Compile the shaders.
	g_openGL->glCompileShader(m_vertexShader);
	g_openGL->glCompileShader(m_fragmentShader);

	// Check to see if the vertex shader compiled successfully.
	g_openGL->glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);
	if(status != 1)
	{
		// If it did not compile then write the syntax error message out to a text file for review.
		OutputShaderErrorMessage(m_vertexShader, vsFilename);
		return false;
	}

	// Check to see if the fragment shader compiled successfully.
	g_openGL->glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &status);
	if(status != 1)
	{
		// If it did not compile then write the syntax error message out to a text file for review.
		OutputShaderErrorMessage(m_fragmentShader, fsFilename);
		return false;
	}

	// Create a shader program object.
	m_shaderProgram = g_openGL->glCreateProgram();

	// Attach the vertex and fragment shader to the program object.
	g_openGL->glAttachShader(m_shaderProgram, m_vertexShader);
	g_openGL->glAttachShader(m_shaderProgram, m_fragmentShader);

	// Bind the shader input variables.
	g_openGL->glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
	g_openGL->glBindAttribLocation(m_shaderProgram, 1, "inputNormal");
	g_openGL->glBindAttribLocation(m_shaderProgram, 2, "inputColor");
	g_openGL->glBindAttribLocation(m_shaderProgram, 3, "inputUV");

	// Link the shader program.
	g_openGL->glLinkProgram(m_shaderProgram);

	// Check the status of the link.
	g_openGL->glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
	if(status != 1)
	{
		// If it did not link then write the syntax error message out to a text file for review.
		OutputLinkerErrorMessage(m_shaderProgram);
		return false;
	}

	return true;
}

char* RenderShader::LoadShaderSourceFile(char* filename)
{
	ifstream fin;
	int fileSize;
	char input;
	char* buffer;

	// Open the shader source file.
	fin.open(filename);

	// If it could not open the file then exit.
	if(fin.fail())
	{
		return 0;
	}

	// Initialize the size of the file.
	fileSize = 0;

	// Read the first element of the file.
	fin.get(input);

	// Count the number of elements in the text file.
	while(!fin.eof())
	{
		fileSize++;
		fin.get(input);
	}

	// Close the file for now.
	fin.close();

	// Initialize the buffer to read the shader source file into.
	buffer = new char[fileSize+1];
	if(!buffer)
	{
		return 0;
	}

	// Open the shader source file again.
	fin.open(filename);

	// Read the shader text file into the buffer as a block.
	fin.read(buffer, fileSize);

	// Close the file.
	fin.close();

	// Null terminate the buffer.
	buffer[fileSize] = '\0';

	return buffer;
}

void RenderShader::OutputShaderErrorMessage(unsigned int shaderId, char* shaderFilename)
{
	int logSize, i;
	char* infoLog;
	ofstream fout;
	wchar_t newString[128];
	unsigned int error, convertedChars;


	// Get the size of the string containing the information log for the failed shader compilation message.
	g_openGL->glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);

	// Increment the size by one to handle also the null terminator.
	logSize++;

	// Create a char buffer to hold the info log.
	infoLog = new char[logSize];
	if(!infoLog)
	{
		return;
	}

	// Now retrieve the info log.
	g_openGL->glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<logSize; i++)
	{
		fout << infoLog[i];
	}

	// Close the file.
	fout.close();

	// Convert the shader filename to a wide character string.
	error = mbstowcs_s(&convertedChars, newString, 128, shaderFilename, 128);
	if(error != 0)
	{
		return;
	}

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	ErrorMessage("Error compiling shader.  Check shader-error.txt for message.", ERROR_GRAPHICS);
}

void RenderShader::OutputLinkerErrorMessage(unsigned int programId)
{
	int logSize, i;
	char* infoLog;
	ofstream fout;


	// Get the size of the string containing the information log for the failed shader compilation message.
	g_openGL->glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logSize);

	// Increment the size by one to handle also the null terminator.
	logSize++;

	// Create a char buffer to hold the info log.
	infoLog = new char[logSize];
	if(!infoLog)
	{
		return;
	}

	// Now retrieve the info log.
	g_openGL->glGetProgramInfoLog(programId, logSize, NULL, infoLog);

	// Open a file to write the error message to.
	fout.open("linker-error.txt");

	// Write out the error message.
	for(i=0; i<logSize; i++)
	{
		fout << infoLog[i];
	}

	// Close the file.
	fout.close();

	// Pop a message up on the screen to notify the user to check the text file for linker errors.
	ErrorMessage("Error compiling linker.  Check linker-error.txt for message.", ERROR_GRAPHICS);
}

void RenderShader::ShutdownShader()
{
	// Detach the vertex and fragment shaders from the program.
	g_openGL->glDetachShader(m_shaderProgram, m_vertexShader);
	g_openGL->glDetachShader(m_shaderProgram, m_fragmentShader);

	// Delete the vertex and fragment shaders.
	g_openGL->glDeleteShader(m_vertexShader);
	g_openGL->glDeleteShader(m_fragmentShader);

	// Delete the shader program.
	g_openGL->glDeleteProgram(m_shaderProgram);
}

bool RenderShader::SetShaderParameters(float* worldMatrix, float* viewMatrix, float* projectionMatrix)
{
	unsigned int location;


	// Set the world matrix in the vertex shader.
	location = g_openGL->glGetUniformLocation(m_shaderProgram, "worldMatrix");
	if(location == -1)
	{
		return false;
	}
	g_openGL->glUniformMatrix4fv(location, 1, false, worldMatrix);

	// Set the view matrix in the vertex shader.
	location = g_openGL->glGetUniformLocation(m_shaderProgram, "viewMatrix");
	if(location == -1)
	{
		return false;
	}
	g_openGL->glUniformMatrix4fv(location, 1, false, viewMatrix);

	// Set the projection matrix in the vertex shader.
	location = g_openGL->glGetUniformLocation(m_shaderProgram, "projectionMatrix");
	if(location == -1)
	{
		return false;
	}
	g_openGL->glUniformMatrix4fv(location, 1, false, projectionMatrix);

	return true;
}
