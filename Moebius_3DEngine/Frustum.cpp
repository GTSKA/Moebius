#include "StdAfx.h"
#include "Frustum.h"

CFrustum::CFrustum(void)
{
	
}

CFrustum::~CFrustum(void)
{
}

void CFrustum::Update(D3DXMATRIX *ViewProjectionInverse_Matrix)
{
	m_corners[0] =	D3DXVECTOR3(-1.0f, -1.0f, 0.0f);	// xyz
	m_corners[1] = 	D3DXVECTOR3( 1.0f, -1.0f, 0.0f);	// Xyz
	m_corners[2] =	D3DXVECTOR3(-1.0f,  1.0f, 0.0f);	// xYz
	m_corners[3] =	D3DXVECTOR3( 1.0f,  1.0f, 0.0f);	// XYz
	m_corners[4] =	D3DXVECTOR3(-1.0f, -1.0f, 1.0f);	// xyZ
	m_corners[5] =	D3DXVECTOR3( 1.0f, -1.0f, 1.0f);	// XyZ
	m_corners[6] =	D3DXVECTOR3(-1.0f,  1.0f, 1.0f);	// xYZ
	m_corners[7] =  D3DXVECTOR3( 1.0f,  1.0f, 1.0f);	// XYZ

	for (int i = 0; i < 8; i++)
    {
		D3DXVec3TransformCoord(&m_corners[i], &m_corners[i], ViewProjectionInverse_Matrix);
	}

	//Near
	D3DXPlaneFromPoints(&m_planes[0], &m_corners[0], &m_corners[1], &m_corners[2]);

    //Far
    D3DXPlaneFromPoints(&m_planes[1], &m_corners[6], &m_corners[7], &m_corners[5]);

    //Left
    D3DXPlaneFromPoints(&m_planes[2], &m_corners[2], &m_corners[6], &m_corners[4]);

    //Right
    D3DXPlaneFromPoints(&m_planes[3], &m_corners[7], &m_corners[3], &m_corners[5]);

    //Up
    D3DXPlaneFromPoints(&m_planes[4], &m_corners[2], &m_corners[3], &m_corners[6]);

    //Bottom
    D3DXPlaneFromPoints(&m_planes[5], &m_corners[1], &m_corners[0], &m_corners[4]);
}

bool CFrustum::TestPoint(D3DXVECTOR3 pt)
{
	bool res = true;
	for(UINT nPlane=0; nPlane < 6; nPlane++)
	{
		if (((m_planes[nPlane].a * pt.x) + (m_planes[nPlane].b * pt.y) + (m_planes[nPlane].c * pt.z) + (m_planes[nPlane].d)) < 0)
		{
			res = false;
			break;
		}
	}
    return res;
}

bool CFrustum::TestPoint(D3DXVECTOR3 pt, __MOEBIUS_FRUSTUM_PLANES nPlane)
{
	bool res = false;
	if (((m_planes[nPlane].a * pt.x) + (m_planes[nPlane].b * pt.y) + (m_planes[nPlane].c * pt.z) + (m_planes[nPlane].d)) >= 0)
	{
		res = true;
	}
    return res;
}


  
