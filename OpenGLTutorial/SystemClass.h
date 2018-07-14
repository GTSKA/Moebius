#pragma once
#include<windows.h>
#include "OpenGLClass.h"
#include "GraphicsClass.h"
#include "InputClass.h"
class SystemClass
{
public:
	SystemClass();
	~SystemClass();
	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	bool InitializeWindows(OpenGLClass*, int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	OpenGLClass* m_OpenGL;
	GraphicsClass* m_Graphics;
	InputClass* m_Input;
};

/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;