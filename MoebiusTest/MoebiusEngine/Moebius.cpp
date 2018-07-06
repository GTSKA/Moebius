#include "Moebius.h"
#include <stdio.h>
#include "Vertex.h"
#include "inputManager.h"
#include "Globals.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include "SceneManager.h"
#include "ResourceManager.h"





namespace MoebiusEngine
{
	Moebius* Moebius::m_pInstance = NULL;
	Moebius::Moebius()
	{
		m_pInstance = NULL;
#ifdef OPENGL_PROJECT
		m_OpenGL = NULL;
#else
		m_Direct3D = NULL;
#endif
		m_camera = 0;
		m_model = 0;
		m_Shader = 0;
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
#ifdef OPENGL_PROJECT
	bool Moebius::init(int screenWidth, int screenHeight, HWND hwnd, OpenGLClass* openGL)
#else
	bool Moebius::init(int screenWidth, int screenHeight, HWND hwnd)
#endif
	{
		//Create Direct3D Object
#ifdef OPENGL_PROJECT
		m_OpenGL = openGL;
#else
		m_Direct3D = new D3DClass;
		if (!m_Direct3D)
			return false;
		//initialize the Direct3D object
		if (!m_Direct3D->init(screenWidth, screenHeight, Globals::VSYNC_ENABLED, hwnd, Globals::FULL_SCREEN, Globals::SCREEN_DEPTH, Globals::SCREEN_NEAR))
		{
			OutputDebugStringA("Could not initialize Direct3D : Error");
			return false;
		}
#endif
		bool resourceManagerResult;
#ifdef OPENGL_PROJECT
		// RMTerrain
		resourceManagerResult = ResourceManager::GetInstance()->Init("../Resources/RMOpenGL.txt", m_OpenGL);
#else
		
		resourceManagerResult = ResourceManager::GetInstance()->Init("../Resources/RM.txt", m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());
#endif
		if (!resourceManagerResult)
		{
			OutputDebugStringA("Error while loading resources\n");
			return false;
		}
		bool sceneManagerResult;
#ifdef OPENGL_PROJECT
		sceneManagerResult = SceneManager::GetInstance()->Init("../Resources/SMOpenGL.txt", m_OpenGL);
#else
		sceneManagerResult = SceneManager::GetInstance()->Init("../Resources/SM.txt");
#endif
		if (!sceneManagerResult)
		{
			OutputDebugStringA("Error while loading scene\n");
			return false;
		}



		return true;
	}
	void Moebius::update(float deltaTime)
	{
		SceneManager::GetInstance()->Update(deltaTime);			
	}
	void Moebius::shutdown() 
	{
		if (m_camera)
		{
			delete m_camera;
			m_camera = 0;
		}
		if (m_model)
		{
			m_model->shutdown();
			delete m_model;
			m_model = 0;
		}
		if (m_Shader)
		{
			m_Shader->shutdown();
			delete m_Shader;
			m_Shader = 0;
		}

		SceneManager::GetInstance()->Clean();
		SceneManager::DestroyInstance();
		ResourceManager::GetInstance()->Clean();
		ResourceManager::DestroyInstance();
#ifdef OPENGL_PROJECT
		m_OpenGL = 0;
#else
		if (m_Direct3D)
		{
			m_Direct3D->shutdown();
			delete m_Direct3D;
			m_Direct3D = NULL;
		}
#endif
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
#ifdef OPENGL_PROJECT
		m_OpenGL->BeginScene(0.0f, 0.2f, 0.4f, 1.0f);
		SceneManager::GetInstance()->Draw(m_OpenGL);
		m_OpenGL->EndScene();
#else
		m_Direct3D->beginScene(0.0f, 0.2f, 0.4f, 1.0f);
	
		SceneManager::GetInstance()->Draw(m_Direct3D->GetDeviceContext());
		// Present the rendered scene to the screen.
		m_Direct3D->endScene();
#endif
		return true;
	}
}
