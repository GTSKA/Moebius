#include "Plane.h"
#include "Vertex.h"
#include <d3d11.h>

Plane::Plane()
{
}


Plane::~Plane()
{
}
void Plane::Init(UINT width, UINT height, ID3D11Device* dev, ID3D11DeviceContext* devcon, int Id)
{
	m_id = Id;
	Init(width, height, dev, devcon);
}
void Plane::Init(UINT width, UINT height, ID3D11Device* dev, ID3D11DeviceContext* devcon)
{
	VERTEX* modelVertex;
	UINT* modelIndices;
	m_numVertex = height*width;
	modelVertex = new VERTEX[height*width];
	m_numIndices = (height - 1)*(width - 1) * 6;
	modelIndices = new UINT[(height - 1)*(width - 1) * 6];
	float twidth = 1.0f/(float)width;
	float theight = 1.0f/(float)height;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			modelVertex[i*width + j].pos = Vector3(i*theight, j*twidth, 0.0f);
		}
	}
	for (int i = 0; i < height-1; ++i)
	{
		for (int j = 0; j < width-1; ++j)
		{
			
			modelIndices[i * 6 * (width - 1) + j * 6] = i * width + j;
			modelIndices[i * 6 * (width - 1) + j * 6 + 1] = width*(i+1)+j;
			modelIndices[i * 6 * (width - 1) + j * 6 + 2] = i*width + j + 1;

			modelIndices[i * 6 * (width - 1) + j * 6 + 3] = width*(i + 1) + j;
			modelIndices[i * 6 * (width - 1) + j * 6 + 4] = width*(i + 1) + j +1;
			modelIndices[i * 6 * (width - 1) + j * 6 + 5] = i*width + j + 1;
		}
	}

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

	delete[] modelIndices;
	delete[] modelVertex;
}
