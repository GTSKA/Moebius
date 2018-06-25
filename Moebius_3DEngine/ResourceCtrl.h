#pragma once
#ifndef _MOEBIUS_RESOURCE_CONTROL_H_
#define	_MOEBIUS_RESOURCE_CONTROL_H_

#include <d3d9.h>
#include <d3dx9.h> 

class IResourceCtrl
{
public:
	virtual HRESULT OnResetDevice(LPDIRECT3DDEVICE9 pDevice) = 0;
	virtual HRESULT OnLostDevice() = 0;
	virtual void OnDisposeDevice() = 0;
};

class CResourceCtrl
{
public:
	CResourceCtrl(void);
	~CResourceCtrl(void);
};

#endif
