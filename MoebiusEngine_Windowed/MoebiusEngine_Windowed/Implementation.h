#include "stdafx.h"
#include "Moebius_Core.h"

static D3DXVECTOR2 vDots[2];

HRESULT CDirectXManager_::CheckDeviceCaps()
{
	HRESULT hr = S_OK;
	if(SUCCEEDED(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		D3DFMT_X8R8G8B8,
		0,
		D3DRTYPE_SURFACE,
		(D3DFORMAT)MAKEFOURCC('A', 'T', 'O', 'C'))))
	{
		//MessageBox(NULL, L"Alphas chingones!!!", L"", 0);
	}

	return hr;
}

HRESULT CDirectXManager_::OneTimeInit(HWND hWnd, HINSTANCE hInst)
{
	HRESULT hr = S_OK;
	SetWindowText(hWnd, L"- Moebius Engine -");
	return hr;
}

HRESULT CDirectXManager_::InitDeviceObjects()
{
	HRESULT hr = S_OK;
	return hr;
}

HRESULT CDirectXManager_::ResetDeviceObjects()
{
	HRESULT hr = S_OK;
	return hr;
}

HRESULT CDirectXManager_::LostDeviceObjects()
{
	HRESULT hr = S_OK;
	return hr;
}
void CDirectXManager_::FrameUpdate()
{
	vDots[0] = D3DXVECTOR2(m_d3dpp.BackBufferWidth/2, m_d3dpp.BackBufferHeight/2);
	vDots[1] = D3DXVECTOR2((m_d3dpp.BackBufferWidth/2) + ( 800 * (FLOAT)cos((FLOAT)m_renderCounts*0.05f)),
		(m_d3dpp.BackBufferHeight/2) + ( 800 * (FLOAT)sin((FLOAT)m_renderCounts*0.05f)));
}

void CDirectXManager_::Render()
{
	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(0,0,1,1), 1.0f, 0 );

	WCHAR msg[255] = {0};
	wsprintf(msg, L"#Frames = %d, W: %d, H: %d", m_renderCounts, m_d3dpp.BackBufferWidth, 
		m_d3dpp.BackBufferHeight);

	RECT destRect1;
	SetRect( &destRect1, 0, 0, 0, 0 );

	m_pLine->SetWidth(15.0f);
	//m_pLine->SetAntialias(TRUE);

	m_pLine->Draw(
 
    m_pd3dDevice->BeginScene();
    {
		
		m_pLine->Begin();
		m_pLine->Draw(vDots, 2, D3DCOLOR_COLORVALUE(1,0,0,1));
		m_pLine->End();

		
		m_pd3dxFont->DrawText( NULL, msg, -1, &destRect1, DT_NOCLIP, 
		                   D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	m_pd3dDevice->EndScene();
}


