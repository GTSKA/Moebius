#include "Model.h"
#include "Vertex.h"
#include <stdio.h>
#include <malloc.h>
#include <d3d11.h>

Model::Model()
{
	m_id = 0;
}


Model::~Model()
{
	if(m_vertexBuffer)
		m_vertexBuffer->Release();
	if(m_indexBuffer)
		m_indexBuffer->Release();
}

MODEL_ERROR Model::InitModel(char* filename, unsigned int Id, ID3D11Device* dev, ID3D11DeviceContext* devcon)
{
	m_id = Id;
	return InitModel(filename, dev ,devcon);
}

MODEL_ERROR Model::InitModel(char* filename, ID3D11Device* dev, ID3D11DeviceContext* devcon)
{
	if (filename == NULL)
		return MODEL_NAME_ERROR;
	FILE* modelFile;
	fopen_s(&modelFile,filename, "r");
	if (modelFile == NULL)
		return MODEL_FILE_ERROR;
	fscanf_s(modelFile, "%*s %d", &m_numVertex);
	VERTEX* modelVertex;
	modelVertex = new VERTEX[m_numVertex];
	m_MaxU = 0;
	for (int i = 0; i < m_numVertex; ++i)
	{
		fscanf_s(modelFile, "   %*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];", &modelVertex[i].pos.x, &modelVertex[i].pos.y, &modelVertex[i].pos.z,
			&modelVertex[i].normal.x, &modelVertex[i].normal.y, &modelVertex[i].normal.z, &modelVertex[i].binormal.x, &modelVertex[i].binormal.y, &modelVertex[i].binormal.z,
			&modelVertex[i].tgt.x, &modelVertex[i].tgt.y, &modelVertex[i].tgt.z, &modelVertex[i].uvs.x, &modelVertex[i].uvs.y);
		m_MaxU = (m_MaxU < modelVertex[i].uvs.x) ? modelVertex[i].uvs.x : m_MaxU;
	}
	if (ferror(modelFile))
		return MODEL_VERTEX_ERROR;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * m_numVertex;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	dev->CreateBuffer(&bd, NULL, &m_vertexBuffer);       // create the buffer


												   // copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(m_vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, modelVertex, sizeof(VERTEX) * m_numVertex);                 // copy the data
	devcon->Unmap(m_vertexBuffer, NULL);                                      // unmap the buffer

	fscanf_s(modelFile, "%*s %d", &m_numIndices);
	unsigned int* modelIndices;
	modelIndices = new unsigned int[m_numIndices];
	for(int i =0;i < m_numIndices;i+=3)
		fscanf_s(modelFile, "%*u.    %u,    %u,    %u   ", &modelIndices[i], &modelIndices[i + 1], &modelIndices[i + 2]);
	if (ferror(modelFile))
		return MODEL_INDEX_ERROR;

	D3D11_BUFFER_DESC bd2;
	bd2.Usage = D3D11_USAGE_DYNAMIC;
	bd2.ByteWidth = sizeof(unsigned int) * m_numIndices;
	bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd2.MiscFlags = 0;

	dev->CreateBuffer(&bd2, NULL, &m_indexBuffer);

	D3D11_MAPPED_SUBRESOURCE ms2;
	devcon->Map(m_indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms2);
	memcpy(ms2.pData, modelIndices, sizeof(unsigned int) * m_numIndices);
	devcon->Unmap(m_indexBuffer, NULL);


	fclose(modelFile);
	delete[] modelIndices;
	delete[] modelVertex;
	return MODEL_SUCCESS;
}

ID3D11Buffer* Model::getIndexBuffer()
{
	return m_indexBuffer;
}

ID3D11Buffer* Model::getVertexBuffer()
{
	return m_vertexBuffer;
}

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