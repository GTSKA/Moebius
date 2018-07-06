#pragma once
#ifdef OPENGL_PROJECT
#include "OpenGLClass.h"
#else
#include "D3DClass.h"
#endif
#include "Camera.h"
#include "Model.h"
#include "Shader.h"


namespace MoebiusEngine 
{
	class Moebius
	{
	protected:
		Moebius();
		~Moebius();

		Moebius(const Moebius&) {}
		Moebius& operator=(const Moebius &) {}
	public:
		static Moebius* getInstance();
		static void destroyInstance();
#ifdef OPENGL_PROJECT
		bool init(int screenWidth, int screenHeight, HWND hwnd, OpenGLClass* openGL);
#else
		bool init(int screenWidth, int screenHeight, HWND hwnd);
#endif
		void shutdown();
		bool Frame();
		bool render();
		void update(float deltaTime);
	protected:

		static Moebius* m_pInstance;
#ifdef OPENGL_PROJECT
		OpenGLClass* m_OpenGL;
#else
		D3DClass* m_Direct3D;
#endif
		Camera* m_camera;
		Model* m_model;
		Shader* m_Shader;
		

	};
}

