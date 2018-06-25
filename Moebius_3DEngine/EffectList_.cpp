#include "StdAfx.h"
#include "EffectList_.h"

CEffectList_::CEffectList_(void)
{
}

CEffectList_::~CEffectList_(void)
{
}

HRESULT CEffectList_::AddEffect()
{
	m_effectList.Add();
	return S_OK;
}

HRESULT CEffectList_::AddEffect(LPDIRECT3DDEVICE9 pDevice, STRSAFE_LPCSTR effectCode)
{
	m_effectList.Add();
	return m_effectList[m_effectList.GetSize()-1].SetEffectCode(pDevice, effectCode);
}

HRESULT CEffectList_::AddEffect(LPDIRECT3DDEVICE9 pDevice, CEffectBase_ &effect)
{
	m_effectList.Add();
	return m_effectList[m_effectList.GetSize()-1].SetEffectCode(pDevice, effect.GetEffectCode());
}

HRESULT CEffectList_::AddEffect(LPDIRECT3DDEVICE9 pDevice, __MOEBIUS_EFFECT_PRESET preset)
{
	HRESULT hr = E_FAIL;

	if(preset == _MOEBIUS_EFFECT_BASIC)
	{
		m_effectList.Add();
		hr = m_effectList[m_effectList.GetSize()-1].SetEffectCode(pDevice, __SHADER_BASIC);
	}
	if(preset == _MOEBIUS_EFFECT_FRACTAL)
	{
		m_effectList.Add();
		hr = m_effectList[m_effectList.GetSize()-1].SetEffectCode(pDevice, __SHADER_FRACTALS);
	}

	if(preset == _MOEBIUS_EFFECT_NODES)
	{
		m_effectList.Add();
		hr = m_effectList[m_effectList.GetSize()-1].SetEffectCode(pDevice, __TTTT);
	}

	

	//m_effectList[0].m_Effect->SetFloat

	return hr;
}

HRESULT CEffectList_::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	for(UINT i=0; i<m_effectList.GetSize(); i++)
	{
		if(FAILED(m_effectList[i].OnResetDevice(pDevice)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CEffectList_::OnLostDevice()
{
	for(UINT i=0; i<m_effectList.GetSize(); i++)
	{
		if(FAILED(m_effectList[i].OnLostDevice()))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

void CEffectList_::OnDisposeDevice()
{
	for(UINT i=0; i<m_effectList.GetSize(); i++)
	{
		m_effectList[i].OnDisposeDevice();
	}
}

UINT CEffectList_::GetNumEffects()
{
	return m_effectList.GetSize();	
}
