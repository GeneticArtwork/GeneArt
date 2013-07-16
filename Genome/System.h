////////////////////////////////////////////////////////////////////////////////
// Filename: system.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

//////////////
// INCLUDES //
//////////////
#include "Graphics.h"
#include "Genome.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: System
////////////////////////////////////////////////////////////////////////////////
class System
{
public:
	System();
	~System();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	bool InitializeGraphics();
	bool InitializeWindows();
	bool ExecuteCommand(string* commands, int commandCount);
	void ShutdownWindows();

private:
	Genome*		m_genome;
	Graphics*	m_graphics;
	HINSTANCE	m_hinstance;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static System* ApplicationHandle = 0;

#endif