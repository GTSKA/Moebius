#pragma once
#ifndef GLfloat
typedef float GLfloat;
#endif
#define M_PI  3.141593

//Vector2
class Vector2
{
public:
	//Constructors
	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(GLfloat _x, GLfloat _y) : x(_x), y(_y) {}
	Vector2(GLfloat * pArg) : x(pArg[0]), y(pArg[1]) {}
	Vector2(Vector2 & vector) : x(vector.x), y(vector.y) {}

	//Vector's operations
	GLfloat Length();
	Vector2 & Normalize();
	Vector2 operator + (Vector2 & vector);
	Vector2 & operator += (Vector2 & vector);
	Vector2 operator - ();
	Vector2 operator - (Vector2 & vector);
	Vector2 & operator -= (Vector2 & vector);
	Vector2 operator * (GLfloat k);
	Vector2 & operator *= (GLfloat k);
	Vector2 operator / (GLfloat k);
	Vector2 & operator /= (GLfloat k);
	Vector2 & operator = (Vector2 & vector);
	Vector2 Modulate(Vector2 & vector);
	GLfloat Dot(Vector2 & vector);

	//access to elements
	GLfloat operator [] (unsigned int idx);

	//data members
	GLfloat x;
	GLfloat y;
};

//Vector3
class Vector3
{
public:
	//Constructors
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(GLfloat _x, GLfloat _y, GLfloat _z) : x(_x), y(_y), z(_z) {}
	Vector3(GLfloat * pArg) : x(pArg[0]), y(pArg[1]), z(pArg[2]) {}
	Vector3(Vector3 & vector) : x(vector.x), y(vector.y), z(vector.z) {}

	//Vector's operations
	GLfloat Length();
	Vector3 & Normalize();
	Vector3 operator + (Vector3 & vector);
	Vector3 & operator += (Vector3 & vector);
	Vector3 operator - ();
	Vector3 operator - (Vector3 & vector);
	Vector3 & operator -= (Vector3 & vector);
	Vector3 operator * (GLfloat k);
	Vector3 & operator *= (GLfloat k);
	Vector3 operator / (GLfloat k);
	Vector3 & operator /= (GLfloat k);
	Vector3 & operator = (Vector3 & vector);
	Vector3 Modulate(Vector3 & vector);
	GLfloat Dot(Vector3 & vector);
	Vector3 Cross(Vector3 & vector);

	static Vector3 Cross(Vector3&, Vector3&);
	static GLfloat Dot(Vector3&, Vector3&);
	static GLfloat Dist(Vector3&, Vector3&);


	//access to elements
	GLfloat operator [] (unsigned int idx);

	// data members
	GLfloat x;
	GLfloat y;
	GLfloat z;
};



class Matrix;

//Vector4
class Vector4
{
public:
	//Constructors
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(GLfloat _x, GLfloat _y, GLfloat _z) : x(_x), y(_y), z(_z), w(1.0f) {}
	Vector4(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vector4(GLfloat * pArg) : x(pArg[0]), y(pArg[1]), z(pArg[2]), w(pArg[3]) {}
	Vector4(Vector3 & vector) : x(vector.x), y(vector.y), z(vector.z), w(1.0f) {}
	Vector4(Vector3 & vector, GLfloat _w) : x(vector.x), y(vector.y), z(vector.z), w(_w) {}
	Vector4(Vector4 & vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w) {}

	//Vector's operations
	GLfloat Length();
	Vector4 & Normalize();
	Vector4 operator + (Vector4 & vector);
	Vector4 & operator += (Vector4 & vector);
	Vector4 operator - ();
	Vector4 operator - (Vector4 & vector);
	Vector4 & operator -= (Vector4 & vector);
	Vector4 operator * (GLfloat k);
	Vector4 & operator *= (GLfloat k);
	Vector4 operator / (GLfloat k);
	Vector4 & operator /= (GLfloat k);
	Vector4 & operator = (Vector4 & vector);
	Vector4 Modulate(Vector4 & vector);
	GLfloat Dot(Vector4 & vector);
	Vector3 xyz();

	//matrix multiplication
	Vector4 operator * (Matrix & m);

	//access to elements
	GLfloat operator [] (unsigned int idx);

	//data members
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
};

//Matrix 4 X 4
class Matrix
{
public:
	//constructors
	Matrix() {}
	Matrix(GLfloat val);
	Matrix(Matrix & mat);

	// Matrix operations
	Matrix & SetZero();
	Matrix & SetIdentity();

	Matrix & SetRotationX(GLfloat angle);
	Matrix & SetRotationY(GLfloat angle);
	Matrix & SetRotationZ(GLfloat angle);
	Matrix & SetRotationAngleAxis(float angle, float x, float y, float z);

	Matrix & SetScale(GLfloat scale);
	Matrix & SetScale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);
	Matrix & SetScale(GLfloat * pScale);
	Matrix & SetScale(Vector3 &scaleVec);

	Matrix & SetTranslation(GLfloat x, GLfloat y, GLfloat z);
	Matrix & SetTranslation(GLfloat *pTrans);
	Matrix & SetTranslation(Vector3 &vec);

	Matrix & SetPerspective(GLfloat fovY, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane);

	Matrix Transpose();

	Matrix operator + (Matrix & mat);
	Matrix & operator += (Matrix & mat);
	Matrix operator - (Matrix & mat);
	Matrix &operator -= (Matrix & mat);

	Matrix operator * (Matrix & mat);
	Matrix operator * (GLfloat k);
	Matrix & operator *= (GLfloat k);

	Vector4 operator * (Vector4 & vec);

	Matrix & operator = (Matrix & mat);

	//data members
	GLfloat m[4][4];
};

//Line
class Line
{
public:
	Line() {}
	Line(Vector3& _p0, Vector3& _d)
	{
		p0 = _p0;
		d = _d;
	}
	float DistanceToPoint(Vector3&);
	static bool ClosestPoint(Line&, Line&, Vector3&, Vector3&);
	static GLfloat Distance(Line&, Line&);
	static GLfloat DistanceSq(Line&, Line&);
	Vector3 p0;
	Vector3 d;
};

//Plane
class Plane
{
public:
	Plane();
	Plane(const GLfloat*);
	Plane(Vector4);
	Plane(GLfloat, GLfloat, GLfloat, GLfloat);
	Plane(Vector3&, GLfloat);

	//loads the plane from a point on the surface and a normal vector
	static Plane ConstructFromPointNormal(Vector3&, Vector3&);

	//loads the plane from a point on the surface and two vectors in the plane
	static Plane ConstructFromPointVectors(Vector3&, Vector3&, Vector3&);

	//loads the plane from 3 points on the surface
	static Plane ConstructFromPoints(Vector3&, Vector3&, Vector3&);

	// Normalization
	Plane Normalize();

	GLfloat UnsignedDistance(Vector3&);
	GLfloat SignedDistance(Vector3&);
	Vector3 ClosestPoint(Vector3&);

	//determines the intersect of the line defined by the points V1 and V2 with the plane.
	//If there is no intersection, Hit will be false.
	Vector3 IntersectLine(Vector3&, Vector3&, bool&);

	//determines the intersect of the line defined by the points V1 and V2 with the plane.
	//Returns the point of intersection.  Origin is returned if no intersection exists.
	Vector3 IntersectLine(Vector3&, Vector3&);

	//Paramaterize the line with the variable t such that t = 0 is V1 and t = 1 is V2.
	//returns the t for this line that lies on this plane.
	GLfloat IntersectLineRatio(Vector3&, Vector3&);

	Vector3 IntersectLine(Line&);



	//dot product of a plane and a 4D vector
	static GLfloat Dot(Plane&, Vector4&);
	//dot product of a plane and a 3D coordinate
	static GLfloat DotCoord(Plane&, Vector3&);
	//dot product of a plane and a 3D normal
	static GLfloat DotNormal(Plane&, Vector3&);

	static bool PlanePlaneIntersection(Plane&, Plane&, Line&);


	__forceinline Vector3 Plane::Normal() const
	{
		return Vector3(a, b, c);
	}
	__forceinline Plane Flip()
	{
		Plane result;
		result.a = -a;
		result.b = -b;
		result.c = -c;
		result.d = -d;
		return result;
	}

	


	bool operator == (const Plane& p) const;
	
	bool operator != (const Plane& p) const;
	

	friend Plane operator *(GLfloat k, const Plane& p);
	

	Plane operator * (GLfloat) const;
	Plane operator / (GLfloat) const;

	Plane& operator *= (GLfloat);
	Plane& operator /= (GLfloat);

	operator const GLfloat*() const;
	operator GLfloat*();

	//the (a, b, c, d) in a*x + b*y + c*z + d = 0.
	GLfloat a, b, c, d;
};