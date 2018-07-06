#pragma once
#include "MathUtils.h"
#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

namespace MoebiusEngine
{
	static enum LightType
	{
		LIGHT_DIRECTIONAL = 0,
		LIGHT_POINT = 1
	};
	static enum MoveType
	{
		LIGHTMOVE_CIRCLE = 0
	};
	struct MovingAttributes
	{
		bool moving;
		float speed;
		float radius;
		MoveType type;
	};
	class Light
	{
	public:
		Light();
		~Light();
		bool Init(unsigned int Id, LightType type, Vector3 pos_dir, Vector3 color, MovingAttributes* movement = NULL);
		void Update(float deltaTime);
		void CalcWorldMatrix();
		Matrix GetWorldMatrix();
		unsigned int GetId();
		int GetType();
		Vector3 GetColor();
		Vector3 GetPosDir();
	protected:
		unsigned int m_Id;
		LightType m_Type;
		Vector3 m_PosDir;
		Vector3 m_movedPos;
		Vector3 m_color;
		float m_speed;
		float m_radius;
		bool m_isMoving;
		float m_angle;
		Matrix m_worldMatrix;
		MoveType m_MoveType;
	};
}

