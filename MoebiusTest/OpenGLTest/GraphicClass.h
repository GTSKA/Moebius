#pragma once
#include "ModelClass.h"
#include "Camera.h"
#include "OpenGLClass.h"
namespace MoebiusEngine 
{
	class Shader;
}
class GraphicClass
{
public:
	GraphicClass();
	~GraphicClass();
	bool Initialize(MoebiusEngine::OpenGLClass*);
	void shutdown();
	bool frame();
	void update(float);

private:
	bool render();
	MoebiusEngine::OpenGLClass* m_OpenGL;
	MoebiusEngine::Camera* m_camera;
	ModelClass* m_Model;
	
};

