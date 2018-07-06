#include "Plane.h"
#include "Vertex.h"
#include <d3d11.h>

namespace MoebiusEngine
{
	Rectangle::Rectangle()
	{
	}


	Rectangle::~Rectangle()
	{
	}
#ifdef OPENGL_PROJECT
	Rectangle::Rectangle(OpenGLClass* OpenGL)
	{
		m_OpenGL = OpenGL;
	}
	MODEL_ERROR Rectangle::Init(UINT width, UINT height, int Id)
	{
		m_id = Id;
		return Init(width, height);
	}
#else
	MODEL_ERROR Rectangle::Init(UINT width, UINT height, ID3D11Device* dev, int Id)
	{
		m_id = Id;
		return Init(width, height, dev);
	}
#endif
#ifdef OPENGL_PROJECT
	MODEL_ERROR Rectangle::Init(UINT width, UINT height)
#else
	MODEL_ERROR Rectangle::Init(UINT width, UINT height, ID3D11Device* dev)
#endif
	{
		VERTEX* modelVertex;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		UINT* modelIndices;
		m_numVertex = height*width;
		modelVertex = new VERTEX[height*width];
		m_numIndices = (height - 1)*(width - 1) * 6;
		modelIndices = new UINT[(height - 1)*(width - 1) * 6];
		float twidth = 1.0f / (float)width;
		float theight = 1.0f / (float)height;
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				modelVertex[i*width + j].pos = Vector3(i*theight, j*twidth, 0.0f);
			}
		}
		for (int i = 0; i < height - 1; ++i)
		{
			for (int j = 0; j < width - 1; ++j)
			{

				modelIndices[i * 6 * (width - 1) + j * 6] = i * width + j;
				modelIndices[i * 6 * (width - 1) + j * 6 + 1] = width*(i + 1) + j;
				modelIndices[i * 6 * (width - 1) + j * 6 + 2] = i*width + j + 1;

				modelIndices[i * 6 * (width - 1) + j * 6 + 3] = width*(i + 1) + j;
				modelIndices[i * 6 * (width - 1) + j * 6 + 4] = width*(i + 1) + j + 1;
				modelIndices[i * 6 * (width - 1) + j * 6 + 5] = i*width + j + 1;
			}
		}
#ifdef OPENGL_PROJECT
		m_OpenGL->glGenBuffers(1, &m_vertexBuffer);
		m_OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		m_OpenGL->glBufferData(GL_ARRAY_BUFFER, m_numVertex * sizeof(VERTEX), modelVertex, GL_STATIC_DRAW);

		m_OpenGL->glGenBuffers(1, &m_indexBuffer);
		m_OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_indexBuffer);
		m_OpenGL->glBufferData(GL_ARRAY_BUFFER, m_numIndices * sizeof(unsigned int), modelIndices, GL_STATIC_DRAW);
		m_OpenGL->glBindBuffer(GL_ARRAY_BUFFER, 0);
#else
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bd.ByteWidth = sizeof(VERTEX) * m_numVertex;             // size is the VERTEX struct * 3
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

		vertexData.pSysMem = modelVertex;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		HRESULT hr;
		hr = dev->CreateBuffer(&bd, &vertexData, &m_vertexBuffer);       // create the buffer
		if (FAILED(hr))
			return MODEL_VERTEX_ERROR;

		

		D3D11_BUFFER_DESC bd2;
		bd2.Usage = D3D11_USAGE_DYNAMIC;
		bd2.ByteWidth = sizeof(unsigned int) * m_numIndices;
		bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd2.MiscFlags = 0;

		indexData.pSysMem = modelIndices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		hr = dev->CreateBuffer(&bd2, &indexData, &m_indexBuffer);
		if (FAILED(hr))
			return MODEL_INDEX_ERROR;
#endif

		delete[] modelIndices;
		delete[] modelVertex;
		return MODEL_SUCCESS;
	}

	void Rectangle::disconnect()
	{
		m_vertexBuffer = NULL;
		m_indexBuffer = NULL;
	}
}
