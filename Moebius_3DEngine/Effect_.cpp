#include "StdAfx.h"
#include "Effect_.h"

CEffectCtrl_::CEffectCtrl_()
{
	m_Effect = NULL;
}

CEffectCtrl_::~CEffectCtrl_(void)
{
	SAFE_RELEASE(m_Effect);
}

HRESULT CEffectCtrl_::Create(LPDIRECT3DDEVICE9 pDevice, LPCSTR effectCode)
{
	if(effectCode != NULL)
	{
		LPD3DXBUFFER pError = NULL;

		UINT codeSize = strlen(effectCode);
		HRESULT hr = D3DXCreateEffect(pDevice, (LPCVOID)effectCode, codeSize, NULL, NULL, D3DXFX_NOT_CLONEABLE, NULL, &m_Effect, &pError);

		if(hr == D3DERR_INVALIDCALL)
		{
			MessageBox(NULL, L"Error in D3DXCreateEffect with D3DERR_INVALIDCALL", L"ERROR", MB_OK|MB_ICONERROR);
			return E_FAIL;
		}
		else if( hr == E_OUTOFMEMORY)
		{
			MessageBox(NULL, L"Error in D3DXCreateEffect with E_OUTOFMEMORY", L"ERROR", MB_OK|MB_ICONERROR);
			return E_FAIL;
		}
		else if(hr == D3DXERR_INVALIDDATA)
		{
			CHAR *errorMsg = ((CHAR*)pError->GetBufferPointer());
			MessageBoxA(NULL, errorMsg, "ERROR: D3DXERR_INVALIDDATA", MB_OK|MB_ICONERROR);
			delete errorMsg;
			return E_FAIL;
		}
		if(FAILED(hr))
		{
			CHAR *errorMsg = ((CHAR*)pError->GetBufferPointer());
			if(errorMsg != NULL)
			{
				MessageBoxA(NULL, errorMsg, "Compiler Error", MB_OK|MB_ICONERROR);
			}
			else
			{
				MessageBox(NULL, L"Unknown error in the effec", L"ERROR", MB_OK|MB_ICONERROR);
			}
			delete errorMsg;
			
			return E_FAIL;
		}

		return S_OK;
	}
	else
	{
		MessageBox(NULL, L"Error in buffer code", L"ERROR", MB_OK|MB_ICONERROR);
		return E_FAIL;
	}
	
}

HRESULT CEffectCtrl_::ValidateTechnique(D3DXHANDLE handleTechnique)
{
	m_Effect->ValidateTechnique(handleTechnique);
	return S_OK;
}


HRESULT CEffectCtrl_::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	m_Effect->OnResetDevice();
	return S_OK;
}

HRESULT CEffectCtrl_::OnLostDevice()
{
	m_Effect->OnLostDevice();
	return S_OK;
}
void CEffectCtrl_::OnDisposeDevice()
{
	SAFE_RELEASE(m_Effect);
}


HRESULT CEffectBase_::Init(LPDIRECT3DDEVICE9 pDevice)
{
	return Create(pDevice, m_effectCode);
}

STRSAFE_LPSTR CEffectBase_::GetEffectCode()
{
	return m_effectCode;
}

HRESULT CEffectBase_::SetEffectCode(LPDIRECT3DDEVICE9 pDevice, STRSAFE_LPCSTR effectCode)
{
	int l = strlen(effectCode) + 1;
	m_effectCode = (char*)malloc(l*sizeof(char));

	CopyMemory((LPVOID)m_effectCode, (LPVOID)effectCode, strlen(effectCode) * sizeof(CHAR));

	for(int  i=0; i<l-1; i++)
	{
		m_effectCode[i] = effectCode[i];
	}
	m_effectCode[l-1] = '\0';

	SAFE_RELEASE(m_Effect);
	return Create(pDevice, m_effectCode);
}

CBasicShaders_::CBasicShaders_() 
{
	m_effectCode =
	//--------------------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------------------

	//Matrix 
	//---------------------------------------------------------------------------------------
	"float4x4 WorldViewProjection_Matrix;\n"

	//Parameters
	//---------------------------------------------------------------------------------------
	"float4 diffuseColor = {1.0f, 1.0f, 1.0f, 1.0f};\n"

	//Textures
	//---------------------------------------------------------------------------------------
	"texture diffuseTexture_01;\n"
	"sampler2D diffuseTexture_01Sampler = sampler_state\n"
	"{\n"
	"Texture = <diffuseTexture_01>;\n"
	"MinFilter = Linear;\n"
	"MagFilter = Linear;\n"
	"MipFilter = Linear;\n"
	"AddressU = Clamp;\n"
	"AddressV = Clamp;\n"
	"};\n"

	//Vertex Structs
	//---------------------------------------------------------------------------------------
	"struct VertexInput\n"
	"{\n"
	"float4 position : POSITION;\n"
	"float2 texCoord0: TEXCOORD0;\n"
	"};\n"

	//Pixel Structs
	//---------------------------------------------------------------------------------------
	"struct PixelInput\n"
	"{\n"
	"float4 hpos      : POSITION;\n"
	"float2 texCoord0 : TEXCOORD0;\n"
	"};\n"



	//Vertex Shaders
	//---------------------------------------------------------------------------------------
	"PixelInput VertexShader_Main(VertexInput vs_in)\n"
	"{\n"
	"    PixelInput vs_out;\n"
	"    vs_out.hpos = mul(vs_in.position, WorldViewProjection_Matrix);\n"
	"    vs_out.texCoord0 = vs_in.texCoord0;\n"
	"    return vs_out;\n"
	"}\n"

	"PixelInput VS_SCREEN(float4 Pos: POSITION)\n"
	"{\n"
	"    PixelInput vs_out;\n"
	"    Pos.xy = sign(Pos.xy);\n"
	"    vs_out.hpos = float4(Pos.xy, 0 ,1);\n"
	"    vs_out.texCoord0.x = 0.5 * (1 + Pos.x);\n"
	"    vs_out.texCoord0.y = 0.5 * (1 - Pos.y);\n"
	"    return vs_out;\n"
	"}\n"

	//Pixel Shaders
	//---------------------------------------------------------------------------------------
	"float4 ColorOnly_PS(PixelInput ps_in) : COLOR\n"
	"{\n"
	"    return diffuseColor;\n"
	"}\n"

	"float4 TextureOnly_PS(PixelInput ps_in) : COLOR\n"
	"{\n"
	"	return tex2D( diffuseTexture_01Sampler, ps_in.texCoord0 );\n"
	"}\n"
	"technique Color \n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"			CullMode = NONE;\n"
	"		    VertexShader = compile vs_2_0 VertexShader_Main();\n"
	"		    PixelShader = compile ps_2_0 ColorOnly_PS();\n"
	"    }\n"
	"}\n"

	"technique ColorWireframe\n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"		CullMode = NONE;\n"
	"	    VertexShader = compile vs_2_0 VertexShader_Main();\n"
	"		PixelShader = compile ps_2_0 ColorOnly_PS();\n"
	"       FillMode = WIREFRAME;\n"
	"	}\n"
	"}\n"

	"technique Textured\n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"		CullMode = NONE;\n"
	"		    VertexShader = compile vs_2_0 VertexShader_Main();\n"
	//"		    VertexShader = compile vs_2_0 VS_SCREEN();\n"
	"		    PixelShader = compile ps_2_0 TextureOnly_PS();\n"
	"    }\n"
	"}\n"

	"technique TexturedWireframe\n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"		CullMode = NONE;\n"
	"	    VertexShader = compile vs_2_0 VertexShader_Main();\n"
	"	    PixelShader = compile ps_2_0 TextureOnly_PS();\n"
	"       FillMode = WIREFRAME;\n"
	"	}\n"
	"}\n";

	//--------------------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------------------
}

CBasicShaders_::~CBasicShaders_()
{
	
}