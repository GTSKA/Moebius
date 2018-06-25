#include "StdAfx.h"
#include "RenderCtrl.h"

static D3DXMATRIX mWVP;

CRenderCtrl::CRenderCtrl(void)
{
}

CRenderCtrl::~CRenderCtrl(void)
{

}

HRESULT CRenderCtrl::InitDeviceObjects()
{
	m_textureList.SetDefaultPath(L"Data\\Textures\\2D");
	//m_textureList.AddTexture(_MOEBIUS_TEXTYPE_FILE, 0,0,D3DXCOLOR(1,1,0,1), L"Data\\Textures\\2D\\Signs 2.jpg", __TEXTURESHADER_RAINBOW4);
	m_textureList.AddTexture(L"Signs 2.jpg");	
	m_textureList.AddTexture(L"Signs 1.jpg");	
	m_textureList.AddTexture(__TEXTURESHADER_RAINBOW4);

	//m_geom.AddQuad(m_pd3dDevice, D3DXVECTOR3(-0.5f, -0.5,0),  D3DXVECTOR3(0.5, 0.5,0)); 
	//m_geom.AddQuad(m_pd3dDevice, D3DXVECTOR3(-1.5f, -1.5,0),  D3DXVECTOR3(-0.5, -0.5,0)); 
	//m_geom.AddQuad(m_pd3dDevice, D3DXVECTOR3(0.5f, 0.5,0),  D3DXVECTOR3(1.5, 1.5,0)); 
	//m_quad.AddQuad(m_pd3dDevice, D3DXVECTOR3(-1.5f, -0.5,0),  D3DXVECTOR3(-0.5, 0.5,0)); 



	//m_quad.DeleteQuad(m_pd3dDevice, 2);
	//m_quad.EditQuad(m_pd3dDevice, 0, D3DXVECTOR3(-1.5f, -0.5,0),  D3DXVECTOR3(-0.5, 0.5,0));

	m_geometryList.AddGeometry();
	m_geometryList[0].AddQuad(m_pd3dDevice, D3DXVECTOR3(-0.5f, -0.5,0),  D3DXVECTOR3(0.5, 0.5,0)); 
	m_geometryList[0].AddQuad(m_pd3dDevice, D3DXVECTOR3(-1.5f, -1.5,0),  D3DXVECTOR3(-0.5, -0.5,0)); 
	m_geometryList[0].AddQuad(m_pd3dDevice, D3DXVECTOR3(0.5f, 0.5,0),  D3DXVECTOR3(1.5, 1.5,0)); 

	return S_OK;
}

HRESULT CRenderCtrl::RestoreDeviceObjects()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	D3DVIEWPORT9 viewPort;
	this->m_pd3dDevice->GetViewport(&viewPort);
	
	D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian(45.0f), 
		(((FLOAT)viewPort.Width)/((FLOAT)viewPort.Height)),
		0.1f, 100.0f);
	//   m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	D3DXMATRIX matWorld;
    D3DXMatrixTranslation( &matWorld, 0.0f, 0.0f, 8.0f );
    //m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	//D3DXMatrixMultiply(&mWVP,&matWorld,&matProj); 

	//m_effect.m_Effect->SetTechnique("Textured");
	m_effectList[0].m_Effect->SetTechnique("Textured");

	camera.Create(true, true,
		D3DXVECTOR3(0,0,8),
		D3DXVECTOR3(0,0,0),
		(((FLOAT)viewPort.Width)/((FLOAT)viewPort.Height)),
		0.1f,
		100.0f);

	//m_textureList[0].SetPercentLevel((FLOAT)25.0f);

	return S_OK;
}

void CRenderCtrl::Render()
{
	m_pd3dDevice->BeginScene();

	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(0.5,0.5,0.60,1), 1.0f, 0 );

	//D3DXMATRIX m;
	//D3DXMatrixScaling(&m, 500, 500, 500);
	//m_sprite.m_sprite->SetTransform(&m);
	//m_sprite.m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
	//m_sprite.m_sprite->Draw(m_t.m_texture, NULL, NULL, NULL, D3DXCOLOR(1,1,1,1));
	//m_sprite.m_sprite->End();

	//m_pd3dDevice->SetTexture(0, m_t.m_texture);

	//m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//m_quad.RenderQuad_LIST(m_pd3dDevice);

	//m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//m_quad.RenderQuad_LIST(m_pd3dDevice, (UINT)0);

	//D3DXMATRIX matWorld, w;
 //   D3DXMatrixTranslation( &matWorld, 0.0f, 0.0f, 8.0f );
	//D3DXMatrixMultiply(&w, &matWorld, &camera.m_Projection);

	D3DXVECTOR4 v(1,1,0,1);

	//m_effect.m_Effect->SetTexture("diffuseTexture_01", m_textureList[0].m_texture);
	//
	//m_effect.m_Effect->SetMatrix("WorldViewProjection_Matrix", &camera.m_ViewProjection);
	//m_effect.m_Effect->SetVector("diffuseColor", &v);

	//UINT passes = 0;
	//m_effect.m_Effect->Begin(&passes, 0);

	//for(UINT  ip = 0; ip<passes; ip++)
	//{
	//	m_effect.m_Effect->BeginPass(ip);
	//	m_geometryList[0].Render(m_pd3dDevice);
	//	//m_quad.Render(m_pd3dDevice, 1);
	//	m_effect.m_Effect->EndPass();
	//}
	//m_effect.m_Effect->End();



	m_effectList[0].m_Effect->SetTexture("diffuseTexture_01", m_textureList[0].m_texture);
	m_effectList[0].m_Effect->SetMatrix("WorldViewProjection_Matrix", &camera.m_ViewProjection);
	m_effectList[0].m_Effect->SetVector("diffuseColor", &v);

	UINT passes = 0;
	m_effectList[0].m_Effect->Begin(&passes, 0);
	for(UINT  ip = 0; ip<passes; ip++)
	{
		m_effectList[0].m_Effect->BeginPass(ip);
		m_geometryList[0].Render(m_pd3dDevice);
		m_effectList[0].m_Effect->EndPass();
	}
	m_effectList[0].m_Effect->End();

	////m_pd3dxFont->DrawText( NULL, m_FrameStats, -1, NULL, DT_NOCLIP, 
	////		                 D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));

	D3DXVECTOR3 vPickRayDir;
    D3DXVECTOR3 vPickRayOrig;

	camera.MousetoViewVector(xMouse, yMouse, m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 
		&vPickRayOrig,
		&vPickRayDir);

	WCHAR msg[255] = {0};
	UINT q;
	if(m_geometryList[0].RayIntersect(vPickRayOrig, vPickRayDir, &q))
	{
		StringCchPrintf(msg, 255 * sizeof(WCHAR), L"HIT!!!!!, Quad = %d ", q);
		m_pd3dxFont->DrawText( NULL, msg, -1, NULL, DT_NOCLIP, 
			                 D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	}
	else
	{
		StringCchPrintf(msg, 255 * sizeof(WCHAR), L"X= %d, Y= %d", 
			//m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight);
			_State->GetScreenWidth(), _State->GetScreenHeight());

		m_pd3dxFont->DrawText( NULL, msg, -1, NULL, DT_NOCLIP, 
			                 D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	}
	m_pd3dDevice->EndScene();
}


HRESULT CRenderCtrl::LostDeviceObjects()
{
	return S_OK;
}

void CRenderCtrl::DisposeDeviceObjects()
{
	
}