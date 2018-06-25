#pragma once

#ifndef _MOEBIUS3D_TEXTURE_MANAGER_H_
#define	_MOEBIUS3D_TEXTURE_MANAGER_H_

#pragma warning( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds

#include "M3D_CommondInclude.h"

#include "FileManager_.h"

#include <d3d9.h>
#include <d3dx9.h>

#include "ShaderTextureCodes.h"


typedef enum __MOEBIUS_TEXTURETYPE
{
	_MOEBIUS_TEXTYPE_DEFAULT		= 0,
    _MOEBIUS_TEXTYPE_COLOR			= 1,
	_MOEBIUS_TEXTYPE_FILE			= 2,
	_MOEBIUS_TEXTYPE_PROCEDUAL		= 3,
	_MOEBIUS_TEXTYPE_RENDERTARGET	= 4,
}__MOEBIUS_TEXTURETYPE;

class CTextureManager
{
public:

	LPDIRECT3DTEXTURE9		m_texture;
	LPD3DXTEXTURESHADER		m_textureShader;

	CTextureManager(void);
	~CTextureManager(void);

	CTextureManager(__MOEBIUS_TEXTURETYPE type, UINT width, UINT height, D3DXCOLOR color, STRSAFE_LPWSTR fileName, STRSAFE_LPCSTR procedualCode);
	
	HRESULT CreateFromColor(LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR color);
	HRESULT CreateFromFile(LPDIRECT3DDEVICE9 pDevice, STRSAFE_LPWSTR fileName);
	HRESULT CreateFromProcedualFunction(LPDIRECT3DDEVICE9 pDevice);

	HRESULT FillTexture();

	HRESULT	OnResetDevice(LPDIRECT3DDEVICE9 pDevice);

	HRESULT SaveToFile_JPG(STRSAFE_LPWSTR fileName);

	HRESULT	OnLostDevice();
	void OnDisposeDevice();

	inline UINT GetTextureWidth();
	inline UINT GetTextureHeight();

private:
	
	__MOEBIUS_TEXTURETYPE	m_texType;
	D3DXCOLOR				m_texColor;
	LPWSTR					m_texFileName;
	LPWSTR					m_texFilePath;
	UINT					m_texWidth;
	UINT					m_texHeight;
	bool					m_bProceduCodeOK;
	LPSTR					m_procedualCode;
};

#endif
