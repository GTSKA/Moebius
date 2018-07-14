#pragma once
#include "ColorShaderClass.h"
#include "TextureShaderClass.h"
#include "ModelClass.h"
#include "CameraClass.h"
class ObjectClass
{
public:
	ObjectClass();
	~ObjectClass();
	void init(OpenGLClass*, int, ModelClass*, ColorShaderClass*, TextureShaderClass*);
	void render(OpenGLClass*,CameraClass*);
	void shutdown(OpenGLClass*);
private:
	void initVAO(OpenGLClass*);
	void renderbuffer(OpenGLClass*);

	ModelClass* m_model;
	ColorShaderClass* m_colorshader;
	TextureShaderClass* m_textureshader;
	GLuint m_VAO;
	int m_shadertype;
};

