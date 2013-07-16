#include "system.h"

#define PRINT			"print"
#define LOAD			"load"
#define HELP			"help"
#define QUIT			"quit"
#define OPENGL			"opengl"

System::System()
{
	m_genome = 0;
	m_graphics = 0;
}

System::~System()
{

}

bool System::Initialize()
{
	m_genome = new Genome;


	return true;
}

bool System::InitializeGraphics()
{
	m_graphics = new Graphics;

	m_graphics->Initialize();

	if(!InitializeWindows())
		return false;

	return true;
}

bool System::InitializeWindows()
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	bool result;

	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = APPLICATION_NAME;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Create a temporary window for the OpenGL extension setup.
	g_hwnd = CreateWindowEx(WS_EX_APPWINDOW, APPLICATION_NAME, APPLICATION_NAME, WS_POPUP,
				0, 0, 640, 480, NULL, NULL, m_hinstance, NULL);
	if(g_hwnd == NULL)
	{
		return false;
	}

	// Don't show the window.
	ShowWindow(g_hwnd, SW_HIDE);
	
	// Initialize a temporary OpenGL window and load the OpenGL extensions.
	result = m_graphics->GetOpenGL()->InitializeExtensions();
	if(!result)
	{
		MessageBox(g_hwnd, L"Could not initialize the OpenGL extensions.", L"Error", MB_OK);
		return false;
	}

	// Release the temporary window now that the extensions have been initialized.
	DestroyWindow(g_hwnd);
	g_hwnd = NULL;

	// Determine the resolution of the clients desktop screen.
	g_screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	g_screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)g_screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)g_screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		g_screenWidth  = 1024;
		g_screenHeight = 768;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - g_screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - g_screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	g_hwnd = CreateWindowEx(WS_EX_APPWINDOW, APPLICATION_NAME, APPLICATION_NAME, 
				WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_OVERLAPPED | 
				WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
				posX, posY, g_screenWidth, g_screenHeight, NULL, NULL, m_hinstance, NULL);
	if(g_hwnd == NULL)
	{
		return false;
	}
	
	// Initialize OpenGL now that the window has been created.
	result = m_graphics->GetOpenGL()->InitializeOpenGL(g_screenWidth, g_screenHeight, SCREEN_DEPTH, SCREEN_NEAR, VSYNC_ENABLED);
	if(!result)
	{
		MessageBox(g_hwnd, L"Could not initialize OpenGL, check if your video card supports OpenGL 4.0.", L"Error", MB_OK);
		return false;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(g_hwnd, SW_SHOW);
	SetForegroundWindow(g_hwnd);
	SetFocus(g_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);
}

void System::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(g_hwnd);
	g_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(APPLICATION_NAME, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;
}

LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

void System::Shutdown()
{
	ShutdownWindows();

	if(m_genome)
	{
		m_genome->Shutdown();
		delete m_genome;
		m_genome = 0;
	}

	if(m_graphics)
	{
		m_graphics->Shutdown();
		delete m_graphics;
		m_graphics = 0;
	}
}

void System::Run()
{
	cout << "The Genome Project\n\n";
	cout <<	"\n\n\nEnter a command. Type help for a list of commands.\n\n> ";

	bool done = false;
	while(!done)
	{
		string input;
		string command[2]	= {"", ""};

		getline(cin, input);

		// Break apart commands.
		istringstream iss(input);
		int i = 0;
		while(iss && i < 2)
		{
			iss >> command[i++];
		}

		// If ExecuteCommand returns true the user has quit.
		done = ExecuteCommand(command, i);
	}
}

bool System::ExecuteCommand(string* commands, int commandCount)
{
	cout << "\n";

	// Convert to Lower case.
	for(int i = 0; i < commandCount; i++)
	{
		std::transform(commands[i].begin(), commands[i].end(), commands[i].begin(), ::tolower);
	}

	if(commands[0] == HELP)
	{
		cout << "COMMAND LIST";
		cout << "\nLOAD FILE\t\tLoad a plain text file.";
		cout << "\nPRINT NUMBER\t\tPrint every % NUMBER.";
		cout << "\nOPENGL\t\t\tInitialize and launch open gl window.";
		cout << "\nQUIT\t\t\tExit the application.";
	}
	else if(commands[0] == LOAD)
	{
		if(m_genome->LoadFromTextFile((char*)commands[1].c_str()))
		{
			cout << "File loaded.";
		}
		else
		{
			cout << "Unable to load file!";
		}
	}
	else if(commands[0] == PRINT)
	{
		if(IsNumber(commands[1]))
			m_genome->Print(stoi(commands[1]));
	}
	else if(commands[0] == OPENGL)
	{
		InitializeGraphics();
	}
	else if(commands[0] == QUIT)
	{
		return true;
	}
	else
	{
		cout << "\n\nTry that again. Help lists available commands.";
	}

	cout << "\n\n> ";

	return false;
}