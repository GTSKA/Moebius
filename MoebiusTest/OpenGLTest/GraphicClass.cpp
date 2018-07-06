#include "GraphicClass.h"
#define OPENGL_PROJECT


GraphicClass::GraphicClass()
{
	m_camera = 0;
	m_Model = 0;
}


GraphicClass::~GraphicClass()
{
}

bool GraphicClass::Initialize(MoebiusEngine::OpenGLClass* OpenGL)
{
	m_OpenGL = OpenGL;

	m_camera = new MoebiusEngine::Camera;
	if (!m_camera)
		return false;
	m_camera->Init();
	m_Model = new ModelClass;
	if (!m_Model)
		return false;
	
	
	
	return true;
}
void GraphicClass::shutdown()
{
	
	if (m_Model)
	{
		m_Model->shutdown();
		delete m_Model;
		m_Model = 0;
	}
	if (m_camera)
	{
		delete m_camera;
		m_camera = 0;
	}
	

}

bool GraphicClass::frame()
{
	if (!render())
		return false;
	return true;
}

bool GraphicClass::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	m_OpenGL->BeginScene(0.0f, 0.2f, 0.4f, 1.0f);


	m_OpenGL->EndScene();
	
	
	return true;
}

void GraphicClass::update(float deltaTime)
{
	
}

