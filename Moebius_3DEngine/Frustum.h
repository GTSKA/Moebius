#pragma once
#ifndef _MOEBIUS_FRUSTUM_H_
#define	_MOEBIUS_FRUSTUM_H_

#include <d3d9.h>
#include <d3dx9.h> 

#include <CArray_.h>

typedef enum __MOEBIUS_FRUSTUM_PLANES
{
	_NEAR_PLANE								= 0,
	_FAR_PLANE								= 1,
	_RIGHT_PLANE							= 2,
	_LEFT_PLANE								= 3,
	_UP_PLANE								= 4,
	_DOWN_PLANE								= 5,
	
}__MOEBIUS_FRUSTUM_PLANES;

class CFrustum
{
public:
	CFrustum(void);
	~CFrustum(void);

	void Update(D3DXMATRIX *ViewProjectionInverse_Matrix);

	bool TestPoint(D3DXVECTOR3 pt);
	bool TestPoint(D3DXVECTOR3 pt, __MOEBIUS_FRUSTUM_PLANES nPlane);
	//bool TestPoint(D3DXVECTOR3 pt, __MOEBIUS_FRUSTUM_PLANES nPlane);
	//bool TestPoint(CArr pt);

	/*bool TestNearPlane(D3DXVECTOR3 pt);
	bool TestFarPlane(D3DXVECTOR3 pt);
	bool TestRightPlane(D3DXVECTOR3 pt);
	bool TestLeftNearPlane(D3DXVECTOR3 pt);
	bool TestUpPlane(D3DXVECTOR3 pt);
	bool TestDownPlane(D3DXVECTOR3 pt);*/

protected:

	D3DXVECTOR3		m_corners[8];
	D3DXPLANE		m_planes[8];
	
};

#endif
