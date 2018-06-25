#include "StdAfx.h"
#include "TextureManager.h"


CTextureManager::CTextureManager(void)
{
	m_texture = NULL;
	m_textureShader = NULL; 
	m_texFileName = new WCHAR[MAX_PATH];
	m_texFilePath = new WCHAR[MAX_PATH];

	m_texColor = D3DXCOLOR(1,1,1,1);
	m_bProceduCodeOK = false;
}

CTextureManager::~CTextureManager(void)
{
	
}

CTextureManager::CTextureManager(__MOEBIUS_TEXTURETYPE type, UINT width, UINT height, D3DXCOLOR color, STRSAFE_LPWSTR fileName, STRSAFE_LPCSTR proceduralCode)
{
	m_texture = NULL;
	m_textureShader = NULL; 
	m_texFileName = new WCHAR[MAX_PATH];
	m_texFilePath = new WCHAR[MAX_PATH];

	CFileManager_ file;

	LPD3DXBUFFER pCode = NULL;
	LPD3DXBUFFER pError = NULL;

	
	switch(type)
	{
	case _MOEBIUS_TEXTYPE_FILE:
		if(SUCCEEDED(file.Create(fileName)))
		{
			m_texFilePath = file.GetFullFilePath();
			m_texFileName = file.GetFileName();
		}
		m_texType = _MOEBIUS_TEXTYPE_FILE;
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
		m_texType = _MOEBIUS_TEXTYPE_RENDERTARGET;
		break;

	default:
		m_texColor = D3DXCOLOR(1,1,1,1);
		m_texType = _MOEBIUS_TEXTYPE_DEFAULT;
		break;
	}
}

UINT CTextureManager::GetTextureWidth()
{
	return m_texWidth;
}

UINT CTextureManager::GetTextureHeight()
{
	return m_texHeight;
}

HRESULT CTextureManager::CreateFromColor(LPDIRECT3DDEVICE9 pDevice, D3DXCOLOR color)
{
	LPD3DXBUFFER pCode = NULL;
	LPD3DXBUFFER pError = NULL;

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
	v.x = color.r;
	v.y = color.g;
	v.z = color.b;
	v.w = color.a;
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

	//MessageBox(NULL, L"GO!!!!!!!!!!", 0, 0);
	
	
	SAFE_RELEASE(pError);
	SAFE_RELEASE(pCode);

	SaveToFile_JPG(L"Out.jpg");

	return S_OK;
}

HRESULT CTextureManager::CreateFromFile(LPDIRECT3DDEVICE9 pDevice, STRSAFE_LPWSTR fileName)
{
	//First chek if the file exist
	CFileManager_ file;
	if(FAILED(file.Create(fileName)))
	{
		MessageBox(NULL, L"The file does not exist", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}
	
	//Create atexture
	if(FAILED(D3DXCreateTextureFromFile(pDevice, fileName, &m_texture)))
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

	pSurface.Width = m_texWidth;
	pSurface.Height = m_texHeight;

	return S_OK;
}


HRESULT CTextureManager::CreateFromProcedualFunction(LPDIRECT3DDEVICE9 pDevice)
{
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

		//SaveToFile_JPG(L"OutPut.jpg");
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CTextureManager::FillTexture()
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


HRESULT CTextureManager::SaveToFile_JPG(STRSAFE_LPWSTR fileName)
{
	if(FAILED(D3DXSaveTextureToFile(fileName, D3DXIFF_JPG, m_texture, NULL)))
	{
		MessageBox(NULL, L"D3DXSaveTextureToFile", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CTextureManager::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	//Reload the texture...
	switch(m_texType)
	{
	case _MOEBIUS_TEXTYPE_FILE:
		return CreateFromFile(pDevice, m_texFilePath);

	case _MOEBIUS_TEXTYPE_COLOR:
		return CreateFromColor(pDevice, m_texColor);
		break;

	case _MOEBIUS_TEXTYPE_PROCEDUAL:
		
		return CreateFromProcedualFunction(pDevice);
		break;

	case _MOEBIUS_TEXTYPE_RENDERTARGET:
		break;

	default:
		return CreateFromColor(pDevice, m_texColor);
	}

	return E_FAIL;
}

HRESULT CTextureManager::OnLostDevice()
{
	SAFE_RELEASE(m_textureShader);
	SAFE_RELEASE(m_texture);
	return S_OK;
}
void CTextureManager::OnDisposeDevice()
{
	
	SAFE_DELETE(m_texFilePath);
	SAFE_DELETE(m_texFileName);

	SAFE_RELEASE(m_textureShader);
	SAFE_RELEASE(m_texture);
}


