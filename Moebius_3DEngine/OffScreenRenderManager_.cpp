#include "StdAfx.h"
#include "OffScreenRenderManager_.h"

COffScreenRenderManager_::COffScreenRenderManager_(void)
{
	m_defaultSurface = NULL;
}

COffScreenRenderManager_::~COffScreenRenderManager_(void)
{
}

HRESULT COffScreenRenderManager_::Begin(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DSURFACE9 renderSurface)
{
	return pDevice->SetRenderTarget(0, renderSurface);
}

HRESULT COffScreenRenderManager_::End(LPDIRECT3DDEVICE9 pDevice)
{
	return pDevice->SetRenderTarget(0, m_defaultSurface);
}

HRESULT COffScreenRenderManager_::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	SAFE_RELEASE(m_defaultSurface);
	return pDevice->GetRenderTarget(0, &m_defaultSurface);
}

HRESULT COffScreenRenderManager_::OnLostDevice()
{
	SAFE_RELEASE(m_defaultSurface);
	return S_OK;
}

void COffScreenRenderManager_::OnDisposeDevice()
{
	SAFE_RELEASE(m_defaultSurface);
}