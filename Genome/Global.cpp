#include "Global.h"

HWND	g_hwnd;
OpenGL*	g_openGL	= 0;

int g_screenWidth	= 1024;
int g_screenHeight	= 768;
float g_screenDepth = 1000.0f;
float g_screenNear	= 0.1f;
bool g_fullScreen	= false;
bool g_vsync		= false;


extern void ErrorMessage(char* message, char* messageType)
{
	std::cout << "ERROR: " << messageType << "\n\t" << message;
};