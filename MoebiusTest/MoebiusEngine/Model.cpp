#include "Model.h"
#include <stdio.h>
#include <d3d11.h>
#include "Vertex.h"
namespace MoebiusEngine
{
	Model::Model()
	{
		m_id = 0;
		m_vertexBuffer = 0;
		m_indexBuffer = 0;
	}


	Model::~Model()
	{
		
	}
#ifdef OPENGL_PROJECT
	Model::Model(OpenGLClass* openGL)
	{
		m_id = 0;
		m_vertexBuffer = 0;
		m_indexBuffer = 0;
		m_OpenGL = openGL;
	}
	MODEL_ERROR Model::InitModel(char* filename, unsigned int Id)
	{
		m_id = Id;
		return InitModel(filename);
	}
#else
	MODEL_ERROR Model::InitModel(char* filename, unsigned int Id, ID3D11Device* dev)
	{
		m_id = Id;
		return InitModel(filename, dev);
	}
#endif

#ifdef OPENGL_PROJECT
	MODEL_ERROR Model::InitModel(char* filename)
#else
	MODEL_ERROR Model::InitModel(char* filename, ID3D11Device* dev)
#endif
	{
		if (filename == NULL)
			return MODEL_NAME_ERROR;
#ifdef OPENGL_PROJECT
#else
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
#endif
		FILE* modelFile;
		fopen_s(&modelFile, filename, "r");
		if (modelFile == NULL)
			return MODEL_FILE_ERROR;
		fscanf_s(modelFile, "%*s %d", &m_numVertex);
		VERTEX* modelVertex;
		modelVertex = new VERTEX[m_numVertex];
		m_MaxU = 0;
		Vector3 m_minPoint(0, 0, 0);
		Vector3 m_maxPoint(0, 0, 0);
		for (int i = 0; i < m_numVertex; ++i)
		{
			fscanf_s(modelFile, "   %*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];", &modelVertex[i].pos.x, &modelVertex[i].pos.y, &modelVertex[i].pos.z,
				&modelVertex[i].normal.x, &modelVertex[i].normal.y, &modelVertex[i].normal.z, &modelVertex[i].binormal.x, &modelVertex[i].binormal.y, &modelVertex[i].binormal.z,
				&modelVertex[i].tgt.x, &modelVertex[i].tgt.y, &modelVertex[i].tgt.z, &modelVertex[i].uvs.x, &modelVertex[i].uvs.y);
			m_MaxU = (m_MaxU < modelVertex[i].uvs.x) ? modelVertex[i].uvs.x : m_MaxU;
			m_minPoint.x = (m_minPoint.x > modelVertex[i].pos.x) ? modelVertex[i].pos.x : m_minPoint.x;
			m_minPoint.y = (m_minPoint.y > modelVertex[i].pos.y) ? modelVertex[i].pos.y : m_minPoint.y;
			m_minPoint.z = (m_minPoint.z > modelVertex[i].pos.z) ? modelVertex[i].pos.z : m_minPoint.z;
			m_maxPoint.x = (m_maxPoint.x < modelVertex[i].pos.x) ? modelVertex[i].pos.x : m_maxPoint.x;
			m_maxPoint.y = (m_maxPoint.y < modelVertex[i].pos.y) ? modelVertex[i].pos.y : m_maxPoint.y;
			m_maxPoint.z = (m_maxPoint.z < modelVertex[i].pos.z) ? modelVertex[i].pos.z : m_maxPoint.z;
		}
		if (ferror(modelFile))
			return MODEL_VERTEX_ERROR;
		m_center = Vector3((m_minPoint.x + m_maxPoint.x) / 2.0f, (m_minPoint.y + m_maxPoint.y) / 2.0f, (m_minPoint.z + m_maxPoint.z) / 2.0f);
		m_size = Vector3((m_maxPoint.x - m_minPoint.x) / 2.0f, (m_maxPoint.y - m_minPoint.y) / 2.0f, (m_maxPoint.z - m_minPoint.z) / 2.0f);
#ifdef OPENGL_PROJECT
		m_OpenGL->glGenBuffers(1, &m_vertexBuffer);
		m_OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		m_OpenGL->glBufferData(GL_ARRAY_BUFFER, m_numVertex * sizeof(VERTEX), modelVertex, GL_STATIC_DRAW);

#else
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;                // write access access by CPU and GPU
		bd.ByteWidth = sizeof(VERTEX) * m_numVertex;             // size is the VERTEX struct * 3
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		vertexData.pSysMem = modelVertex;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;


		if (FAILED(dev->CreateBuffer(&bd, &vertexData, &m_vertexBuffer))) // create the buffer
		{
			return MODEL_VERTEX_ERROR;
		}
#endif

		fscanf_s(modelFile, "%*s %d", &m_numIndices);
		unsigned int* modelIndices;
		modelIndices = new unsigned int[m_numIndices];
		for (int i = 0; i < m_numIndices; i += 3)
			fscanf_s(modelFile, "%*u.    %u,    %u,    %u   ", &modelIndices[i], &modelIndices[i + 1], &modelIndices[i + 2]);
		if (ferror(modelFile))
			return MODEL_INDEX_ERROR;

#ifdef OPENGL_PROJECT
		m_OpenGL->glGenBuffers(1, &m_indexBuffer);
		m_OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_indexBuffer);
		m_OpenGL->glBufferData(GL_ARRAY_BUFFER, m_numIndices * sizeof(unsigned int), modelIndices, GL_STATIC_DRAW);
		m_OpenGL->glBindBuffer(GL_ARRAY_BUFFER, 0);
#else

		D3D11_BUFFER_DESC bd2;
		bd2.Usage = D3D11_USAGE_DEFAULT;
		bd2.ByteWidth = sizeof(unsigned int) * m_numIndices;
		bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd2.MiscFlags = 0;
		bd2.CPUAccessFlags = 0;
		bd2.StructureByteStride = 0;


		indexData.pSysMem = modelIndices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;


		if (FAILED(dev->CreateBuffer(&bd2, &indexData, &m_indexBuffer)))
		{
			return MODEL_INDEX_ERROR;
		}
#endif
		fclose(modelFile);
		delete[] modelIndices;
		delete[] modelVertex;
		return MODEL_SUCCESS;
	}
#ifdef OPENGL_PROJECT
	MODEL_ERROR Model::InitTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Vector4 color)
#else
	MODEL_ERROR Model::InitTriangle(Vector3 v1, Vector3 v2, Vector3 v3, Vector4 color, ID3D11Device* dev)
#endif
	{
		
#ifdef OPENGL_PROJECT
#else
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
#endif
		VERTEX modelVertex[3];
		unsigned int modelIndices[3];
		modelVertex[0].pos = v1;
		modelVertex[1].pos = v2;
		modelVertex[2].pos = v3;
		modelVertex[0].color = modelVertex[1].color = modelVertex[2].color = color;

#ifdef OPENGL_PROJECT
#else
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		m_numVertex = 3;
		bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bd.ByteWidth = sizeof(VERTEX) * m_numVertex;             // size is the VERTEX struct * 3
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

		vertexData.pSysMem = modelVertex;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		       
		if (FAILED(dev->CreateBuffer(&bd, &vertexData, &m_vertexBuffer))) // create the buffer
		{
			return MODEL_VERTEX_ERROR;
		}
#endif



		modelIndices[0] = 0;
		modelIndices[1] = 1;
		modelIndices[2] = 2;

#ifdef OPENGL_PROJECT
#else
		m_numIndices = 3;
		D3D11_BUFFER_DESC bd2;
		bd2.Usage = D3D11_USAGE_DYNAMIC;
		bd2.ByteWidth = sizeof(unsigned int) * m_numIndices;
		bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd2.MiscFlags = 0;

		indexData.pSysMem = modelIndices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		
		if (FAILED(dev->CreateBuffer(&bd2, &indexData, &m_indexBuffer)))
		{
			return MODEL_INDEX_ERROR;
		}
#endif
		return MODEL_SUCCESS;
	}
#ifdef OPENGL_PROJECT
	GLuint Model::getIndexBuffer()
	{
		return m_indexBuffer;
	}
	GLuint Model::getVertexBuffer()
	{
		return m_vertexBuffer;
	}
#else
	ID3D11Buffer* Model::getIndexBuffer()
	{
		return m_indexBuffer;
	}

	ID3D11Buffer* Model::getVertexBuffer()
	{
		return m_vertexBuffer;
	}
#endif

	unsigned int Model::getIndexSize()
	{
		return m_numIndices;
	}

	unsigned int Model::getVertexSize()
	{
		return m_numVertex;
	}

	unsigned int Model::getId()
	{
		return m_id;
	}
	float Model::getMaxU()
	{
		return m_MaxU;
	}
	Vector3 Model::getCenter()
	{
		return m_center;
	}
	Vector3 Model::getSize()
	{
		return m_size;
	}
	void Model::shutdown()
	{
#ifdef OPENGL_PROJECT
		// Disable the two vertex array attributes.
		m_OpenGL->glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_OpenGL->glDeleteBuffers(1, &m_vertexBuffer);
		m_vertexBuffer = NULL;

		m_OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		m_OpenGL->glDeleteBuffers(1, &m_indexBuffer);
		m_indexBuffer = NULL;
		
#else
		if (m_vertexBuffer)
		{
			m_vertexBuffer->Release();
			m_vertexBuffer = 0;
		}
		if (m_indexBuffer)
		{
			m_indexBuffer->Release();
			m_indexBuffer = 0;
		}
#endif
	}
}
