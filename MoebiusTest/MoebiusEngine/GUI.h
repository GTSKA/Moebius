#pragma once
#include <windows.h>
#ifdef OPENGL_PROJECT
#include "OpenGLClass.h"
#endif
	class GUI
	{
	public:
		GUI();
		GUI(const GUI&);
		~GUI();

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
#ifdef OPENGL_PROJECT
		bool InitializeWindows(MoebiusEngine::OpenGLClass*, int&, int&);
#else
#endif
		void shutdownWindows();

		LPCWSTR m_applicationName;
		HINSTANCE m_hInstance;
		HWND m_hwnd;
#ifdef OPENGL_PROJECT
		MoebiusEngine::OpenGLClass* m_OpenGL;
#else
#endif
	};
	/////////////////////////
	// FUNCTION PROTOTYPES //
	/////////////////////////
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	/////////////
	// GLOBALS //
	/////////////
	static GUI* ApplicationHandle = 0;



