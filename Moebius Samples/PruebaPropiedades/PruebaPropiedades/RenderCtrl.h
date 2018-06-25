#pragma once
#ifndef _ENGINE
#define	_ENGINE

#include "Moebius3D.h"

#include "CameraBase.h"

class CRenderCtrl : public CMoebius3D
{
public:
	CRenderCtrl(void);
	~CRenderCtrl(void);
	
	virtual void Render();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT LostDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual void DisposeDeviceObjects();

	FLOAT xMouse;
	FLOAT yMouse;

	CCameraBase_ camera;


private:

	FLOAT angle;
	D3DXMATRIX mScale;
};


#endif