#pragma once
#include "OpenGLClass.h"
#include "GraphicClass.h"

class SystemGUI
{
public:
	SystemGUI();
	~SystemGUI();

	bool Init();
	void shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void UpdateTime();
	void Update();
	LARGE_INTEGER g_L1;
	LARGE_INTEGER g_L0;
	LARGE_INTEGER Fq;
	float g_DeltaTime;
	bool InitializeWindows(int&, int&);

	void shutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	MoebiusEngine::OpenGLClass* m_openGL;
	GraphicClass* m_Graphics;

};
/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/////////////
// GLOBALS //
/////////////
static SystemGUI* AppHandle = 0;

