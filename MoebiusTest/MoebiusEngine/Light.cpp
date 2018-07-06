#include "Light.h"
#include "MathUtils.h"

namespace MoebiusEngine
{
	Light::Light()
	{
	}


	Light::~Light()
	{
	}

	bool Light::Init(unsigned int Id, LightType type, Vector3 pos_dir, Vector3 color, MovingAttributes* movement)
	{
		m_Id = Id;
		m_PosDir = pos_dir;
		m_color = color;
		m_Type = type;
		if (type == LIGHT_DIRECTIONAL)
		{
			m_PosDir = m_PosDir.Normalize();
		}
		if (movement->moving)
		{
			m_speed = movement->speed;
			m_radius = movement->radius;
			m_angle = 0;
			m_MoveType = movement->type;
		}
		else
		{
			m_speed = 0.0f;
			m_radius = 0.0f;
			CalcWorldMatrix();
		}
		m_isMoving = movement->moving;
		return true;
	}

	void Light::Update(float deltaTime)
	{
		if (m_isMoving)
		{
			if (m_MoveType == LIGHTMOVE_CIRCLE)
			{
				m_angle += m_speed * deltaTime * (float)M_PI / 180.0f;
			}
			CalcWorldMatrix();
		}
	}
	void Light::CalcWorldMatrix()
	{
		Vector4 newPos;
		newPos.x = 0.0f; newPos.y = 0.0f; newPos.z = 0.0f; newPos.w = 0.0f;
		//move the light
		if (m_MoveType == LIGHTMOVE_CIRCLE)
		{
			Vector4 radiusVector;
			Matrix posTraslation;
			posTraslation.SetIdentity();
			Matrix rotation;
			rotation.SetIdentity();
			rotation.SetRotationY(m_angle);
			radiusVector.x = m_radius; radiusVector.y = 0; radiusVector.z = 0; radiusVector.w = 1;
			posTraslation.SetTranslation(m_PosDir);
			newPos = radiusVector * rotation * posTraslation;
			m_movedPos.x = newPos.x;
			m_movedPos.y = newPos.y;
			m_movedPos.z = newPos.z;
		}


		//init scale, rotation and traslation matrix
		Matrix rotX, rotY, rotZ;
		Matrix scale;
		Matrix translation;
		rotX.SetIdentity();
		rotY.SetIdentity();
		rotZ.SetIdentity();
		scale.SetIdentity();
		translation.SetIdentity();

		//Scale and rotation not calculated
		translation.SetTranslation(newPos.x, newPos.y, newPos.z);
		m_worldMatrix = scale * rotZ * rotX * rotY * translation;
	}
	Matrix Light::GetWorldMatrix()
	{
		return m_worldMatrix;
	}
	unsigned int Light::GetId()
	{
		return m_Id;
	}
	int Light::GetType()
	{
		return (int)m_Type;
	}
	Vector3 Light::GetColor()
	{
		return m_color;
	}
	Vector3 Light::GetPosDir()
	{
		if (m_Type == LIGHT_DIRECTIONAL)
		{
			return m_PosDir;
		}
		else
		{
			return m_movedPos;
		}
	}
}
