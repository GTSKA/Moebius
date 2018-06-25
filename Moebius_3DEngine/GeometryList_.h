#pragma once

#ifndef _MOEBIUS3D_GEOMETRY_LIST_H_
#define	_MOEBIUS3D_GEOMETRY_LIST_H_

#include "M3D_CommondInclude.h"
#include "Geometry_.h"


template <typename vertexType> class CGeometryList_ : public IResourceCtrl
{
public:
	CGeometryList_(void);
	~CGeometryList_(void);

	void AddGeometry();
	void AddGeometry(__MOEBIUS_GEOMETRY_TYPE type, DWORD FVF, UINT nVertices, D3DXVECTOR3 v0, D3DXVECTOR3 v1);
	void AddGeometry(STRSAFE_LPWSTR fileName);

	UINT GetNumGeometries();

	HRESULT	OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	HRESULT	OnLostDevice();
	void OnDisposeDevice();

	CGeometry_<vertexType>& operator [] (const UINT index)
	{
		return m_geometryList[index];
	}

protected:

	CArray_<CGeometry_<vertexType>> m_geometryList;

};

template <typename vertexType> CGeometryList_<vertexType>::CGeometryList_(void)
{

}

template <typename vertexType> CGeometryList_<vertexType>::~CGeometryList_(void)
{

}

template <typename vertexType> void CGeometryList_<vertexType>::AddGeometry()
{
	m_geometryList.Add();
}

template <typename vertexType> void CGeometryList_<vertexType>::AddGeometry(STRSAFE_LPWSTR fileName)
{
	m_geometryList.Add();
	//m_geometryList[m_geometryList.GetSize()-1].Define

}

template <typename vertexType> void CGeometryList_<vertexType>::AddGeometry(__MOEBIUS_GEOMETRY_TYPE type, DWORD FVF, UINT nVertices, D3DXVECTOR3 v0, D3DXVECTOR3 v1)
{
	m_geometryList[m_geometryList.GetSize()-1].Define(__MOEBIUS_GEOMETRY_TYPE type, DWORD FVF, UINT nVertices, D3DXVECTOR3 v0, D3DXVECTOR3 v1);
}

template <typename vertexType> HRESULT CGeometryList_<vertexType>::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	for(UINT i=0; i<m_geometryList.GetSize(); i++)
	{
		if(FAILED(m_geometryList[i].OnResetDevice(pDevice)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

template <typename vertexType> HRESULT CGeometryList_<vertexType>::OnLostDevice()
{
	for(UINT i=0; i<m_geometryList.GetSize(); i++)
	{
		if(FAILED(m_geometryList[i].OnLostDevice()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

template <typename vertexType> void CGeometryList_<vertexType>::OnDisposeDevice()
{
	for(UINT i=0; i<m_geometryList.GetSize(); i++)
	{
		m_geometryList[i].OnDisposeDevice();
	}
}

template <typename vertexType> UINT CGeometryList_<vertexType>::GetNumGeometries()
{
	return m_geometryList.GetSize();
}

#endif
