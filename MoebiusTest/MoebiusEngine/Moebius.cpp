#include "Moebius.h"
#include <stdio.h>
#ifdef WIN32
#include <Windows.h>
#endif

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

namespace MoebiusEngine
{
	Moebius* Moebius::m_pInstance = NULL;
	Moebius::Moebius()
	{
		m_pInstance = NULL;
		m_Direct3D = NULL;
	}
	Moebius::~Moebius()
	{
		
	}
	Moebius* Moebius::getInstance()
	{
		if (m_pInstance == NULL)
			m_pInstance = new Moebius;
		return m_pInstance;
	}
	void Moebius::destroyInstance()
	{
		if (m_pInstance) 
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
	bool Moebius::init(int screenWidth, int screenHeight, HWND hwnd)
	{
		//Create Direct3D Object
		m_Direct3D = new D3DClass;
		if (!m_Direct3D)
			return false;
		//initialize the Direct3D object
		if (!m_Direct3D->init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
		{
			OutputDebugStringA("Could not initialize Direct3D : Error");
			return false;
		}

		return true;
	}
	void Moebius::shutdown() 
	{
		if (m_Direct3D)
		{
			m_Direct3D->shutdown();
			delete m_Direct3D;
			m_Direct3D = NULL;
		}
		return;
	}
	bool Moebius::Frame()
	{
		bool res;
		res = render();
		if (!res)
		{
			return false;
		}
		return true;
	}
	bool Moebius::render()
	{
		// Clear the buffers to begin the scene
		m_Direct3D->beginScene(1,0,0,1);

		// Present the rendered scene to the screen.
		m_Direct3D->endScene();
		return true;
	}
}
