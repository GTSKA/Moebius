#include "ObjectClass.h"



ObjectClass::ObjectClass()
{
}


ObjectClass::~ObjectClass()
{
}

void ObjectClass::init(OpenGLClass* openGL, int shadertype, ModelClass* model, ColorShaderClass* colorShader, TextureShaderClass* textureShader)
{
	m_shadertype = shadertype;
	m_model = model;
	m_textureshader = textureShader;
	m_colorshader = colorShader;
	initVAO(openGL);
}

void ObjectClass::initVAO(OpenGLClass* openGL)
{
	// Allocate an OpenGL vertex array object.
	openGL->glGenVertexArrays(1, &m_VAO);

	// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	openGL->glBindVertexArray(m_VAO);

		// Enable the two vertex array attributes.
	openGL->glEnableVertexAttribArray(0);  // Vertex position.
	openGL->glEnableVertexAttribArray(1); // Vertex texcoord
	openGL->glBindBuffer(GL_ARRAY_BUFFER, m_model->getVertexArray());
	openGL->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType), 0);
	if (m_shadertype == 2)
	{
		openGL->glBindBuffer(GL_ARRAY_BUFFER, m_model->getVertexArray());
		openGL->glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (6 * sizeof(float)));
	}
	if (m_shadertype == 1)
	{
		openGL->glBindBuffer(GL_ARRAY_BUFFER, m_model->getVertexArray());
		openGL->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (3 * sizeof(float)));
	}
}

void ObjectClass::render(OpenGLClass* openGL, CameraClass* camera)
{
	float worldMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];
	openGL->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	openGL->GetProjectionMatrix(projectionMatrix);
	if (m_shadertype == 1)
	{
		m_colorshader->SetShader(openGL);
		m_colorshader->SetShaderParameters(openGL, worldMatrix, viewMatrix, projectionMatrix);
	}
	if (m_shadertype == 2)
	{
		m_textureshader->SetShader(openGL);
		m_textureshader->SetShaderParameters(openGL, worldMatrix, viewMatrix, projectionMatrix, 0);
	}
	
	renderbuffer(openGL);
}

void ObjectClass::shutdown(OpenGLClass* openGL)
{
	openGL->glDisableVertexAttribArray(0);
	openGL->glDisableVertexAttribArray(1);

	// Release the vertex array object.
	openGL->glBindVertexArray(0);
	openGL->glDeleteVertexArrays(1, &m_VAO);
}

void ObjectClass::renderbuffer(OpenGLClass* openGL)
{
	openGL->glBindVertexArray(m_VAO);

	openGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->getIndexArray());
	// Render the vertex buffer using the index buffer.
	glDrawElements(GL_TRIANGLES, m_model->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
}