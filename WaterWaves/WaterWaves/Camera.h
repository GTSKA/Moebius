#pragma once
#include "MathUtil.h"
class Camera
{
public:
	Camera();
	~Camera();
	void Init(float width, float height);
	void Init(float i_fov, float i_near, float i_far, float i_speed, float width, float height);
	void moveForward(float deltaTime);
	void moveBackward(float deltaTime);
	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);
	void rotateUp(float deltaTime);
	void rotateDown(float deltaTime);
	void rotateLeft(float deltaTime);
	void rotateRight(float deltaTime);
	Matrix getWorldMatrix();
	Matrix getViewMatrix();
	Matrix getProjectionMatrix();
	Vector3 getPosition();
	void calcWorldMatrix();
	void calcViewMatrix();
protected:
	Vector3 m_positionInfo;
	Vector3 m_rotateInfo;
	Matrix m_worldMatrix;
	Matrix m_viewMatrix;
	Matrix m_projectionMatrix;
	float m_Speed;
};

