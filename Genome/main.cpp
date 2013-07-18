////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
//
// This executes on program launch and should have access
// to all program .h files.
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <tchar.h>

#include "system.h"


int _tmain(int argc, _TCHAR* argv[])
{
	System* system = new System;

	// -- The main program loop. -- //
	if(system->Initialize())
		system->Run();

	// Shutdown the program and free all memory.
	system->Shutdown();
	delete system;
	system = 0;

	return 0;
}