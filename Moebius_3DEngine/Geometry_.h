#pragma once

#ifndef _MOEBIUS3D_GEOMETRY_H_
#define	_MOEBIUS3D_GEOMETRY_H_

#include "M3D_CommondInclude.h"
#include "ResourceCtrl.h"
#include "FileManager_.h"

#define V_OK(x) x != S_OK ? MessageBox(NULL, L"Error", L"Error", 0); return E_FAIL;

typedef enum __MOEBIUS_GEOMETRY_TYPE
{
	_MOEBIUS_GEOMTYPE_EMPTY						= 0,
	_MOEBIUS_GEOMTYPE_QUAD_STRIP				= 1,
	_MOEBIUS_GEOMTYPE_QUAD_LIST					= 2,
	_MOEBIUS_GEOMTYPE_PLANE						= 3,
}__MOEBIUS_GEOMETRY_TYPE;

typedef enum __MOEBIUES_GEOMETRY_RESTORE_SOURCE
{
	_MOEBIUS_GEOM_RESTORE_SOURCE_VERTEXLIST			= 0,
	_MOEBIUS_GEOM_RESTORE_SOURCE_VERTEXBUFFER		= 1,
	_MOEBIUS_GEOM_RESTORE_SOURCE_FILE				= 2,
	_MOEBIUS_GEOM_RESTORE_SOURCE_FUNCTION			= 3,
}__MOEBIUES_GEOMETRY_RESTORE_SOURCE;

struct _MoebiusVertex_puvc 
{
	D3DXVECTOR3 position;	//Vertex position
	FLOAT tu, tv;			//Vertex texture coordinates
	D3DXVECTOR4	color;		//Vertex color
   
	/*enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_TEX1
	};*/
};

template <typename vertexType> class CGeometry_ : public IResourceCtrl
{
public:

	__MOEBIUS_GEOMETRY_TYPE m_type;
	LPWSTR					m_texFilePath;

	CGeometry_(void);
	~CGeometry_(void);

	void Define(__MOEBIUS_GEOMETRY_TYPE type, DWORD FVF, UINT nVertices, D3DXVECTOR3 v0, D3DXVECTOR3 v1);

	UINT GetNumVertices();
	UINT GetNumFaces();
	UINT GetNumQuads();

	HRESULT Clear();

	HRESULT	OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	HRESULT	OnLostDevice();
	void OnDisposeDevice();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pDevice);

	virtual HRESULT OpenFromOBJ(LPDIRECT3DDEVICE9 pDevice, STRSAFE_LPWSTR fileName);

	virtual HRESULT AddQuad(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 v0, D3DXVECTOR3 v1);
	virtual HRESULT AddQuad(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR4 color);

	virtual HRESULT AddQuad(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3);
	virtual HRESULT AddQuad(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3, D3DXVECTOR4 color);
	virtual HRESULT AddQuad(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3, D3DXVECTOR4 color0, D3DXVECTOR4 color1, D3DXVECTOR4 color2, D3DXVECTOR4 color3);

	virtual HRESULT AddLine(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 Start, D3DXVECTOR3 End, float thick);
	virtual HRESULT AddLine(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 Start, D3DXVECTOR3 End, D3DXVECTOR4 color, float thick);
	virtual HRESULT AddLine(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 Start, D3DXVECTOR3 End);
	virtual HRESULT AddLine(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 Start, D3DXVECTOR3 End, D3DXVECTOR4 color);
	
	virtual HRESULT EditLine(LPDIRECT3DDEVICE9 pDevice,UINT nLine,D3DXVECTOR3 Start, D3DXVECTOR3 End);

	virtual HRESULT DeleteLine(LPDIRECT3DDEVICE9 pDevice, UINT nLine);


	virtual void Render(LPDIRECT3DDEVICE9 pDevice);

	virtual void Render(LPDIRECT3DDEVICE9 pDevice, UINT nQuad);
	virtual HRESULT DeleteQuad(LPDIRECT3DDEVICE9 pDevice, UINT nQuad);

	virtual HRESULT EditQuad(LPDIRECT3DDEVICE9 pDevice, UINT nQuad, D3DXVECTOR3 v0, D3DXVECTOR3 v1);
	virtual HRESULT EditQuad(LPDIRECT3DDEVICE9 pDevice, UINT nQuad, D3DXVECTOR4 color);

	virtual BOOL RayIntersect(D3DXVECTOR3 vRayOrig, D3DXVECTOR3 vPickRayDir);
	virtual BOOL RayIntersect(D3DXVECTOR3 vRayOrig, D3DXVECTOR3 vPickRayDir, UINT* pQuad);

	virtual BOOL RayIntersect_Quad(UINT nQuad, D3DXVECTOR3 vRayOrig, D3DXVECTOR3 vPickRayDir);

	HRESULT Reset(LPDIRECT3DDEVICE9 pDevice);

protected:

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DVERTEXDECLARATION9	m_pVertexDeclaration;
	LPDIRECT3DINDEXBUFFER9		m_pIndexBuffer;	

	CArray_<vertexType> m_vertexList;
	CArray_<WORD> m_indexList;

	UINT m_nVertices;
	UINT m_nFaces;
	UINT m_nQuads;
	float m_thick;

	virtual HRESULT InitVertexBuffer(LPDIRECT3DDEVICE9 pDevice);
};

template <typename vertexType> CGeometry_<vertexType>::CGeometry_(void)
{
	m_pVB = NULL;
	m_pIndexBuffer = NULL;
	m_pVertexDeclaration = NULL;
	m_nVertices = 0;
	m_nFaces = 0;
	m_nQuads = 0;
}

template <typename vertexType> void CGeometry_<vertexType>::Define(__MOEBIUS_GEOMETRY_TYPE type, DWORD FVF, UINT nVertices, D3DXVECTOR3 v0, D3DXVECTOR3 v1)
{
	m_pVB = NULL;
	m_pIndexBuffer = NULL;
	m_pVertexDeclaration = NULL;

	m_type = type;
	m_nVertices = 0;
	m_nFaces = 0;
	m_nQuads = 0;
}

template <typename vertexType> HRESULT CGeometry_<vertexType>::InitVertexBuffer(LPDIRECT3DDEVICE9 pDevice)
{
	if(m_vertexList.GetSize() > 0 && m_indexList.GetSize() > 0)
	{
		if(FAILED(pDevice->CreateVertexBuffer(m_vertexList.GetSize() * sizeof(vertexType),
			D3DUSAGE_DYNAMIC,
			0,
			D3DPOOL_DEFAULT, 
			&m_pVB, 
			NULL)))
			{
				MessageBox(NULL, L"Error in: CreateVertexBuffer", L"ERROR", MB_OK|MB_ICONERROR);
				return E_FAIL;
			}

		if(FAILED(pDevice->CreateIndexBuffer(m_indexList.GetSize() * sizeof(WORD), 
			D3DUSAGE_DYNAMIC, 
			D3DFMT_INDEX16, 
			D3DPOOL_DEFAULT, 
			&m_pIndexBuffer, NULL)))
		{
			MessageBox(NULL, L"Error in: CreateIndexBuffer", L"ERROR", MB_OK|MB_ICONERROR);
				return E_FAIL;
		}

		m_nVertices = m_indexList.GetSize();
		m_nFaces = (UINT)(m_nVertices / 3.0f);
		m_nQuads = (UINT)(m_nFaces / 2.0f);

		//Create the vertex declaration
		D3DVERTEXELEMENT9 dwDecl[] = 
		{
			//  Stream  Offset         Type						Method                 Usage			Usage Index       
			{     0,      0,     D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_POSITION,		0      },
			{     0,      12,    D3DDECLTYPE_FLOAT2,	D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,		0      },
			{     0,      20,    D3DDECLTYPE_FLOAT4,	D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_COLOR,			0      },
			D3DDECL_END()
		};

		if(FAILED(pDevice->CreateVertexDeclaration(dwDecl, &m_pVertexDeclaration)))
		{
			MessageBox(NULL, L"Error in: CreateVertexDeclaration", L"ERROR", MB_OK|MB_ICONERROR);
				return E_FAIL;
		}
	}
	return S_OK;
}



template <typename vertexType> HRESULT CGeometry_<vertexType>::Create(LPDIRECT3DDEVICE9 pDevice)
{
	////Release the index buffer
	SAFE_RELEASE(m_pVB);

	////Release the vertex buffer
	SAFE_RELEASE(m_pIndexBuffer);

	////Release the vertex declaration
	SAFE_RELEASE(m_pVertexDeclaration);

	if(m_vertexList.GetSize() == 0 && m_indexList.GetSize() == 0)
	{
		m_nVertices = m_indexList.GetSize();
		m_nFaces = (UINT)(m_nVertices / 3.0f);
		m_nQuads = (UINT)(m_nFaces / 2.0f);

		return S_OK;
	}

	if(FAILED(InitVertexBuffer(pDevice)))
	{
		MessageBox(NULL, L"Error Initializing the vertex buffer", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	//Now fill the vertex buffer,...
	//_MoebiusVertex_puvc *v = NULL;
	vertexType *v = NULL;
	if(FAILED(m_pVB->Lock(0, 0, (void**)&v, 0)))
	{
		MessageBox(NULL, L"Error in: LPDIRECT3DVERTEXBUFFER9::Lock", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}
	m_vertexList.GetData(v);

	if(FAILED(m_pVB->Unlock()))
	{
		MessageBox(NULL, L"Error in: LPDIRECT3DVERTEXBUFFER9::UnLock", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	WORD *indexB = NULL;
	if(FAILED(m_pIndexBuffer->Lock(0, m_indexList.GetSize() * sizeof(WORD) , (void**)&indexB, 0)))
	{
		MessageBox(NULL, L"Error in: LPDIRECT3DINDEXBUFFER9::Lock", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}
	m_indexList.GetData(indexB);
	m_pIndexBuffer->Unlock();

	return S_OK;
}

template <typename vertexType> void CGeometry_<vertexType>::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if(m_pIndexBuffer != NULL && m_pVB != NULL && m_nVertices > 0 && m_nFaces > 0)
	{
		pDevice->SetIndices(m_pIndexBuffer);
		pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(_MoebiusVertex_puvc));
		pDevice->SetVertexDeclaration(m_pVertexDeclaration);
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				0,
				0,
				m_nVertices,
				0,
				m_nFaces);
	}
}
template <typename vertexType> void CGeometry_<vertexType>::Render(LPDIRECT3DDEVICE9 pDevice, UINT nQuad)
{
	if(nQuad < m_nQuads && m_pIndexBuffer != NULL && m_pVB != NULL)
	{
		pDevice->SetIndices(m_pIndexBuffer);
		pDevice->SetStreamSource( 0, m_pVB, 0, sizeof(_MoebiusVertex_puvc));
		pDevice->SetVertexDeclaration(m_pVertexDeclaration);
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				0,
				0,
				4,
				nQuad * 6,
				2);
	}
}


template <typename vertexType> HRESULT CGeometry_<vertexType>::OpenFromOBJ(LPDIRECT3DDEVICE9 pDevice, STRSAFE_LPWSTR fileName)
{
	SAFE_DELETE(m_texFilePath);
	m_texFilePath = new WCHAR[MAX_PATH];
	if(FAILED(FindFile(m_texFilePath, MAX_PATH, fileName)))
	{
		return E_FAIL;				
	}
	//MessageBox(NULL, m_texFilePath , L"File founded!!!",0);

	wifstream InFile(m_texFilePath);
	if( !InFile )
	{
		return E_FAIL;
	}
	else
	{
		WCHAR strCommand[256] = {0};

		CArray_<D3DXVECTOR3> positions;
		CArray_<D3DXVECTOR3> texCoords;
		//CArray_<D3DXVECTOR3> normals;

		for(;;)
		{
			//----------------------------------------------------------------------------------------
			InFile >> strCommand;

			if( !InFile )
			{
				break;
			}

			//Comments
			if( 0 == wcscmp(strCommand, L"#"))
			{

			}
			
			//Vertex Position
			else if( 0 == wcscmp(strCommand, L"v"))
			{
				float x, y, z;
				InFile >> x >> y >> z;
				positions.Add(D3DXVECTOR3(x, y, z));
				
			
			}

			//Texture Coords
			else if( 0 == wcscmp(strCommand, L"vt"))
			{
				float u, v, w;
				InFile >> u >> v >> w;
				texCoords.Add(D3DXVECTOR3( u, v, w));
			}

			//else if( 0 == wcscmp( strCommand, L"vn" ) )
			//{
			//	//Normals
			//	float x, y, z;
			//	InFile >> x >> y >> z;
			//	normals.Add( D3DXVECTOR3( x, y, z ) );
			//}

			//Faces
			else if( 0 == wcscmp( strCommand, L"f" ) )
		    {
				// Face
				UINT iPosition, iTexCoord, iNormal;
				vertexType vertex;

				for( UINT iFace = 0; iFace < 3; iFace++ )
				{
					//Get the position index
					InFile >> iPosition;
					vertex.position = positions[iPosition - 1];
					InFile.ignore();

					//Get the texcoord index
					if( '/' != InFile.peek())
	                {
						InFile >> iTexCoord;
						//vertex.texcoord = texCoords[iTexCoord - 1];

						vertex.tu = texCoords[iTexCoord - 1].x;
						vertex.tv = texCoords[iTexCoord - 1].y;
					}

					InFile.ignore();
					//Get the texcoord index
					if( '/' != InFile.peek())
	                {
						InFile >> iNormal;
						//vertex.normal = normals[iNormal - 1];
					}


					vertex.color = D3DXVECTOR4(1,1,1,1);
					m_vertexList.Add(vertex);
				}
			}
			else
			{
				// Unimplemented or unrecognized command
			}
			InFile.ignore( 1000, '\n' );
		}

		for(UINT i=0; i<m_vertexList.GetSize(); i++)
		{
			//Now Fill the Index Buffer...
			m_indexList.Add(i);
		}

		/*WCHAR msg[255] = {0};
		StringCchPrintf(msg, 256 * sizeof(WCHAR),  L"Vertex Num: %d, TexCoord Num: %d, Faces:; %d", positions.GetSize(), texCoords.GetSize(), m_indexList.GetSize()/3);
		MessageBox(NULL, msg, L"- RESUME -", 0);*/

		//return S_OK;
		return Create(pDevice);
	}
}

template <typename vertexType> HRESULT CGeometry_<vertexType>::AddQuad(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 v0, D3DXVECTOR3 v1)
{
	//Vertex 01
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v0;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;

	//Vertex 02
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = D3DXVECTOR3(v0.x, v1.y, 0.0f);
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;

	//Vertex 03
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v1;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;
	
	//Vertex 04
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = D3DXVECTOR3(v1.x, v0.y, 0.0f);
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;

	//Triangle 1
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 3);
	m_indexList.Add(m_vertexList.GetSize() - 2);

	//Triangle 2
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 2);
	m_indexList.Add(m_vertexList.GetSize() - 1);

	return Create(pDevice);
}

template <typename vertexType> HRESULT CGeometry_<vertexType>::AddQuad(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR4 color)
{
	//Vertex 01
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v0;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	//Vertex 02
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = D3DXVECTOR3(v0.x, v1.y, 0.0f);
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	//Vertex 03
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v1;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;
	
	//Vertex 04
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = D3DXVECTOR3(v1.x, v0.y, 0.0f);
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	//Triangle 1
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 3);
	m_indexList.Add(m_vertexList.GetSize() - 2);

	//Triangle 2
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 2);
	m_indexList.Add(m_vertexList.GetSize() - 1);

	return Create(pDevice);
}

template <typename vertexType> HRESULT CGeometry_<vertexType>::AddQuad(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3)
{
	//Vertex 01
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v0;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;

	//Vertex 02
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v1;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;

	//Vertex 03
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v2;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;
	
	//Vertex 04
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v3;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;

	//Triangle 1
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 3);
	m_indexList.Add(m_vertexList.GetSize() - 2);

	//Triangle 2
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 2);
	m_indexList.Add(m_vertexList.GetSize() - 1);

	return Create(pDevice);
}

template <typename vertexType> HRESULT CGeometry_<vertexType>::AddQuad(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3, D3DXVECTOR4 color)
{
	//Vertex 01
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v0;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	//Vertex 02
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v1;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	//Vertex 03
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v2;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;
	
	//Vertex 04
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v3;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	//Triangle 1
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 3);
	m_indexList.Add(m_vertexList.GetSize() - 2);

	//Triangle 2
	m_indexList.Add(m_vertexList.GetSize() - 3);
	m_indexList.Add(m_vertexList.GetSize() - 1);
	m_indexList.Add(m_vertexList.GetSize() - 2);

	return Create(pDevice);
}

template <typename vertexType> HRESULT CGeometry_<vertexType>::AddQuad(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, D3DXVECTOR3 v3, D3DXVECTOR4 color0, D3DXVECTOR4 color1, D3DXVECTOR4 color2, D3DXVECTOR4 color3)
{
	//Vertex 01
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v0;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color0;

	//Vertex 02
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v1;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color1;

	//Vertex 03
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v2;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color2;
	
	//Vertex 04
	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = v3;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color3;

	//Triangle 1
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 3);
	m_indexList.Add(m_vertexList.GetSize() - 2);

	//Triangle 2
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 2);
	m_indexList.Add(m_vertexList.GetSize() - 1);

	return Create(pDevice);

}
template <typename vertexType> HRESULT CGeometry_<vertexType>::DeleteQuad(LPDIRECT3DDEVICE9 pDevice, UINT nQuad)
{
	if(m_nQuads <= 1)
	{
		////Release the index buffer
		SAFE_RELEASE(m_pVB);

		////Release the vertex buffer
		SAFE_RELEASE(m_pIndexBuffer);
		m_vertexList.RemoveAll();
		m_indexList.RemoveAll();
	}
	else
	{
		UINT minIndex = 6 * nQuad;
		UINT minVertex = 4 * nQuad;
		for(UINT i=0; i<6; i++)
		{
			if(i<4)
			{
				m_vertexList.Remove(minVertex);
			}
			m_indexList.Remove(minIndex);
		}

		for(UINT i=minIndex; i<m_indexList.GetSize(); i++)
		{
			m_indexList[i] -= 4;
		}
	}
	return Create(pDevice);
}


template <typename vertexType> HRESULT CGeometry_<vertexType>::EditQuad(LPDIRECT3DDEVICE9 pDevice, UINT nQuad, D3DXVECTOR3 v0, D3DXVECTOR3 v1)
{
	if(nQuad >=0 && nQuad < m_nQuads)
	{
		UINT index = 4 * nQuad;
		m_vertexList[index + 0].position = v0;
		m_vertexList[index + 1].position = D3DXVECTOR3(v0.x, v1.y, 0.0f);;
		m_vertexList[index + 2].position = v1;
		m_vertexList[index + 3].position = D3DXVECTOR3(v1.x, v0.y, 0.0f);


		//Lock the vertices,...
		vertexType *v = NULL;
		//if(FAILED(m_pVB->Lock((4 * nQuad) * sizeof(vertexType), 4 * sizeof(vertexType), (void**)&v, 0)))
		if(FAILED(m_pVB->Lock(0,0, (void**)&v, 0)))
		{
			MessageBox(NULL, L"Error in: LPDIRECT3DVERTEXBUFFER9::Lock", L"ERROR", MB_OK|MB_ICONERROR);
			return E_FAIL;
		}

		m_vertexList.GetData(v);

		if(FAILED(m_pVB->Unlock()))
		{
			MessageBox(NULL, L"Error in: LPDIRECT3DVERTEXBUFFER9::UnLock", L"ERROR", MB_OK|MB_ICONERROR);
			return E_FAIL;
		}
		return S_OK;
	}
	return E_FAIL;
}
template <typename vertexType> HRESULT CGeometry_<vertexType>::EditQuad(LPDIRECT3DDEVICE9 pDevice, UINT nQuad, D3DXVECTOR4 color)
{
	if(nQuad >=0 && nQuad < m_nQuads)
	{
		UINT index = 4 * nQuad;

		m_vertexList[index + 0].color = color;
		m_vertexList[index + 1].color = color;
		m_vertexList[index + 2].color = color;
		m_vertexList[index + 3].color = color;

		//Lock the vertices,...
		vertexType *v = NULL;
		//if(FAILED(m_pVB->Lock((4 * nQuad) * sizeof(vertexType), 4 * sizeof(vertexType), (void**)&v, 0)))
		if(FAILED(m_pVB->Lock(0,0, (void**)&v, 0)))
		{
			MessageBox(NULL, L"Error in: LPDIRECT3DVERTEXBUFFER9::Lock", L"ERROR", MB_OK|MB_ICONERROR);
			return E_FAIL;
		}

		m_vertexList.GetData(v);

		if(FAILED(m_pVB->Unlock()))
		{
			MessageBox(NULL, L"Error in: LPDIRECT3DVERTEXBUFFER9::UnLock", L"ERROR", MB_OK|MB_ICONERROR);
			return E_FAIL;
		}
		return S_OK;
	}
	return E_FAIL;
}
template <typename vertexType> BOOL CGeometry_<vertexType>::RayIntersect(D3DXVECTOR3 vRayOrig, D3DXVECTOR3 vPickRayDir)
{
	FLOAT u;
	FLOAT v;
	FLOAT dist;

	for(UINT i=0; i<m_indexList.GetSize(); i+=3)
	{
		if(D3DXIntersectTri(&m_vertexList[m_indexList[i + 0]].position,
			&m_vertexList[m_indexList[i + 1]].position,
			&m_vertexList[m_indexList[i + 2]].position, 
			&vRayOrig, 
			&vPickRayDir, 
			&u, 
			&v, 
			&dist))
		{
			return TRUE;
		}
	}
	return FALSE;
}

template <typename vertexType> BOOL CGeometry_<vertexType>::RayIntersect(D3DXVECTOR3 vRayOrig, D3DXVECTOR3 vPickRayDir, UINT* pQuad)
{
	FLOAT u;
	FLOAT v;
	FLOAT dist;

	UINT t=0;
	for(UINT i=0; i<m_indexList.GetSize(); i+=3)
	{
		if(D3DXIntersectTri(&m_vertexList[m_indexList[i + 0]].position,
			&m_vertexList[m_indexList[i + 1]].position,
			&m_vertexList[m_indexList[i + 2]].position, 
			&vRayOrig, 
			&vPickRayDir, 
			&u, 
			&v, 
			&dist))
		{
			if(t % 2 == 0)
			{
				*pQuad = t / 2;
			}
			else
			{
				*pQuad = (t -  1) / 2;
			}
			return TRUE;
		}
		t++;
	}
	return FALSE;
}

template <typename vertexType> BOOL CGeometry_<vertexType>::RayIntersect_Quad(UINT nQuad, D3DXVECTOR3 vRayOrig, D3DXVECTOR3 vPickRayDir)
{
	if(nQuad >= 0 && nQuad < m_nQuads)
	{
		UINT index = 6 * nQuad;
		FLOAT u;
		FLOAT v;
		FLOAT dist;

		if(D3DXIntersectTri(&m_vertexList[m_indexList[index + 0]].position,
				&m_vertexList[m_indexList[index + 1]].position,
				&m_vertexList[m_indexList[index + 2]].position, 
				&vRayOrig, 
				&vPickRayDir, 
				&u, 
				&v, 
				&dist))
		{
			return TRUE;
		}
		else if(D3DXIntersectTri(&m_vertexList[m_indexList[index + 3]].position,
				&m_vertexList[m_indexList[index + 4]].position,
				&m_vertexList[m_indexList[index + 5]].position, 
				&vRayOrig, 
				&vPickRayDir, 
				&u, 
				&v, 
				&dist))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}



template <typename vertexType> CGeometry_<vertexType>::~CGeometry_(void)
{
	//Release the index buffer
	SAFE_RELEASE(m_pVB);

	//Release the vertex buffer
	SAFE_RELEASE(m_pIndexBuffer);

	//Release the vertex declaration
	SAFE_RELEASE(m_pVertexDeclaration);
}


template <typename vertexType> HRESULT CGeometry_<vertexType>::Clear()
{
	//Release the vertex buffer
	SAFE_RELEASE(m_pVB);

	//Release the vertex buffer
	SAFE_RELEASE(m_pIndexBuffer);

	//Release the vertex declaration
	SAFE_RELEASE(m_pVertexDeclaration);

	m_indexList.RemoveAll();
	m_vertexList.RemoveAll();

	return S_OK;

}
template <typename vertexType> HRESULT CGeometry_<vertexType>::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	return Create(pDevice);
}
template <typename vertexType> HRESULT CGeometry_<vertexType>::OnLostDevice()
{
	//Release the vertex buffer
	SAFE_RELEASE(m_pVB);

	//Release the vertex buffer
	SAFE_RELEASE(m_pIndexBuffer);

	//Release the vertex declaration
	SAFE_RELEASE(m_pVertexDeclaration);

	return S_OK;
}
template <typename vertexType> void CGeometry_<vertexType>::OnDisposeDevice()
{
	//Delete the file path
	SAFE_DELETE(m_texFilePath);

	//Release the vertex buffer
	SAFE_RELEASE(m_pVB);	

	//Release the index buffer
	SAFE_RELEASE(m_pIndexBuffer);

	//Release the vertex declaration
	SAFE_RELEASE(m_pVertexDeclaration);
}

template <typename vertexType> UINT CGeometry_<vertexType>::GetNumVertices()
{
	return m_nVertices;
}


template <typename vertexType> UINT CGeometry_<vertexType>::GetNumFaces()
{
	return m_nFaces;
}


template <typename vertexType> UINT CGeometry_<vertexType>::GetNumQuads()
{
	return m_nQuads;
}

template <typename vertexType> HRESULT CGeometry_<vertexType>::AddLine(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 Start, D3DXVECTOR3 End)
{
    D3DXVECTOR3 z(0.0f,0.0f,1.0f);
	D3DXVECTOR3 w1,w2;
	w1 = End - Start;
	D3DXVec3Cross(&w2,&w1,&z);
	D3DXVec3Normalize(&w2,&w2);

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = Start + w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = Start - w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = End + w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = End - w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;

	//Triangle 1
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 1);
	m_indexList.Add(m_vertexList.GetSize() - 3);

	//Triangle 2
	m_indexList.Add(m_vertexList.GetSize() - 1);
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 2);

	return Create(pDevice);

}
template <typename vertexType> HRESULT CGeometry_<vertexType>::AddLine(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 Start, D3DXVECTOR3 End, D3DXVECTOR4 color)
{
    D3DXVECTOR3 z(0.0f,0.0f,1.0f);
	D3DXVECTOR3 w1,w2;
	w1 = End - Start;
	D3DXVec3Cross(&w2,&w1,&z);
	D3DXVec3Normalize(&w2,&w2);

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = Start + w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = Start - w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = End + w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = End - w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	//Triangle 1
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 1);
	m_indexList.Add(m_vertexList.GetSize() - 3);

	//Triangle 2
	m_indexList.Add(m_vertexList.GetSize() - 1);
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 2);

	return Create(pDevice);

}
template <typename vertexType> HRESULT CGeometry_<vertexType>::AddLine(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 Start, D3DXVECTOR3 End, D3DXVECTOR4 color, float thick)
{
    D3DXVECTOR3 z(0.0f,0.0f,1.0f);
	D3DXVECTOR3 w1,w2;
	w1 = End - Start;
	D3DXVec3Cross(&w2,&w1,&z);
	D3DXVec3Normalize(&w2,&w2);
	m_thick = thick;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = Start + w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = Start - w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = End + w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = End - w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].color = color;

	//Triangle 1
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 1);
	m_indexList.Add(m_vertexList.GetSize() - 3);

	//Triangle 2
	m_indexList.Add(m_vertexList.GetSize() - 1);
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 2);

	return Create(pDevice);
}
template <typename vertexType> HRESULT CGeometry_<vertexType>::AddLine(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 Start, D3DXVECTOR3 End, float thick)
{
    D3DXVECTOR3 z(0.0f,0.0f,1.0f);
	D3DXVECTOR3 w1,w2;
	w1 = End - Start;
	D3DXVec3Cross(&w2,&w1,&z);
	D3DXVec3Normalize(&w2,&w2);
	m_thick = thick;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = Start + w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = Start - w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 0.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = End + w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 0.0f;

	m_vertexList.Add();
	m_vertexList[m_vertexList.GetSize()-1].position = End - w2*0.5*m_thick;
	m_vertexList[m_vertexList.GetSize()-1].tu = 1.0f;
	m_vertexList[m_vertexList.GetSize()-1].tv = 1.0f;

	//Triangle 1
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 1);
	m_indexList.Add(m_vertexList.GetSize() - 3);

	//Triangle 2
	m_indexList.Add(m_vertexList.GetSize() - 1);
	m_indexList.Add(m_vertexList.GetSize() - 4);
	m_indexList.Add(m_vertexList.GetSize() - 2);


	return Create(pDevice);

}
template <typename vertexType> HRESULT CGeometry_<vertexType>::EditLine(LPDIRECT3DDEVICE9 pDevice,UINT nLine,D3DXVECTOR3 Start, D3DXVECTOR3 End)
{ 
	UINT index = nLine *4;
	D3DXVECTOR3 z(0.0f,0.0f,1.0f);
	D3DXVECTOR3 w1,w2;
	w1 = End - Start;
	D3DXVec3Cross(&w2,&w1,&z);
	D3DXVec3Normalize(&w2,&w2);

	m_vertexList[index + 0].position = Start + w2*0.5*m_thick;
	m_vertexList[index + 1].position = Start - w2*0.5*m_thick;
	m_vertexList[index + 2].position = End + w2*0.5*m_thick;
	m_vertexList[index + 3].position = End - w2*0.5*m_thick;
	return Create(pDevice);
}

template <typename vertexType> HRESULT CGeometry_<vertexType>::DeleteLine(LPDIRECT3DDEVICE9 pDevice, UINT nLine)
{
	if(m_nQuads <= 1)
	{
		////Release the index buffer
		SAFE_RELEASE(m_pVB);

		////Release the vertex buffer
		SAFE_RELEASE(m_pIndexBuffer);
		m_vertexList.RemoveAll();
		m_indexList.RemoveAll();
	}
	else
	{
		UINT minIndex = 6 * nLine;
		UINT minVertex = 4 * nLine;
		for(UINT i=0; i<6; i++)
		{
			if(i<4)
			{
				m_vertexList.Remove(minVertex);
			}
			m_indexList.Remove(minIndex);
		}

		for(UINT i=minIndex; i<m_indexList.GetSize(); i++)
		{
			m_indexList[i] -= 4;
		}
	}
	return Create(pDevice);
}
template <typename vertexType> HRESULT CGeometry_<vertexType>::Reset(LPDIRECT3DDEVICE9 pDevice)
{
		////Release the index buffer
		SAFE_RELEASE(m_pVB);

		////Release the vertex buffer
		SAFE_RELEASE(m_pIndexBuffer);

		m_vertexList.RemoveAll();
		m_indexList.RemoveAll();
	return Create(pDevice);
}
#endif