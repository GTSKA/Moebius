#include "Frustum.h"


namespace MoebiusEngine
{
	Frustum::Frustum()
	{
	}

	Frustum::Frustum(const Frustum&)
	{}

	Frustum::~Frustum()
	{
	}

	void Frustum::constructFrustum(float screenDepth, Matrix projectionMatrix, Matrix viewMatrix)
	{
		float zMin, r;
		Matrix mat;

		//Calculate the minimum z in the Frustum
		zMin = -projectionMatrix.m[3][2] / projectionMatrix.m[2][2];
		r = screenDepth / (screenDepth - zMin);
		projectionMatrix.m[2][2] = r;
		projectionMatrix.m[3][2] = -r*zMin;

		//Create the Frustum matrix from the view matrix and updated projection matrix
		mat = viewMatrix*projectionMatrix;

		//calculate near plane of Frustum
		m_planes[0].a = mat.m[0][3] + mat.m[0][2];
		m_planes[0].b = mat.m[1][3] + mat.m[1][2];
		m_planes[0].c = mat.m[2][3] + mat.m[2][2];
		m_planes[0].d = mat.m[3][3] + mat.m[3][2];
		m_planes[0] = m_planes[0].Normalize();

		//calculate far plane of Frustum
		m_planes[1].a = mat.m[0][3] - mat.m[0][2];
		m_planes[1].b = mat.m[1][3] - mat.m[1][2];
		m_planes[1].c = mat.m[2][3] - mat.m[2][2];
		m_planes[1].d = mat.m[3][3] - mat.m[3][2];
		m_planes[1] = m_planes[1].Normalize();

		//calculate left plane of Frustum
		m_planes[2].a = mat.m[0][3] + mat.m[0][0];
		m_planes[2].b = mat.m[1][3] + mat.m[1][0];
		m_planes[2].c = mat.m[2][3] + mat.m[2][0];
		m_planes[2].d = mat.m[3][3] + mat.m[3][0];
		m_planes[2] = m_planes[2].Normalize();

		//calculate right plane of Frustum
		m_planes[3].a = mat.m[0][3] - mat.m[0][0];
		m_planes[3].b = mat.m[1][3] - mat.m[1][0];
		m_planes[3].c = mat.m[2][3] - mat.m[2][0];
		m_planes[3].d = mat.m[3][3] - mat.m[3][0];
		m_planes[3] = m_planes[3].Normalize();

		//calculate top plane of Frustum
		m_planes[4].a = mat.m[0][3] - mat.m[0][1];
		m_planes[4].b = mat.m[1][3] - mat.m[1][1];
		m_planes[4].c = mat.m[2][3] - mat.m[2][1];
		m_planes[4].d = mat.m[3][3] - mat.m[3][1];
		m_planes[4] = m_planes[4].Normalize();

		//calculate bottom plane of Frustum
		m_planes[5].a = mat.m[0][3] + mat.m[0][1];
		m_planes[5].b = mat.m[1][3] + mat.m[1][1];
		m_planes[5].c = mat.m[2][3] + mat.m[2][1];
		m_planes[5].d = mat.m[3][3] + mat.m[3][1];
		m_planes[5] = m_planes[5].Normalize();	
	}

	bool Frustum::checkPoint(float x, float y , float z)
	{
		for (register int i = 0; i < 6; ++i)
		{
			if (Plane::DotCoord(m_planes[i], Vector3(x, y, z)) < 0.0f)
				return false;
		}
		return true;

	}
	bool Frustum::checkPoint(Vector3& point)
	{
		for (register int i = 0; i < 6; ++i)
		{
			if (Plane::DotCoord(m_planes[i], point) < 0.0f)
				return false;
		}
		return true;
	}
	bool Frustum::checkCube(float xCenter, float yCenter, float zCenter, float radius)
	{
		for (register int i = 0; i < 6; ++i)
		{
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter - radius, yCenter - radius, zCenter - radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter + radius, yCenter - radius, zCenter - radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter - radius, yCenter + radius, zCenter - radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter + radius, yCenter + radius, zCenter - radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter - radius, yCenter - radius, zCenter + radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter + radius, yCenter - radius, zCenter + radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter - radius, yCenter + radius, zCenter + radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter + radius, yCenter + radius, zCenter + radius)) >= 0.0f)
				continue;

			return false;
		}
		return true;
	}
	bool Frustum::checkCube(Vector3& center, float radius)
	{
		for (register int i = 0; i < 6; ++i)
		{
			if (Plane::DotCoord(m_planes[i], Vector3(center.x - radius, center.y - radius, center.z - radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x + radius, center.y - radius, center.z - radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x - radius, center.y + radius, center.z - radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x + radius, center.y + radius, center.z - radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x - radius, center.y - radius, center.z + radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x + radius, center.y - radius, center.z + radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x - radius, center.y + radius, center.z + radius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x + radius, center.y + radius, center.z + radius)) >= 0.0f)
				continue;

			return false;
		}
		return true;
	}
	bool Frustum::checkSphere(float xCenter, float yCenter, float zCenter, float radius)
	{
		for (register int i = 0; i < 6; ++i)
		{
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter, yCenter, zCenter)) < -radius)
				return false;
		}
		return true;
	}
	bool Frustum::checkSphere(Vector3& center, float radius)
	{
		float val;
		for (register int i = 0; i < 6; ++i)
		{
			val = Plane::DotCoord(m_planes[i], center);
			if (val < -radius)
				return false;
		}
		return true;
	}
	bool Frustum::checkRectangle(float xCenter, float yCenter, float zCenter, float xRadius, float yRadius, float zRadius)
	{
		for (register int i = 0; i < 6; ++i)
		{
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter - xRadius, yCenter - yRadius, zCenter - zRadius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter + xRadius, yCenter - yRadius, zCenter - zRadius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter - xRadius, yCenter + yRadius, zCenter - zRadius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter + xRadius, yCenter + yRadius, zCenter - zRadius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter - xRadius, yCenter - yRadius, zCenter + zRadius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter + xRadius, yCenter - yRadius, zCenter + zRadius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter - xRadius, yCenter + yRadius, zCenter + zRadius)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(xCenter + xRadius, yCenter + yRadius, zCenter + zRadius)) >= 0.0f)
				continue;
			return false;
		}
		return true;
	}

	bool Frustum::checkRectangle(Vector3& center, Vector3& radius)
	{
		for (register int i = 0; i < 6; ++i)
		{
			if (Plane::DotCoord(m_planes[i], Vector3(center.x - radius.x, center.y - radius.y, center.z - radius.z)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x + radius.x, center.y - radius.y, center.z - radius.z)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x - radius.x, center.y + radius.y, center.z - radius.z)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x + radius.x, center.y + radius.y, center.z - radius.z)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x - radius.x, center.y - radius.y, center.z + radius.z)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x + radius.x, center.y - radius.y, center.z + radius.z)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x - radius.x, center.y + radius.y, center.z + radius.z)) >= 0.0f)
				continue;
			if (Plane::DotCoord(m_planes[i], Vector3(center.x + radius.x, center.y + radius.y, center.z + radius.z)) >= 0.0f)
				continue;

			return false;
		}
		return true;
	}
}