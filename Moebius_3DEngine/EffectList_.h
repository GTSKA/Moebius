#pragma once

#ifndef _MOEBIUS3D_EFFECT_LIST_H_
#define	_MOEBIUS3D_EFFECT_LIST_H_

#include "M3D_CommondInclude.h"
#include "Effect_.h"

#include "FractalShaders_.h"

typedef enum __MOEBIUS_EFFECT_PRESET
{
	_MOEBIUS_EFFECT_BASIC		= 0,
	_MOEBIUS_EFFECT_FRACTAL		= 1,
	_MOEBIUS_EFFECT_NODES		= 2,
}__MOEBIUS_EFFECT_PRESET;


class CEffectList_ : public IResourceCtrl
{
public:
	CEffectList_(void);
	~CEffectList_(void);

	HRESULT AddEffect();
	HRESULT AddEffect(LPDIRECT3DDEVICE9 pDevice, STRSAFE_LPCSTR effectCode);
	HRESULT AddEffect(LPDIRECT3DDEVICE9 pDevice, CEffectBase_ &effect);
	HRESULT AddEffect(LPDIRECT3DDEVICE9 pDevice, __MOEBIUS_EFFECT_PRESET preset);

	UINT GetNumEffects();

	HRESULT	OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	HRESULT	OnLostDevice();
	void OnDisposeDevice();

	CEffectBase_& operator [] (const UINT index)
	{
		return m_effectList[index];		
	}

protected:

	
	CArray_<CEffectBase_> m_effectList;
};

#endif
