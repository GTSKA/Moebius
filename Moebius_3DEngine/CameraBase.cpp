#include "StdAfx.h"
#include "CameraBase.h"

CCameraBase_::CCameraBase_(void)
{
	D3DXMatrixIdentity(&m_View);
	D3DXMatrixIdentity(&m_Projection);
}

CCameraBase_::CCameraBase_(bool bUpdateViewMatrix, bool bUpdateProjectionMatrix, D3DXVECTOR3 camPosition, D3DXVECTOR3 camTarget, FLOAT fieldOfView, FLOAT zNear, FLOAT zFar, FLOAT AspectRadio, D3DXVECTOR3 vUp)
{
	Create(bUpdateViewMatrix, bUpdateProjectionMatrix, camPosition, camTarget, fieldOfView, zNear, zFar, AspectRadio, vUp);
}

CCameraBase_::~CCameraBase_(void)
{
}

void CCameraBase_::Create(bool bUpdateViewMatrix, bool bUpdateProjectionMatrix, D3DXVECTOR3 camPosition, D3DXVECTOR3 camTarget, FLOAT AspectRadio , FLOAT zNear, FLOAT zFar, FLOAT fieldOfView, D3DXVECTOR3 vUp)
{
	//View parameters
	m_cameraPosition = camPosition;
	m_cameraTarget = camTarget;
	m_cameraUp = vUp;
	m_ViewVector = m_cameraPosition - m_cameraTarget;
	
	//Projection parameters
	m_fieldOfView = fieldOfView;
	m_zNear = zNear;
	m_zFar = zFar;
	m_fAspectRadio = AspectRadio;

	D3DXMatrixIdentity(&m_View);
	D3DXMatrixIdentity(&m_Projection);

	UpdateMatrix(bUpdateViewMatrix, bUpdateProjectionMatrix);
}

void CCameraBase_::UpdateMatrix(bool bUpdateViewMatrix, bool bUpdateProjectionMatrix)
{
	if(bUpdateViewMatrix)
	{
		//Recreate the View Matrix
		D3DXMatrixLookAtLH(&m_View, &m_cameraPosition, &m_cameraTarget, &m_cameraUp); 
	}
	if(bUpdateProjectionMatrix)
	{
		D3DXMatrixPerspectiveFovLH(&m_Projection, m_fieldOfView, m_fAspectRadio, m_zNear, m_zFar);
	}

	if(bUpdateViewMatrix)
	{
		D3DXMatrixMultiply(&m_ViewProjection, &m_View, &m_Projection);
		D3DXMatrixInverse(&m_ViewInverse, NULL, &m_View);
		D3DXMatrixTranspose(&m_ViewInverseTranspose, &m_ViewInverse);

		D3DXMatrixInverse(&m_ViewProjectionInverse, NULL, &m_ViewProjection);
		D3DXMatrixInverse(&m_ViewProjectionInverseTranspose, NULL, &m_ViewProjectionInverse);
	}
	if(bUpdateProjectionMatrix)
	{
		D3DXMatrixInverse(&m_ProjectionInverse, NULL, &m_Projection);
		D3DXMatrixTranspose(&m_ProjectionInverseTranspose, &m_ProjectionInverse);
	}
}

void CCameraBase_::SetViewMatrixParameters(D3DXVECTOR3 camPosition, D3DXVECTOR3 camTarget, D3DXVECTOR3 vUp)
{
	//View parameters
	m_cameraPosition = camPosition;
	m_cameraTarget = camTarget;
	m_cameraUp = vUp;
	m_ViewVector = m_cameraPosition - m_cameraTarget;

	UpdateMatrix(true, false);
}

void CCameraBase_::SetProjectionParamters(FLOAT AspectRadio, FLOAT zNear, FLOAT zFar, FLOAT fieldOfView)
{
	//Projection parameters
	m_fieldOfView = fieldOfView;
	m_zNear = zNear;
	m_zFar = zFar;
	m_fAspectRadio = AspectRadio;

	//D3DXMatrixIdentity(&m_View);
	//D3DXMatrixIdentity(&m_Projection);

	UpdateMatrix();
}

void CCameraBase_::TransformCamera(D3DXMATRIX matrixTranform)
{
	D3DXMatrixMultiply(&m_View, &matrixTranform, &m_View);
	UpdateMatrix(true, false);
}

void CCameraBase_::OnFrameMove(FLOAT elpsedTime, FLOAT absTime)
{

}

void CCameraBase_::MousetoViewVector(int xMousePos, int yMousePos, UINT screenWidht, UINT screenHeight, D3DXVECTOR3 *vOrig, D3DXVECTOR3* vDir)
{
	D3DXVECTOR3 vp;
	vp.x =  (((2.0f * xMousePos)/screenWidht)-1)/m_Projection._11;
    vp.y = -(((2.0f * yMousePos)/screenHeight)-1)/m_Projection._22;
    vp.z =  1.0f;

	//Get teh screen space origin ray
    (*vOrig).x = m_ViewInverse._41;
    (*vOrig).y = m_ViewInverse._42;
    (*vOrig).z = m_ViewInverse._43;

	//Transform the screen space pick ray into 3D space
	(*vDir).x  = (vp.x * m_ViewInverse._11) + (vp.y * m_ViewInverse._21) + (vp.z * m_ViewInverse._31);
    (*vDir).y  = (vp.x * m_ViewInverse._12) + (vp.y * m_ViewInverse._22) + (vp.z * m_ViewInverse._32);
    (*vDir).z  = (vp.x * m_ViewInverse._13) + (vp.y * m_ViewInverse._23) + (vp.z * m_ViewInverse._33);
	D3DXVec3Normalize(vDir, vDir);
}

void CCameraBase_::MousetoViewVector(int xMousePos, int yMousePos, UINT screenWidht, UINT screenHeight, D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, FLOAT hDist)
{
	D3DXVECTOR3 vp;
	vp.x =  (((2.0f * xMousePos)/screenWidht)-1)/m_Projection._11;
    vp.y = -(((2.0f * yMousePos)/screenHeight)-1)/m_Projection._22;
    vp.z =  1.0f;

	D3DXVECTOR3 vDir;

	//Get teh screen space origin ray
    (*v1).x = m_ViewInverse._41;
    (*v1).y = m_ViewInverse._42;
    (*v1).z = m_ViewInverse._43;

	//Transform the screen space pick ray into 3D space
	(vDir).x  = (vp.x * m_ViewInverse._11) + (vp.y * m_ViewInverse._21) + (vp.z * m_ViewInverse._31);
    (vDir).y  = (vp.x * m_ViewInverse._12) + (vp.y * m_ViewInverse._22) + (vp.z * m_ViewInverse._32);
    (vDir).z  = (vp.x * m_ViewInverse._13) + (vp.y * m_ViewInverse._23) + (vp.z * m_ViewInverse._33);
	D3DXVec3Normalize(&vDir, &vDir);
	(*v2) = (*v1) + (hDist * vDir);
}


