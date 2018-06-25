#pragma once

#ifndef _MOEBIUS3D_INDEXED_GEOMETRY_H_
#define	_MOEBIUS3D_INDEXED_GEOMETRY_H_

#include "M3D_CommondInclude.h"
#include "ResourceCtrl.h"

struct Vertex
{
    float x, y, z;
};

Vertex _uPlaneVertices[] =
{
	{-0.5f, 0.5f, 0.0f },
	{ 0.5f, 0.5f, 0.0f },
	{-0.5f,-0.5f, 0.0f },
	{ 0.5f,-0.5f, 0.0f }
};

struct TexCoord
{
	float tu, tv;
};

TexCoord _uPlaneCoords[] =
{
	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },
	{ 0.0f, 1.0f },
	{ 1.0f, 1.0f },
};

template <typename vertexType> class CIndexedGeometry : public IResourceCtrl
{
public:
	CIndexedGeometry(void);
	~CIndexedGeometry(void);

	HRESULT	OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	HRESULT	OnLostDevice();
	void OnDisposeDevice();

	HRESULT Create(LPDIRECT3DDEVICE9 pDevice);

protected:

	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer;
	LPDIRECT3DVERTEXBUFFER9 m_TexCoord_01_Buffer;

	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;

	LPDIRECT3DINDEXBUFFER9  m_pIB;
};

template <typename vertexType> CIndexedGeometry<vertexType>::CIndexedGeometry(void)
{
	m_VertexBuffer = NULL; 
	m_TexCoord_01_Buffer = NULL;
}

template <typename vertexType> CIndexedGeometry<vertexType>::~CIndexedGeometry(void)
{

}

template <typename vertexType> HRESULT CIndexedGeometry<vertexType>::Create(LPDIRECT3DDEVICE9 pDevice)
{
	//Create the vertex buffer,...
	pDevice->CreateVertexBuffer(4 * sizeof(Vertex), D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &m_VertexBuffer, NULL); 
	void* pVertex = NULL; 
	m_VertexBuffer->Lock(0, 4 * sizeof(Vertex), &pVertex, D3DLOCK_DISCARD);
	memcpy( pVertices, _uPlaneVertices, sizeof(_uPlaneVertices));
	m_VertexBuffer->Unlock();

	//Create the vertex buffer,...
	pDevice->CreateVertexBuffer(4 * sizeof(TexCoord), D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &m_TexCoord_01_Buffer, NULL); 
	void* pTex = NULL; 
	m_TexCoord_01_Buffer->Lock(0, 4 * sizeof(TexCoord), &pTex, D3DLOCK_DISCARD);
	memcpy( pTex, _uPlaneCoords, sizeof(_uPlaneCoords));
	m_TexCoord_01_Buffer->Unlock();

	//Create the vertex declaration
	D3DVERTEXELEMENT9 dwDecl[] = 
	{
		//  Stream  Offset         Type                   Method                 Usage          Usage Index       
		{     0,      0,     D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0      },
		{     0,      12,    D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     0      },
		D3DDECL_END()
	};
	pDevice->CreateVertexDeclaration(dwDecl, &m_pVertexDeclaration);

}

template <typename vertexType> HRESULT CIndexedGeometry<vertexType>::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	return S_OK;
}

template <typename vertexType> HRESULT CIndexedGeometry<vertexType>::OnLostDevice()
{
	//Release the vertex buffer
	SAFE_RELEASE(m_VertexBuffer);
	SAFE_RELEASE(m_TexCoord_01_Buffer);

	//Release the index buffer
	SAFE_RELEASE(m_pIB);
	return S_OK;
}
template <typename vertexType> void CIndexedGeometry<vertexType>::OnDisposeDevice()
{
	//Release the vertex buffer
	SAFE_RELEASE(m_VertexBuffer);
	SAFE_RELEASE(m_TexCoord_01_Buffer);

	//Release the index buffer
	SAFE_RELEASE(m_pIB);
}

#endif
