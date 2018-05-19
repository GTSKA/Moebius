#include "Quad.h"
#include "Vertex.h"
#include "Shaders.h"
#include <d3d11.h>

Quad::Quad()
{
}


Quad::~Quad()
{
	m_IndexBuffer->Release();
	m_IndexBuffer = NULL;
	m_vertexBuffer->Release();
	m_vertexBuffer = NULL;
}

ID3D11Buffer* Quad::getIndexBuffer()
{
	return m_IndexBuffer;
}
ID3D11Buffer* Quad::getVertexBuffer()
{
	return m_vertexBuffer;
}
void Quad::Init(ID3D11Device* dev, ID3D11DeviceContext* devcon)
{
	VERTEX quad_vertices[4];
	quad_vertices[0].pos.x = -1.0; quad_vertices[0].pos.y = 1.0; quad_vertices[0].pos.z = 0.0;
	quad_vertices[0].uvs.x = 0.0; quad_vertices[0].uvs.y = 1.0;
	quad_vertices[1].pos.x = -1.0; quad_vertices[1].pos.y = -1.0; quad_vertices[1].pos.z = 0.0;
	quad_vertices[1].uvs.x = 0.0; quad_vertices[1].uvs.y = 0.0;
	quad_vertices[2].pos.x = 1.0; quad_vertices[2].pos.y = -1.0; quad_vertices[2].pos.z = 0.0;
	quad_vertices[2].uvs.x = 1.0; quad_vertices[2].uvs.y = 0.0;
	quad_vertices[3].pos.x = 1.0; quad_vertices[3].pos.y = 1.0; quad_vertices[3].pos.z = 0.0;
	quad_vertices[3].uvs.x = 1.0; quad_vertices[3].uvs.y = 1.0;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 4;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	dev->CreateBuffer(&bd, NULL, &m_vertexBuffer);       // create the buffer


														 // copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(m_vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, quad_vertices, sizeof(VERTEX) * 4);                 // copy the data
	devcon->Unmap(m_vertexBuffer, NULL);                                      // unmap the buffer

	unsigned int quad_indices[6];
	quad_indices[0] = 0; quad_indices[1] = 1; quad_indices[2] = 2;
	quad_indices[3] = 0; quad_indices[4] = 2; quad_indices[5] = 3;

	D3D11_BUFFER_DESC bd2;
	bd2.Usage = D3D11_USAGE_DYNAMIC;
	bd2.ByteWidth = sizeof(unsigned int) * 6;
	bd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd2.MiscFlags = 0;

	dev->CreateBuffer(&bd2, NULL, &m_indexBuffer);

	D3D11_MAPPED_SUBRESOURCE ms2;
	devcon->Map(m_indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms2);
	memcpy(ms2.pData, quad_indices, sizeof(unsigned int) * 6);
	devcon->Unmap(m_indexBuffer, NULL);

}

void Quad::Draw(Shaders* Shader, ID3D11DeviceContext* devcon)
{
	devcon->VSSetShader(Shader->getVertexShader(), 0, 0);
	devcon->IASetInputLayout(Shader->getVertexBufferLayout());
	UINT stride = sizeof(VERTEX);
	UINT offset;
	if (Shader->positionAttribute != -1)
	{
		offset = 0;
		devcon->IASetVertexBuffers(Shader->positionAttribute, 1, &m_vertexBuffer, &stride, &offset);
	}
	if (Shader->colorAttribute != -1)
	{
		offset = 3 * sizeof(float);
		devcon->IASetVertexBuffers(Shader->colorAttribute, 1, &m_vertexBuffer, &stride, &offset);
	}
	if (Shader->normalAttribute != -1)
	{
		offset = 7 * sizeof(float);
		devcon->IASetVertexBuffers(Shader->normalAttribute, 1, &m_vertexBuffer, &stride, &offset);
	}
	if (Shader->binormalAttribute != -1)
	{
		offset = 10 * sizeof(float);
		devcon->IASetVertexBuffers(Shader->binormalAttribute, 1, &m_vertexBuffer, &stride, &offset);
	}
	if (Shader->tangentAttribute != -1)
	{
		offset = 13 * sizeof(float);
		devcon->IASetVertexBuffers(Shader->tangentAttribute, 1, &m_vertexBuffer, &stride, &offset);
	}
	if (Shader->uvAttribute != -1)
	{
		offset = 16 * sizeof(float);
		devcon->IASetVertexBuffers(Shader->uvAttribute, 1, &m_vertexBuffer, &stride, &offset);
	}
	devcon->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(6, 0, 0);
}