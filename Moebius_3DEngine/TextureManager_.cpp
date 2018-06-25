#include "StdAfx.h"
#include "TextureManager_.h"


CTextureManager_::CTextureManager_(void)
{
	m_texture = NULL;
	m_textureSurface = NULL;
	m_textureShader = NULL; 
//	m_texFileName = new WCHAR[MAX_PATH];
	m_texFilePath = new WCHAR[MAX_PATH];

	m_texColor = D3DXCOLOR(1,1,1,1);
	m_bProceduCodeOK = false;
}

CTextureManager_::~CTextureManager_(void)
{
	//SAFE_DELETE(m_texFilePath);
}

void CTextureManager_::Define(__MOEBIUS_TEXTURETYPE type, UINT width, UINT height, D3DXCOLOR color, STRSAFE_LPWSTR fileName, STRSAFE_LPCSTR proceduralCode, bool bGenerateMinMaps)
{
	SAFE_RELEASE(m_texture);
	SAFE_RELEASE(m_textureSurface);
	SAFE_RELEASE(m_textureShader);
	m_texFilePath = new WCHAR[MAX_PATH];
	m_makeMinMaps = bGenerateMinMaps;

	LPD3DXBUFFER pCode = NULL;
	LPD3DXBUFFER pError = NULL;
	
	switch(type)
	{
	case _MOEBIUS_TEXTYPE_FILE:

		if(SUCCEEDED(FindFile(m_texFilePath, MAX_PATH, fileName)))
		{
			m_texType = _MOEBIUS_TEXTYPE_FILE;
		}
		break;

	case _MOEBIUS_TEXTYPE_COLOR:
		m_texColor = color;
		m_texType = _MOEBIUS_TEXTYPE_COLOR;
		break;

	case _MOEBIUS_TEXTYPE_PROCEDUAL:

		m_bProceduCodeOK = true;

		//We chek if the code is ok...
		m_procedualCode = new CHAR[strlen(proceduralCode)];
		StringCchCopyA(m_procedualCode, strlen(proceduralCode), proceduralCode);

		if(FAILED(D3DXCompileShader(m_procedualCode, strlen(m_procedualCode), NULL, NULL, 
			"Main", "tx_1_0", 0, &pCode, &pError, NULL)))
		{
			CHAR *errorMsg = ((CHAR*)pError->GetBufferPointer());
			MessageBoxA(NULL, errorMsg, "Error in shader compilation", MB_OK|MB_ICONERROR);
			delete errorMsg;
			m_bProceduCodeOK = false;
		}
		if(FAILED(D3DXCreateTextureShader((DWORD*)pCode->GetBufferPointer(), &m_textureShader)))
		{
			MessageBox(NULL, L"Error in the shader texture", L"ERROR", MB_OK|MB_ICONERROR);
			m_bProceduCodeOK = false;
		}

		m_texWidth	= width;
		m_texHeight = height;
		m_texType = _MOEBIUS_TEXTYPE_PROCEDUAL;

		SAFE_RELEASE(pError);
		SAFE_RELEASE(pCode);

		break;

	case _MOEBIUS_TEXTYPE_RENDERTARGET:

		m_texWidth	= width;
		m_texHeight = height;
		m_texType = _MOEBIUS_TEXTYPE_RENDERTARGET;

		break;

	default:
		m_texColor = D3DXCOLOR(1,1,1,1);
		m_texType = _MOEBIUS_TEXTYPE_DEFAULT;
		break;
	}
}

UINT CTextureManager_::GetTextureWidth()
{
	return m_texWidth;
}

UINT CTextureManager_::GetTextureHeight()
{
	return m_texHeight;
}

HRESULT CTextureManager_::CreateFromColor(LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR color)
{
	LPD3DXBUFFER pCode = NULL;
	LPD3DXBUFFER pError = NULL;

	m_texColor = color;

	if(FAILED(D3DXCompileShader(__TEXTURESHADER_SINGLECOLOR, strlen(__TEXTURESHADER_SINGLECOLOR), NULL, NULL, 
		"Main", "tx_1_0", 0, &pCode, &pError, NULL)))
	{
		CHAR *errorMsg = ((CHAR*)pError->GetBufferPointer());

	
		MessageBoxA(NULL, errorMsg, "Error in shader compilation", MB_OK|MB_ICONERROR);
		SAFE_DELETE(errorMsg);
		return E_FAIL;
	}
	if(FAILED(D3DXCreateTextureShader((DWORD*)pCode->GetBufferPointer(), &m_textureShader)))
	{
		MessageBox(NULL, L"Error in the shader texture", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	D3DXVECTOR4 v;
	v.x = m_texColor .r;
	v.y = m_texColor .g;
	v.z = m_texColor .b;
	v.w = m_texColor .a;
	m_textureShader->SetVector("color", &v);
	
	if(FAILED(D3DXCreateTexture(pDevice, 1, 1, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &m_texture)))
	{
		MessageBox(NULL, L"Error in D3DXCreateTexture", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	if(FAILED(D3DXFillTextureTX(m_texture, m_textureShader)))
	{
		MessageBox(NULL, L"Error during D3DXFillTextureTX", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	SAFE_RELEASE(pError);
	SAFE_RELEASE(pCode);
	
	return S_OK;
}

HRESULT CTextureManager_::CreateFromFile(LPDIRECT3DDEVICE9 pDevice, STRSAFE_LPWSTR fileName, BOOL bGenerateMinMaps)
{
	//First chek if the file exist
	//if(FAILED(m_file.Create(fileName)))

	if(FAILED(FindFile(m_texFilePath, MAX_PATH, fileName)))
	{
		MessageBox(NULL, L"The file does not exist", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}
	
	//Create atexture
	if(FAILED(D3DXCreateTextureFromFile(pDevice, m_texFilePath, &m_texture)))
	{
		MessageBox(NULL, L"Error calling: D3DXCreateTextureFromFile", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	//Now get the size of the texture
	D3DSURFACE_DESC pSurface;
	ZeroMemory(&pSurface, sizeof(D3DSURFACE_DESC));
	if(FAILED(m_texture->GetLevelDesc(0, &pSurface)))
	{
		MessageBox(NULL, L"GetSurfaceLevel", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	m_texWidth = pSurface.Width;
	m_texHeight = pSurface.Height;

	if(bGenerateMinMaps)
	{
		m_texture->GenerateMipSubLevels();
	}

	return S_OK;
}


HRESULT CTextureManager_::CreateFromProcedualFunction(LPDIRECT3DDEVICE9 pDevice, BOOL bGenerateMinMaps)
{
	LPD3DXBUFFER pCode = NULL;
	LPD3DXBUFFER pError = NULL;
	if(FAILED(D3DXCompileShader(m_procedualCode, strlen(m_procedualCode), NULL, NULL, 
			"Main", "tx_1_0", 0, &pCode, &pError, NULL)))
	{
		CHAR *errorMsg = ((CHAR*)pError->GetBufferPointer());
		MessageBoxA(NULL, errorMsg, "Error in shader compilation", MB_OK|MB_ICONERROR);
		m_bProceduCodeOK = false;
	}
	if(FAILED(D3DXCreateTextureShader((DWORD*)pCode->GetBufferPointer(), &m_textureShader)))
	{
		MessageBox(NULL, L"Error in the shader texture", L"ERROR", MB_OK|MB_ICONERROR);
		m_bProceduCodeOK = false;
	}
	SAFE_RELEASE(pError);
	SAFE_RELEASE(pCode);

	if(m_textureShader != NULL)
	{
		if(FAILED(D3DXCreateTexture(pDevice, m_texWidth, m_texHeight, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &m_texture)))
		{
			MessageBox(NULL, L"Error in D3DXCreateTexture", L"ERROR", MB_OK|MB_ICONERROR);
			return E_FAIL;
		}
		if(FAILED(D3DXFillTextureTX(m_texture, m_textureShader)))
		{
			MessageBox(NULL, L"Error during D3DXFillTextureTX", L"ERROR", MB_OK|MB_ICONERROR);
			return E_FAIL;
		}
		return S_OK;
	}

	if(bGenerateMinMaps)
	{
		m_texture->GenerateMipSubLevels();
	}


	return E_FAIL;
}

HRESULT CTextureManager_::CreateForRenderTarget(LPDIRECT3DDEVICE9 pDevice, BOOL bGenerateMinMaps)
{
	if(FAILED(D3DXCreateTexture(pDevice, m_texWidth, m_texHeight,
		bGenerateMinMaps ? 0 : 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_texture)))
	{
		return E_FAIL;
	}

	if(FAILED(m_texture->GetSurfaceLevel(0, &m_textureSurface)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTextureManager_::FillTexture()
{
	if(m_texType == _MOEBIUS_TEXTYPE_PROCEDUAL && m_texture != NULL && m_textureShader != NULL)
	{
		if(FAILED(D3DXFillTextureTX(m_texture, m_textureShader)))
		{
			MessageBox(NULL, L"Error during D3DXFillTextureTX", L"ERROR", MB_OK|MB_ICONERROR);
			return E_FAIL;
		}
		return S_OK;
	}
	return E_FAIL;
}


HRESULT CTextureManager_::SaveToFile_JPG(STRSAFE_LPWSTR fileName)
{
	if(FAILED(D3DXSaveTextureToFile(fileName, D3DXIFF_JPG, m_texture, NULL)))
	{
		MessageBox(NULL, L"D3DXSaveTextureToFile", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CTextureManager_::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	//MessageBox(NULL, m_texFilePath , 0,0);

	//Reload the texture...
	switch(m_texType)
	{
	case _MOEBIUS_TEXTYPE_FILE:
		return CreateFromFile(pDevice, m_texFilePath, m_makeMinMaps);

	case _MOEBIUS_TEXTYPE_COLOR:
		return CreateFromColor(pDevice, m_texColor);

	case _MOEBIUS_TEXTYPE_PROCEDUAL:
		return CreateFromProcedualFunction(pDevice, m_makeMinMaps);

	case _MOEBIUS_TEXTYPE_RENDERTARGET:
		return CreateForRenderTarget(pDevice, m_makeMinMaps);

	default:
		return CreateFromColor(pDevice, m_texColor);
	}

	return E_FAIL;
}

HRESULT CTextureManager_::OnLostDevice()
{
	SAFE_RELEASE(m_textureShader);
	SAFE_RELEASE(m_textureSurface);
	SAFE_RELEASE(m_texture);
	return S_OK;
}
void CTextureManager_::OnDisposeDevice()
{
	SAFE_DELETE(m_texFilePath);

	SAFE_RELEASE(m_textureShader);
	SAFE_RELEASE(m_textureSurface);
	SAFE_RELEASE(m_texture);
}

UINT CTextureManager_::GetMaxMinMapLevelCount()
{
	if(m_texture != NULL)
	{
		return m_texture->GetLevelCount();
	}
}

UINT CTextureManager_::GetMinMapLevel()
{
	if(m_texture != NULL)
	{
		return m_texture->GetLOD();
	}
}

void CTextureManager_::SetMinMapLevel(UINT nLevel)
{
	if(m_texture != NULL)
	{
		if(nLevel < m_texture->GetLevelCount())
		{
			m_texture->SetLOD(nLevel);
		}
	}
}

void CTextureManager_::SetPercentLevel(FLOAT percent)
{
	if(m_texture != NULL)
	{
		UINT nLevel = (UINT)((100 - percent) * (m_texture->GetLevelCount() - 1) / 100.0f);
		if(nLevel < m_texture->GetLevelCount())
		{
			m_texture->SetLOD(nLevel);
		}
	}
}


