#pragma once
#include "Vertex.h"
#ifndef GLuint
typedef unsigned int GLuint;
#endif
namespace MoebiusEngine
{
	class OpenGLClass;
}
class ModelClass
{
public:
	ModelClass();
	~ModelClass();
	bool initialize();
	void shutdown();
	void render();

private:
	bool InitializeBuffers();
	void shutdownbuffers();
	void renderBuffers();

	int m_vertexcount, m_indexCount;
	GLuint m_vertexArrayObject, m_vertexBuffer, m_indexBuffer;
};

