#include "StdAfx.h"
#include "PreprocessRender_.h"

CPreprocessRender_::CPreprocessRender_(void)
{
	
}

CPreprocessRender_::~CPreprocessRender_(void)
{

}

void CPreprocessRender_::Render(LPDIRECT3DDEVICE9 pDevice)
{
	UINT nPass;
	m_effect.m_Effect->Begin(&nPass, 0);
	for(UINT i=0; i<nPass; i++)
	{
		m_effect.m_Effect->BeginPass(i);
		m_screenPlane.Render(pDevice);
		m_effect.m_Effect->EndPass();
	}
	m_effect.m_Effect->End();
	pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(0.0,0.0,0.0,1), 1.0f, 0 );
}

void CPreprocessRender_::SetBkColor01(D3DXVECTOR4 color)
{
	bkColors[0] = color;
	m_effect.m_Effect->SetVectorArray("_colors", bkColors, 4); 
}

void CPreprocessRender_::SetBkColor02(D3DXVECTOR4 color)
{
	bkColors[1] = color;
	m_effect.m_Effect->SetVectorArray("_colors", bkColors, 4); 
}

void CPreprocessRender_::SetGradientColors(D3DXVECTOR4 color1, D3DXVECTOR4 color2, D3DXVECTOR4 color3, D3DXVECTOR4 color4)
{
	bkColors[0] = color1;
	bkColors[1] = color2;
	bkColors[2] = color3;
	bkColors[3] = color4;
	m_effect.m_Effect->SetVectorArray("_colors", bkColors, 4); 

}

void CPreprocessRender_::SetBkTexture01(LPDIRECT3DTEXTURE9 bkTexture)
{
	m_effect.m_Effect->SetTexture("DiffuseTexture_01", bkTexture); 
}
	
void CPreprocessRender_::SetBkTexture02(LPDIRECT3DTEXTURE9 bkTexture)
{

}

void CPreprocessRender_::SetTextureTilling(float tX, float tY)
{
	fParameters[0] = tX;
	fParameters[1] = tY;
	m_effect.m_Effect->SetFloatArray("_parameters", fParameters, 4); 
}

void CPreprocessRender_::SetTextureBalance(float balance)
{

}

HRESULT CPreprocessRender_::OnInitDeviceObjects(LPDIRECT3DDEVICE9 pDevice)
{
	m_effect.SetEffectCode(pDevice, __PREPROCESS_SHADER_);
	return m_screenPlane.AddQuad(pDevice, D3DXVECTOR3(-1,-1,0), D3DXVECTOR3(1,1,0));
}

HRESULT CPreprocessRender_::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	m_effect.OnResetDevice(pDevice);
	return m_screenPlane.OnResetDevice(pDevice);
}

HRESULT CPreprocessRender_::OnLostDevice()
{
	m_effect.OnLostDevice();
	return m_screenPlane.OnLostDevice();
}

void CPreprocessRender_::OnDisposeDevice()
{
	m_effect.OnDisposeDevice();
	m_screenPlane.OnDisposeDevice();
}



