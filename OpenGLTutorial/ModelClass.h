#pragma once
#include "OpenGLClass.h"
#include "TextureClass.h"
struct VertexType
{
	float x, y, z;
	float r, g, b;
	float tu, tv;
};
class ModelClass
{
public:
	ModelClass();
	~ModelClass();
	bool Initialize(OpenGLClass*, char*, unsigned int, bool,int);
	void Shutdown(OpenGLClass*);
	void Render(OpenGLClass*);
	unsigned int getVertexArray();
	unsigned int getIndexArray();
	int getIndexCount();
	int getVertexCount();

private:
	bool InitializeBuffers(OpenGLClass*,int);
	void ShutdownBuffers(OpenGLClass*);
	void RenderBuffers(OpenGLClass*);

	bool LoadTexture(OpenGLClass*, char*, unsigned int, bool);
	void ReleaseTexture();

	int m_vertexCount, m_indexCount;
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;

	TextureClass* m_Texture;
	
};

