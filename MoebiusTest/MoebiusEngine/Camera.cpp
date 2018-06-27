#include "Camera.h"

namespace MoebiusEngine
{
	Camera::Camera()
	{
	}


	Camera::~Camera()
	{
	}

	void Camera::Init()
	{
	}

	void Camera::Init(float i_fov, float i_near, float i_far, float i_speed)
	{
	}

	void Camera::calcWorldMatrix()
	{
	}

	void Camera::calcViewMatrix()
	{
	}

	void Camera::moveForward(float deltaTime)
	{
	}

	void Camera::moveBackward(float deltaTime)
	{
	}

	void Camera::moveRight(float deltaTime)
	{
	}

	void Camera::moveLeft(float deltaTime)
	{
	}

	void Camera::rotateUp(float deltaTime)
	{
	}

	void Camera::rotateDown(float deltaTime)
	{
	}

	void Camera::rotateLeft(float deltaTime)
	{
	}

	void Camera::rotateRight(float deltaTime)
	{
	}

	Matrix Camera::getWorldMatrix()
	{
	}

	Matrix Camera::getViewMatrix()
	{
	}

	Matrix Camera::getProjectionMatrix()
	{
	}

	Vector3 Camera::getPosition()
	{
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
	}

	void Camera::initFog(Vector3 fogColor, float fogStart, float fogRange)
	{

	}

}
