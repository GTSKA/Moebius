#include "SystemGUI.h"
#include "inputManager.h"
#include "Globals.h"



#pragma comment(lib, "opengl32.lib")
SystemGUI::SystemGUI()
{
}


SystemGUI::~SystemGUI()
{
}

bool SystemGUI::Init()
{
	int screenW, screenH;
	screenW = screenH = 0;
	

	if (!InitializeWindows(screenW, screenH))
		return false;
	MoebiusEngine::InputManager::getInstance()->init();

	m_openGL = new MoebiusEngine::OpenGLClass;

	m_Graphics = new GraphicClass;
	if (!m_Graphics)
		return false;
	if (!m_Graphics->Initialize(m_openGL))
		return false;

	return true;
}

void SystemGUI::shutdown()
{
	if (m_Graphics)
	{
		m_Graphics->shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}
	
	MoebiusEngine::InputManager::destroyInstance();
	shutdownWindows();
}

void SystemGUI::Run()
{
	MSG msg;
	bool done, result;

	//Initialize the message structure
	ZeroMemory(&msg, sizeof(MSG));

	//Loop until there is a quit message from the window or user
	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//if windows signals to end the application exit out
		if (msg.message == WM_QUIT)
			done = true;
		else
		{
			UpdateTime();
			Update();
			result = Frame();
			if (!result)
				done = true;
		}
	}
	return;
}
bool SystemGUI::Frame()
{
	if (!m_Graphics->frame())
		return false;
	return true;
}

LRESULT CALLBACK SystemGUI::MessageHandler(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
	{
		MoebiusEngine::InputManager::getInstance()->keyDown((unsigned int)wParam);
	}
	break;
	case WM_KEYUP:
	{
		MoebiusEngine::InputManager::getInstance()->keyUp((unsigned int)wParam);
	}
	break;
	default:
		return DefWindowProc(hwnd, umsg, wParam, lParam);
		break;
	}
}

bool SystemGUI::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	bool result;


	// Get an external pointer to this object.	
	AppHandle = this;

	// Get the instance of this application.
	m_hInstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Create a temporary window for the OpenGL extension setup.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_POPUP,
		0, 0, 640, 480, NULL, NULL, m_hInstance, NULL);
	if (m_hwnd == NULL)
	{
		return false;
	}

	// Don't show the window.
	ShowWindow(m_hwnd, SW_HIDE);
	//////////////////////////////OPENGL Context init
	m_openGL->InitializeExtensions(m_hwnd);
	/*HDC deviceContext;
	PIXELFORMATDESCRIPTOR pixelFormat;
	int error;
	HGLRC renderContext;

	// Get the device context for this window.
	deviceContext = GetDC(m_hwnd);
	if (!deviceContext)
		return false;
	// Set a temporary default pixel format.
	error = SetPixelFormat(deviceContext, 1, &pixelFormat);
	if (error != 1)
		return false;

	// Create a temporary rendering context.
	renderContext = wglCreateContext(deviceContext);
	if (!renderContext)
		return false;

	// Set the temporary rendering context as the current rendering context for this window.
	error = wglMakeCurrent(deviceContext, renderContext);
	if (error != 1)
		return false;*/

	////////////////////////////////////////////
	

	

	// Release the temporary window now that the extensions have been initialized.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (Globals::FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);
	if (m_hwnd == NULL)
	{
		return false;
	}

	
	

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return true;
}

void SystemGUI::shutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (Globals::FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = NULL;

	// Release the pointer to this class.
	AppHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
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
		return AppHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}

void SystemGUI::UpdateTime()
{
	//Take the frec read
	QueryPerformanceCounter(&g_L1);

	//Obtain delta frec
	long long Diff = g_L1.QuadPart - g_L0.QuadPart;

	//obtaint delta time in sec
	g_DeltaTime = (float)Diff / Fq.QuadPart;

	//Update current read
	g_L0 = g_L1;
}
void SystemGUI::Update()
{
	if(m_Graphics)
		m_Graphics->update(g_DeltaTime);
}
