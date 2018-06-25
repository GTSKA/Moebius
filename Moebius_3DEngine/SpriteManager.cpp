#include "StdAfx.h"
#include "SpriteManager.h"

CSpriteManager::CSpriteManager(void)
{
}

CSpriteManager::~CSpriteManager(void)
{
}

HRESULT CSpriteManager::CreateSprite(LPDIRECT3DDEVICE9 pDevice)
{
	return D3DXCreateSprite(pDevice, &m_sprite);
}

HRESULT CSpriteManager::OnResetDevice(LPDIRECT3DDEVICE9 pDevice)
{
	return m_sprite->OnResetDevice();
}

HRESULT CSpriteManager::OnLostDevice()
{
	return m_sprite->OnLostDevice();
}

void CSpriteManager::OnDisposeDevice()
{
	SAFE_RELEASE(m_sprite);
}



