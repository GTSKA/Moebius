#include "ModelClass.h"



ModelClass::ModelClass()
{
}


ModelClass::~ModelClass()
{
}

bool ModelClass::initialize()
{
	if (!InitializeBuffers())
		return false;
	return true;
}

void ModelClass::shutdown()
{
	shutdownbuffers();
}

void ModelClass::render()
{
	renderBuffers();
}

bool ModelClass::InitializeBuffers()
{
	VERTEX* vertices;
	unsigned int* indices;

	m_vertexcount = 3;
	m_indexCount = 3;
	vertices = new VERTEX[m_vertexcount];
	if (!vertices)
		return false;

	indices = new unsigned int[m_indexCount];
	if (!indices)
		return false;

	vertices[0].pos = Vector3(-1, -1, 0);
	vertices[0].color = Vector4(1, 0, 0, 1);

	vertices[1].pos = Vector3(0, 1, 0);
	vertices[1].color = Vector4(0, 1, 0, 1);

	vertices[2].pos = Vector3(1, -1, 0);
	vertices[2].color = Vector4(0, 0, 1, 1);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	
	

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void ModelClass::shutdownbuffers()
{
	// Disable the two vertex array attributes.
	
}

void ModelClass::renderBuffers()
{
	
}