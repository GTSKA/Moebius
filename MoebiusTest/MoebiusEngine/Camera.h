#pragma once
#include "CVector_.h"
#include "CMatrix_.h"
namespace MoebiusEngine
{
	class Camera
	{
	public:
		Camera();
		~Camera();
		void Init();
		void Init(float i_fov, float i_near, float i_far, float i_speed);
		void moveForward(float deltaTime);
		void moveBackward(float deltaTime);
		void moveLeft(float deltaTime);
		void moveRight(float deltaTime);
		void rotateUp(float deltaTime);
		void rotateDown(float deltaTime);
		void rotateLeft(float deltaTime);
		void rotateRight(float deltaTime);
		CMATRIX_FLOAT4 getWorldMatrix();
		CMATRIX_FLOAT4 getViewMatrix();
		CMATRIX_FLOAT4 getProjectionMatrix();
		CVECTOR_FLOAT3 getPosition();
		CVECTOR_FLOAT3 getFogColor();
		float getFogStart();
		float getFogRange();
		void initFog(CVECTOR_FLOAT3 fogColor, float fogStart, float fogRange);
		void calcWorldMatrix();
		void calcViewMatrix();
	protected:
		CVECTOR_FLOAT3 m_positionInfo;
		CVECTOR_FLOAT3 m_rotateInfo;
		CMATRIX_FLOAT4 m_worldMatrix;
		CMATRIX_FLOAT4 m_viewMatrix;
		CMATRIX_FLOAT4 m_projectionMatrix;
		float m_Speed;
		CVECTOR_FLOAT3 m_fogColor;
		float m_fogStart;
		float m_fogRange;
	};
}

