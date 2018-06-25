#include "StdAfx.h"
#include "TextureList_.h"

CTextureList_::CTextureList_(void)
{

}

CTextureList_::~CTextureList_(void)
{

}

void CTextureList_::AddTexture(__MOEBIUS_TEXTURETYPE type, UINT width, UINT height, D3DXCOLOR color, STRSAFE_LPWSTR fileName, STRSAFE_LPCSTR procedualCode, bool bGenerateMinMaps)
{
	m_TextureList.Add();
	m_TextureList[m_TextureList.GetSize()-1].Define(type, width, height, color, fileName, procedualCode, bGenerateMinMaps);
}

void CTextureList_::AddTexture(STRSAFE_LPWSTR fileName, bool bGenerateMinMaps)
{
	m_TextureList.Add();

	WCHAR addedPath[255] = {0};
	StringCchPrintf(addedPath, 255 * sizeof(WCHAR), L"%s\\%s", m_defaultPath, fileName);
	m_TextureList[m_TextureList.GetSize()-1].Define(_MOEBIUS_TEXTYPE_FILE, 
		0, 0, 
		D3DXCOLOR(1,1,1,1), 
		addedPath, 
		NULL, bGenerateMinMaps);
}

void CTextureList_::AddTexture(D3DXCOLOR color)
{
	m_TextureList.Add();
	m_TextureList[m_TextureList.GetSize()-1].Define(_MOEBIUS_TEXTYPE_COLOR, 
		0, 0, 
		color, NULL, NULL, true);
}

void CTextureList_::AddTexture(STRSAFE_LPCSTR procedualCode, UINT width, UINT height, bool bGenerateMinMaps)
{
	m_TextureList.Add();
	m_TextureList[m_TextureList.GetSize()-1].Define(_MOEBIUS_TEXTYPE_PROCEDUAL, 
		width, height, 
		D3DXCOLOR(1,1,1,1), NULL, procedualCode, bGenerateMinMaps);
}


UINT CTextureList_::GetNumTextures()
{
	return m_TextureList.GetSize();
}

HRESULT CTextureList_::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	for(UINT i=0; i<m_TextureList.GetSize(); i++)
	{
		m_TextureList[i].OnResetDevice(pDevice);
	}
	return S_OK;
}

HRESULT CTextureList_::OnLostDevice()
{
	for(UINT i=0; i<m_TextureList.GetSize(); i++)
	{
		m_TextureList[i].OnLostDevice();
	}
	return S_OK;
}

void CTextureList_::OnDisposeDevice()
{
	for(UINT i=0; i<m_TextureList.GetSize(); i++)
	{
		m_TextureList[i].OnDisposeDevice();
	}
}

void CTextureList_::SetDefaultPath(STRSAFE_LPWSTR path)
{
	StringCchCopy(m_defaultPath, 255 * sizeof(WCHAR) ,path);
}

STRSAFE_LPWSTR CTextureList_::GetDefaultPath()
{
	return m_defaultPath;
}


