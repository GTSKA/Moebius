#pragma once
#ifndef _MOEBIUS3D_OFFSCREEN_RENDER_MANAGER_H_
#define	_MOEBIUS3D_OFFSCREEN_RENDER_MANAGER_H_

#include "M3D_CommondInclude.h"
#include "ResourceCtrl.h"
#include "CameraBase.h"

class COffScreenRenderManager_ : public IResourceCtrl
{
public:

	CCameraBase_ m_camera;

	COffScreenRenderManager_(void);
	~COffScreenRenderManager_(void);

	HRESULT Begin(PDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 renderSurface);
	HRESULT End(LPDIRECT3DDEVICE9 pDevice);

	HRESULT	OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	HRESULT	OnLostDevice();
	void OnDisposeDevice();

protected:

	LPDIRECT3DSURFACE9 m_defaultSurface;
};

#endif
