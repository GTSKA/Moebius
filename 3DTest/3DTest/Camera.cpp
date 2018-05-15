#include "Camera.h"
#include "Globals.h"

Camera::Camera()
{
	m_positionInfo = Vector3(0.0, 0.0, 5.0);
	m_rotateInfo = Vector3(0, M_PI, 0);
}


Camera::~Camera()
{
}
void Camera::Init()
{
	m_worldMatrix.SetIdentity();
	m_viewMatrix.SetIdentity();
	m_projectionMatrix.SetIdentity();
	float l_fov = 1;
	float l_near = 0.1f;
	float l_far = 500;
	m_projectionMatrix.SetPerspective(l_fov,  (float)Globals::screenWidth/ Globals::screenHeight, l_near, l_far);//This will change later, but by start is like this
}

void Camera::Init(float i_fov, float i_near, float i_far, float i_speed)
{
	m_worldMatrix.SetIdentity();
	m_viewMatrix.SetIdentity();
	m_projectionMatrix.SetIdentity();
	m_projectionMatrix.SetPerspective(i_fov, (float)Globals::screenWidth / Globals::screenHeight, i_near, i_far);//This will change later, but by start is like this
	m_Speed = i_speed;
}
void Camera::calcWorldMatrix()
{
	
	Matrix camRotationXMatrix;
	Matrix camRotationYMatrix;
	Matrix camTranslationMatrix;

	
	camRotationXMatrix.SetIdentity();
	camRotationYMatrix.SetIdentity();
	camTranslationMatrix.SetIdentity();

	
	camRotationXMatrix.SetRotationX(m_rotateInfo.x);
	camRotationYMatrix.SetRotationY(m_rotateInfo.y);

	m_worldMatrix = camRotationXMatrix * camRotationYMatrix * camTranslationMatrix; //OPENGL
	
}

void Camera::calcViewMatrix()
{
	Matrix camRotationXMatrix;
	Matrix camRotationYMatrix;
	Matrix camTranslationMatrix;
	camRotationXMatrix.SetIdentity();
	camRotationYMatrix.SetIdentity();
	camTranslationMatrix.SetIdentity();

	camTranslationMatrix.SetTranslation(-m_positionInfo.x, -m_positionInfo.y, -m_positionInfo.z);
	camRotationYMatrix.SetRotationY(-m_rotateInfo.y);
	camRotationXMatrix.SetRotationX(-m_rotateInfo.x);
	m_viewMatrix = camTranslationMatrix * camRotationYMatrix * camRotationXMatrix;
}

void Camera::moveForward(float deltaTime)
{
	Vector4 moveL;
	Matrix worldMatrix;
	Vector4 moveW;

	worldMatrix = getWorldMatrix();

	moveL = Vector4(0, 0, -deltaTime*Globals::SPEED_CAM, 1);
	moveW = moveL * worldMatrix;

	m_positionInfo = m_positionInfo + moveW.xyz();
	calcWorldMatrix();
	calcViewMatrix();
}

void Camera::moveBackward(float deltaTime)
{
	Vector4 moveL;
	Matrix worldMatrix;
	Vector4 moveW;

	worldMatrix = getWorldMatrix();

	moveL = Vector4(0, 0, deltaTime*Globals::SPEED_CAM, 1);
	moveW = moveL * worldMatrix;

	m_positionInfo = m_positionInfo + moveW.xyz();
	calcWorldMatrix();
	calcViewMatrix();
}
void Camera::moveRight(float deltaTime)
{
	Vector4 moveL;
	Matrix worldMatrix;
	Vector4 moveW;

	worldMatrix = getWorldMatrix();

	moveL = Vector4(deltaTime*Globals::SPEED_CAM, 0, 0, 1);
	moveW = moveL * worldMatrix;

	m_positionInfo = m_positionInfo + moveW.xyz();
	calcWorldMatrix();
	calcViewMatrix();
}
void Camera::moveLeft(float deltaTime)
{
	Vector4 moveL;
	Matrix worldMatrix;
	Vector4 moveW;

	worldMatrix = getWorldMatrix();

	moveL = Vector4(-deltaTime*Globals::SPEED_CAM, 0, 0, 1);
	moveW = moveL * worldMatrix;

	m_positionInfo = m_positionInfo + moveW.xyz();
	calcWorldMatrix();
	calcViewMatrix();
}

void Camera::rotateUp(float deltaTime)
{
	m_rotateInfo.x = (m_rotateInfo.x>(float)M_PI) ? m_rotateInfo.x - (float)M_PI : m_rotateInfo.x;
	float angle = m_rotateInfo.x * 180.0f / (float)M_PI;
	if (angle <= 90)
	{
		m_rotateInfo.x += (deltaTime * Globals::SPEED_CAM_ROT * (float)M_PI / 180.0f);
		calcWorldMatrix();
		calcViewMatrix();
	}
	else
	{
		angle = 0;
	}
}
void Camera::rotateDown(float deltaTime)
{
	float angle = m_rotateInfo.x * 180.0f / (float)M_PI;
	m_rotateInfo.x = (-m_rotateInfo.x>(float)M_PI) ? m_rotateInfo.x + (float)M_PI : m_rotateInfo.x;
	if (angle >= -90)
	{
		m_rotateInfo.x -= (deltaTime * Globals::SPEED_CAM_ROT * (float)M_PI / 180.0f);
		calcWorldMatrix();
		calcViewMatrix();
	}
	else 
	{
		angle = 0;
	}
}
void Camera::rotateLeft(float deltaTime)
{

	m_rotateInfo.y -= (deltaTime * Globals::SPEED_CAM_ROT * (float)M_PI / 180.0f);
	calcWorldMatrix();
	calcViewMatrix();
}
void Camera::rotateRight(float deltaTime)
{
	m_rotateInfo.y += (deltaTime * Globals::SPEED_CAM_ROT * (float)M_PI / 180.0f);
	calcWorldMatrix();
	calcViewMatrix();
}

Matrix Camera::getWorldMatrix()
{
	return m_worldMatrix;
}
Matrix Camera::getViewMatrix()
{
	return m_viewMatrix;
}
Matrix Camera::getProjectionMatrix()
{
	return m_projectionMatrix;
}

Vector3 Camera::getPosition()
{
	return m_positionInfo;
}

float Camera::getFogStart()
{
	return m_fogStart;
}

float Camera::getFogRange()
{
	return m_fogRange;
}

Vector3 Camera::getFogColor()
{
	return m_fogColor;
}

void Camera::initFog(Vector3 fogColor, float fogStart, float fogRange)
{
	m_fogColor = fogColor;
	m_fogStart = fogStart;
	m_fogRange = fogRange;
}