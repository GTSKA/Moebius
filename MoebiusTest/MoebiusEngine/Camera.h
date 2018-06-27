#pragma once
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
		Matrix getWorldMatrix();
		Matrix getViewMatrix();
		Matrix getProjectionMatrix();
		Vector3 getPosition();
		Vector3 getFogColor();
		float getFogStart();
		float getFogRange();
		void initFog(Vector3 fogColor, float fogStart, float fogRange);
		void calcWorldMatrix();
		void calcViewMatrix();
	protected:
		Vector3 m_positionInfo;
		Vector3 m_rotateInfo;
		Matrix m_worldMatrix;
		Matrix m_viewMatrix;
		Matrix m_projectionMatrix;
		float m_Speed;
		Vector3 m_fogColor;
		float m_fogStart;
		float m_fogRange;
	};
}

