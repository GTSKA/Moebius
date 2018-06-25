#pragma once

#ifndef _MOEBIUS3D_TEXTURE_LIST_H_
#define	_MOEBIUS3D_TEXTURE_LIST_H_

#include "M3D_CommondInclude.h"
#include "TextureManager_.h"

class CTextureList_ : public IResourceCtrl
{
public:
	CTextureList_(void);
	~CTextureList_(void);

	void AddTexture(__MOEBIUS_TEXTURETYPE type, UINT width, UINT height, D3DXCOLOR color, STRSAFE_LPWSTR fileName, STRSAFE_LPCSTR procedualCode, bool bGenerateMinMaps = true);
	void AddTexture(STRSAFE_LPWSTR fileName, bool bGenerateMinMaps = true);
	void AddTexture(D3DXCOLOR color);
	void AddTexture(STRSAFE_LPCSTR procedualCode, UINT width = 32, UINT height = 32, bool bGenerateMinMaps = true);

	UINT GetNumTextures();

	HRESULT	OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	HRESULT	OnLostDevice();
	void OnDisposeDevice();

	void SetDefaultPath(STRSAFE_LPWSTR path);
	STRSAFE_LPWSTR GetDefaultPath();
	
	CTextureManager_& operator [] (const UINT index)
	{
		return m_TextureList[index];
	}

protected:

	WCHAR m_defaultPath[255];

	CArray_<CTextureManager_> m_TextureList;
};

#endif
