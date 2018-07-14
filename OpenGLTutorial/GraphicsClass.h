#pragma once
#include "OpenGLClass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"
#include "TextureShaderClass.h"
#include "ObjectClass.h"
/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
class GraphicsClass
{
public:
	GraphicsClass();
	~GraphicsClass();

	bool Initialize(OpenGLClass*, HWND);
	void Shutdown();
	bool Frame();
	bool Frame(int);

private:
	bool Render();
	bool Render(int);
	OpenGLClass* m_OpenGL;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ModelClass* m_Model2;
	ColorShaderClass* m_ColorShader;
	TextureShaderClass* m_TextureShader;
	ObjectClass* object1;
	ObjectClass* object2;
};

