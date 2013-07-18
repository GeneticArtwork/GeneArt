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
#include <vector>
#include "OpenGL.h"

#define APPLICATION_NAME		L"GeneArt"

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
	float x, y;
};

struct vector3d
{
	vector3d(float ix, float iy, float iz) {x = ix; y = iy; z = iz;};
	vector3d(){x = y = z = 0.0f;};
	float x, y, z;
};

#endif