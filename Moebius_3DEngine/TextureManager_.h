#pragma once

#ifndef _MOEBIUS3D_TEXTURE_MANAGER_H_
#define	_MOEBIUS3D_TEXTURE_MANAGER_H_

#pragma warning( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds

#include "M3D_CommondInclude.h"
#include "FileManager_.h"
#include "ResourceCtrl.h"
#include "ShaderTextureCodes.h"

typedef enum __MOEBIUS_TEXTURETYPE
{
	_MOEBIUS_TEXTYPE_DEFAULT		= 0,
    _MOEBIUS_TEXTYPE_COLOR			= 1,
	_MOEBIUS_TEXTYPE_FILE			= 2,
	_MOEBIUS_TEXTYPE_PROCEDUAL		= 3,
	_MOEBIUS_TEXTYPE_RENDERTARGET	= 4,
}__MOEBIUS_TEXTURETYPE;

class CTextureManager_ : public IResourceCtrl
{
public:

	LPDIRECT3DTEXTURE9		m_texture;
	LPDIRECT3DSURFACE9		m_textureSurface;
	LPD3DXTEXTURESHADER		m_textureShader;
	LPWSTR					m_texFilePath;

	CTextureManager_(void);
	~CTextureManager_(void);

	void Define(__MOEBIUS_TEXTURETYPE type, UINT width, UINT height, D3DXCOLOR color, STRSAFE_LPWSTR fileName, STRSAFE_LPCSTR procedualCode, bool bGenerateMinMaps = true);
	
	HRESULT CreateFromColor(LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR color);
	HRESULT CreateFromFile(LPDIRECT3DDEVICE9 pDevice, STRSAFE_LPWSTR fileName, BOOL bGenerateMinMaps = true);
	HRESULT CreateFromProcedualFunction(LPDIRECT3DDEVICE9 pDevice, BOOL bGenerateMinMaps = true);
	HRESULT CreateForRenderTarget(LPDIRECT3DDEVICE9 pDevice, BOOL bGenerateMinMaps = true);

	HRESULT FillTexture();

	HRESULT SaveToFile_JPG(STRSAFE_LPWSTR fileName);

	UINT GetMaxMinMapLevelCount();
	UINT GetMinMapLevel();
	void SetMinMapLevel(UINT nLevel);
	void SetPercentLevel(FLOAT percent);

	HRESULT	OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	HRESULT	OnLostDevice();
	void OnDisposeDevice();

	UINT GetTextureWidth();
	UINT GetTextureHeight();

private:
	
	__MOEBIUS_TEXTURETYPE	m_texType;
	D3DXCOLOR				m_texColor;

	bool					m_makeMinMaps;
	
	UINT					m_texWidth;
	UINT					m_texHeight;
	bool					m_bProceduCodeOK;
	LPSTR					m_procedualCode;

	CFileManager_			m_file;
};

#endif
