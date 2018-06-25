#pragma once

#ifndef _MOEBIUS3D_SPRITE_MANAGER_H_
#define	_MOEBIUS3D_SPRITE_MANAGER_H_

#pragma warning( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds

#include "M3D_CommondInclude.h"
#include "ResourceCtrl.h"

class CSpriteManager : public IResourceCtrl
{
public:

	LPD3DXSPRITE m_sprite;

	CSpriteManager(void);
	~CSpriteManager(void);

	HRESULT CreateSprite(LPDIRECT3DDEVICE9 pDevice);

	HRESULT OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	HRESULT	OnLostDevice();
	void OnDisposeDevice();
};

#endif
