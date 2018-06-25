#pragma once
#ifndef _MOEBIUS3D_GEOMETRY_N_H_
#define _MOEBIUS3D_GEOMETRY_N_H_

#include "Geometry_.h"

//Vertex strucut to hold:
//Position
//Tex Coord
//Color
//Normal
//Tangent
//Binormal (co-tangent)
struct _MoebiusVertex_puvcntb 
{
	D3DXVECTOR3 position;	//Vertex position
	FLOAT tu, tv;			//Vertex texture coordinates
	D3DXVECTOR4	color;		//Vertex color

	D3DXVECTOR3 normal;		//Vertex normal

	//D3DXVECTOR3 tangent;	//Vertex tangen
	//D3DXVECTOR3 binormal;	//Vertex binormal
};



template <typename vertexType> class CGeometry_Normal_ : public CGeometry_<vertexType> 
{
public:
	CGeometry_Normal_(void);
	~CGeometry_Normal_(void);

	virtual HRESULT OpenFromOBJ(LPDIRECT3DDEVICE9 pDevice, STRSAFE_LPWSTR fileName);
};


template <typename vertexType> CGeometry_Normal_<vertexType>::CGeometry_Normal_(void)
{
	
}

template <typename vertexType> CGeometry_Normal_<vertexType>::~CGeometry_Normal_(void)
{

}

template <typename vertexType> HRESULT CGeometry_Normal_<vertexType>::OpenFromOBJ(LPDIRECT3DDEVICE9 pDevice, STRSAFE_LPWSTR fileName)
{
	return S_OK;
}

#endif