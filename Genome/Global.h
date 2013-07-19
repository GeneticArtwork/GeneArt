////////////////////////////////////////////////////////////////////////////////
// Filename: global.h
//
// Include global variables here. Declare them as extern and defined them
// in the .cpp file.
////////////////////////////////////////////////////////////////////////////////
#ifndef _GLOBAL_H_
#define _GLOBAL_H_


///////////////
// INCLUDES //
//////////////
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include "OpenGL.h"

#define APPLICATION_NAME		L"GeneArt"

///////////////
// Directories //
//////////////
#define OBJECT_DIRECTORY		"../Data/Objects/"

///////////////
// Variables //
//////////////

// Application window settings.
extern HWND	g_hwnd;						// Hardware handle for open gl window.

// The openGL object used. Graphics initializes and controls the object, however a number of other classes
// including shader classes needs direct access as well.
extern OpenGL*	g_openGL;

////////////////////////////////
// Graphic Settings
///////////////////////////////
extern int g_screenWidth;				// Resolution of window.
extern int g_screenHeight;
extern float g_screenDepth;
extern float g_screenNear;
extern bool g_fullScreen;
extern bool g_vsync;

////////////////////////////////////
//	Math structures and functions.
////////////////////////////////////
#define RADIAN 0.0174532925f

struct vector2d
{
	vector2d(float x, float y) {};
	vector2d(){x = y = 0.0f;};
	float x, y;
};

struct vector3d
{
	vector3d(float ix, float iy, float iz) {x = ix; y = iy; z = iz;};
	vector3d(){x = y = z = 0.0f;};
	float x, y, z;
};

////////////////////////////////////
//	Misc functions
////////////////////////////////////
#define ERROR_GRAPHICS		"GRAPHICS ERROR"

extern void ErrorMessage(char* message, char* messageType);


#endif