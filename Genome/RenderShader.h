////////////////////////////////////////////////////////////////////////////////
// Filename: rendershader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDERSHADER_H_
#define _RENDERSHADER_H_


//////////////
// INCLUDES //
//////////////
#include <fstream>
using namespace std;


///////////////////////
// INCLUDES //
///////////////////////
#include "global.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: RenderShader
//
// Loads in render shader files and manages them.
////////////////////////////////////////////////////////////////////////////////
class RenderShader
{
public:
	RenderShader();
	~RenderShader();

	bool Initialize();
	void Shutdown();
	void SetShader();
	bool SetShaderParameters(float*, float*, float*);

private:
	bool InitializeShader(char*, char*);
	char* LoadShaderSourceFile(char*);
	void OutputShaderErrorMessage(unsigned int, char*);
	void OutputLinkerErrorMessage(unsigned int);
	void ShutdownShader();

private:
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
	unsigned int m_shaderProgram;
};

#endif
