#pragma once
#ifndef _MOEBIUS_CAMERA_BASE_H_
#define	_MOEBIUS_CAMERA_BASE_H_

#include <d3d9.h>
#include <d3dx9.h> 
#include "FrameMoveCtrl.h"

typedef enum __MOEBIUS_CAMERA_MATRIX
{
	_VIEW									= 0,
	_PROJECTION								= 1,
	_VIEW_PROJECTION						= 2,
	_VIEW_INVERSE							= 3,
	_VIEW_INVERSE_TRANSPOSE					= 4,
	_VIEW_PROJECTION_INVERSE				= 5,
	_VIEW_PROJECTION_INVERSE_TRANSPOSE		= 6,
	_PROJECTION_INVERSE						= 7,
	_PROJECTION_INVERSE_TRANSPOSE			= 8,
   
}__MOEBIUS_CAMERA_MATRIX;

class CCameraBase_ : public IFrameMoveCtrl
{
public:

	D3DXVECTOR3		m_cameraPosition;
	D3DXVECTOR3		m_cameraTarget;
	D3DXVECTOR3		m_ViewVector;
	D3DXVECTOR3		m_cameraUp;

	FLOAT			m_fieldOfView;
	FLOAT			m_fAspectRadio;
	FLOAT			m_zNear;
	FLOAT			m_zFar;

	D3DXMATRIX		m_View; 
	D3DXMATRIX		m_ViewProjection;
	D3DXMATRIX		m_ViewInverse;
	D3DXMATRIX		m_ViewInverseTranspose;
	D3DXMATRIX		m_ViewProjectionInverse;
	D3DXMATRIX		m_ViewProjectionInverseTranspose;
	D3DXMATRIX		m_Projection; 
	D3DXMATRIX		m_ProjectionInverse; 
	D3DXMATRIX		m_ProjectionInverseTranspose; 

	CCameraBase_(void);
	CCameraBase_(bool bUpdateViewMatrix, bool bUpdateProjectionMatrix, D3DXVECTOR3 camPosition, D3DXVECTOR3 camTarget, FLOAT AspectRadio, FLOAT zNear, FLOAT zFar, FLOAT fielOfView = D3DXToRadian(45.0f), D3DXVECTOR3 vUp = D3DXVECTOR3(0,1,0));

	~CCameraBase_(void);


	void Create(bool bUpdateViewMatrix, bool bUpdateProjectionMatrix, D3DXVECTOR3 camPosition, D3DXVECTOR3 camTarget, FLOAT AspectRadio, FLOAT zNear, FLOAT zFar, FLOAT fielOfView  = D3DXToRadian(45.0f), D3DXVECTOR3 vUp = D3DXVECTOR3(0,1,0));
	void UpdateMatrix(bool bUpdateViewMatrix = true, bool bUpdateProjectionMatrix = true);
	void SetViewMatrixParameters(D3DXVECTOR3 camPosition, D3DXVECTOR3 camTarget, D3DXVECTOR3 vUp = D3DXVECTOR3(0,1,0)); 
	void SetProjectionParamters(FLOAT AspectRadio, FLOAT zNear, FLOAT zFar, FLOAT fielOfView  = D3DXToRadian(45.0f));
	void TransformCamera(D3DXMATRIX matrixTranform);

	void WorldToScreenSpace(D3DXVECTOR3 *vPoint, UINT screenWidth, UINT screenHeight);
	void WorldToScreenSpace(D3DXVECTOR2 *v2DPoint, D3DXVECTOR3 *vPoint, UINT screenWidth, UINT screenHeight);

	void MousetoViewVector(int xMousePos, int yMousePos, UINT screenWidht, UINT screenHeight, D3DXVECTOR3 *vOrig, D3DXVECTOR3* vDir);
	void MousetoViewVector(int xMousePos, int yMousePos, UINT screenWidht, UINT screenHeight, D3DXVECTOR3 *v1, D3DXVECTOR3* v2, FLOAT hDist);

	virtual void OnFrameMove(FLOAT elpsedTime, FLOAT absTime);
};

class CModelViewCamera_ : public CCameraBase_
{
	public:

	CModelViewCamera_(void);
	~CModelViewCamera_(void);

	void SetCameraValues(float alfa, float beta, float radio, float radiomax, float radiomin);
	void OnMouseMove(int x, int y, bool bLeftMouse, bool bRightMouse);
	void OnMouseWheel(int delta);
	void Refresh();

	float m_radio;
	float m_radioMin;
	float m_radioMax;

	float m_angleDelta;
	float m_alfa;
	float m_beta;

	float m_alfaMax;
	float m_alfaMin;
	float m_betaMax;
	float m_betaMin;
		
	private:

	

		int m_mouse_x;
		int m_mouse_y;

		bool m_bUseLeftButton;
};

#endif
