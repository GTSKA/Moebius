#pragma once
#include "MathUtils.h"
namespace MoebiusEngine
{
	class Frustum
	{
	public:
		Frustum();
		Frustum(const Frustum&);
		~Frustum();

		void constructFrustum(float, Matrix, Matrix);
		//CheckPoint checks if a single point is inside the viewing frustum
		bool checkPoint(float, float, float);
		//CheckPoint checks if a single point is inside the viewing frustum
		bool checkPoint(Vector3&);
		//CheckCube checks if any of the eight corner points of the cube are inside the viewing frustum
		bool checkCube(float, float, float, float);
		//CheckCube checks if any of the eight corner points of the cube are inside the viewing frustum
		bool checkCube(Vector3&, float);
		//CheckSphere checks if the radius of the sphere from the center point is inside all six planes of the viewing frustum.
		bool checkSphere(float, float, float, float);
		//CheckSphere checks if the radius of the sphere from the center point is inside all six planes of the viewing frustum.
		bool checkSphere(Vector3&, float);
		//CheckRectangle works the same as CheckCube except that that it takes as input the x radius, y radius, and z radius of the rectangle instead of just a single radius of a cube
		bool checkRectangle(float, float, float, float, float, float);
		//CheckRectangle works the same as CheckCube except that that it takes as input the x radius, y radius, and z radius of the rectangle instead of just a single radius of a cube
		bool checkRectangle(Vector3&, Vector3&);
	protected:
		Plane m_planes[6];
	};
}
