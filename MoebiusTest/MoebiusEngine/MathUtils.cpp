#include "MathUtils.h"
#include <math.h>

//Vector2
GLfloat Vector2::Length()
{
	return sqrtf(x*x + y*y);
}

Vector2 & Vector2::Normalize()
{
	GLfloat lenInv = 1.0f / Length();
	x *= lenInv;
	y *= lenInv;

	return *this;
}

Vector2 Vector2::operator + (Vector2 & vector)
{
	return Vector2(x + vector.x, y + vector.y);
}

Vector2 & Vector2::operator += (Vector2 & vector)
{
	x += vector.x;
	y += vector.y;

	return *this;
}

Vector2 Vector2::operator - ()
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator - (Vector2 & vector)
{
	return Vector2(x - vector.x, y - vector.y);
}

Vector2 & Vector2::operator -= (Vector2 & vector)
{
	x -= vector.x;
	y -= vector.y;

	return *this;
}

Vector2 Vector2::operator * (GLfloat k)
{
	return Vector2(x * k, y * k);
}

Vector2 & Vector2::operator *= (GLfloat k)
{
	x *= k;
	y *= k;

	return *this;
}

Vector2 Vector2::operator / (GLfloat k)
{
	GLfloat kInv = 1.0f / k;
	return Vector2(x * kInv, y * kInv);
}

Vector2 & Vector2::operator /= (GLfloat k)
{
	return operator *= (1.0f / k);
}

Vector2 & Vector2::operator = (Vector2 & vector)
{
	x = vector.x;
	y = vector.y;

	return *this;
}

GLfloat Vector2::operator [] (unsigned int idx)
{
	return (&x)[idx];
}

Vector2 Vector2::Modulate(Vector2 & vector)
{
	return Vector2(x * vector.x, y * vector.y);
}

GLfloat Vector2::Dot(Vector2 & vector)
{
	return x * vector.x + y * vector.y;
}

//Vector3
GLfloat Vector3::Length()
{
	return sqrtf(x*x + y*y + z*z);
}

Vector3 & Vector3::Normalize()
{
	GLfloat lenInv = 1.0f / Length();
	x *= lenInv;
	y *= lenInv;
	z *= lenInv;

	return *this;
}

Vector3 Vector3::operator + (Vector3 & vector)
{
	return Vector3(x + vector.x, y + vector.y, z + vector.z);
}

Vector3 & Vector3::operator += (Vector3 & vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;

	return *this;
}

Vector3 Vector3::operator - ()
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator - (Vector3 & vector)
{
	return Vector3(x - vector.x, y - vector.y, z - vector.z);
}


Vector3 & Vector3::operator -= (Vector3 & vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;

	return *this;
}

Vector3 Vector3::operator * (GLfloat k)
{
	return Vector3(x * k, y * k, z * k);
}

Vector3 & Vector3::operator *= (GLfloat k)
{
	x *= k;
	y *= k;
	z *= k;

	return *this;
}

Vector3 Vector3::operator / (GLfloat k)
{
	GLfloat kInv = 1.0f / k;
	return Vector3(x * kInv, y * kInv, z * kInv);
}

Vector3 & Vector3::operator /= (GLfloat k)
{
	return operator *= (1.0f / k);
}

Vector3 & Vector3::operator = (Vector3 & vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;

	return *this;
}

GLfloat Vector3::operator [] (unsigned int idx)
{
	return (&x)[idx];
}

Vector3 Vector3::Modulate(Vector3 & vector)
{
	return Vector3(x * vector.x, y * vector.y, z * vector.z);
}

GLfloat Vector3::Dot(Vector3 & vector)
{
	return x * vector.x + y * vector.y + z * vector.z;
}

Vector3 Vector3::Cross(Vector3 & vector)
{
	return Vector3(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
}

Vector3 Vector3::Cross(Vector3& v1, Vector3& v2)
{
	return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}
GLfloat Vector3::Dot(Vector3& v1, Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

GLfloat Vector3::Dist(Vector3& left, Vector3& right)
{
	GLfloat xdiff = right.x - left.x;
	GLfloat ydiff = right.y - left.y;
	GLfloat zdiff = right.z - left.z;
	return sqrtf(xdiff*xdiff + ydiff*ydiff + zdiff*zdiff);
}

//Vector4
GLfloat Vector4::Length()
{
	return sqrtf(x*x + y*y + z*z + w*w);
}

Vector4 & Vector4::Normalize()
{
	GLfloat lenInv = 1.0f / Length();
	x *= lenInv;
	y *= lenInv;
	z *= lenInv;
	w *= lenInv;

	return *this;
}

Vector4 Vector4::operator + (Vector4 & vector)
{
	return Vector4(x + vector.x, y + vector.y, z + vector.z, w + vector.w);
}

Vector4 & Vector4::operator += (Vector4 & vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
	w += vector.w;

	return *this;
}

Vector4 Vector4::operator - ()
{
	return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator - (Vector4 & vector)
{
	return Vector4(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
}

Vector4 & Vector4::operator -= (Vector4 & vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
	w -= vector.w;

	return *this;
}

Vector4 Vector4::operator * (GLfloat k)
{
	return Vector4(x * k, y * k, z * k, w * k);
}

Vector4 & Vector4::operator *= (GLfloat k)
{
	x *= k;
	y *= k;
	z *= k;
	w *= k;

	return *this;
}

Vector4 Vector4::operator / (GLfloat k)
{
	GLfloat kInv = 1.0f / k;
	return Vector4(x * kInv, y * kInv, z * kInv, w * kInv);
}

Vector4 & Vector4::operator /= (GLfloat k)
{
	return operator *= (1.0f / k);
}

Vector4 & Vector4::operator = (Vector4 & vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	w = vector.w;
	return *this;
}

GLfloat Vector4::operator [] (unsigned int idx)
{
	return (&x)[idx];
}

Vector4 Vector4::Modulate(Vector4 & vector)
{
	return Vector4(x * vector.x, y * vector.y, z * vector.z, w * vector.w);
}

GLfloat Vector4::Dot(Vector4 & vector)
{
	return x * vector.x + y * vector.y + z * vector.z + w * vector.w;
}


Vector4 Vector4::operator * (Matrix & m)
{
	Vector4 res;
	res.x = x * m.m[0][0] + y * m.m[1][0] + z * m.m[2][0] + w * m.m[3][0];
	res.y = x * m.m[0][1] + y * m.m[1][1] + z * m.m[2][1] + w * m.m[3][1];
	res.z = x * m.m[0][2] + y * m.m[1][2] + z * m.m[2][2] + w * m.m[3][2];
	res.w = x * m.m[0][3] + y * m.m[1][3] + z * m.m[2][3] + w * m.m[3][3];

	return res;
}

Vector3 Vector4::xyz()
{
	return Vector3(x, y, z);
}

//Matrix 4 X 4
Matrix::Matrix(GLfloat val)
{
	m[0][0] = val; m[0][1] = val; m[0][2] = val; m[0][3] = val;
	m[1][0] = val; m[1][1] = val; m[1][2] = val; m[1][3] = val;
	m[2][0] = val; m[2][1] = val; m[2][2] = val; m[2][3] = val;
	m[3][0] = val; m[3][1] = val; m[3][2] = val; m[3][3] = val;
}

Matrix::Matrix(Matrix & mat)
{
	m[0][0] = mat.m[0][0]; m[0][1] = mat.m[0][1]; m[0][2] = mat.m[0][2]; m[0][3] = mat.m[0][3];
	m[1][0] = mat.m[1][0]; m[1][1] = mat.m[1][1]; m[1][2] = mat.m[1][2]; m[1][3] = mat.m[1][3];
	m[2][0] = mat.m[2][0]; m[2][1] = mat.m[2][1]; m[2][2] = mat.m[2][2]; m[2][3] = mat.m[2][3];
	m[3][0] = mat.m[3][0]; m[3][1] = mat.m[3][1]; m[3][2] = mat.m[3][2]; m[3][3] = mat.m[3][3];
}

Matrix & Matrix::SetZero()
{
	m[0][0] = 0.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 0.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 0.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 0.0f;

	return *this;
}

Matrix & Matrix::SetIdentity()
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

	return *this;
}

Matrix & Matrix::SetRotationX(GLfloat angle)
{
	GLfloat s = sinf(angle);
	GLfloat c = cosf(angle);
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = c; m[1][2] = s; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = -s; m[2][2] = c; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	return *this;
}

Matrix & Matrix::SetRotationY(GLfloat angle)
{
	GLfloat s = sinf(angle);
	GLfloat c = cosf(angle);
	m[0][0] = c; m[0][1] = 0.0f; m[0][2] = -s; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = s; m[2][1] = 0.0f; m[2][2] = c; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	return *this;
}

Matrix & Matrix::SetRotationZ(GLfloat angle)
{
	GLfloat s = sinf(angle);
	GLfloat c = cosf(angle);
	m[0][0] = c; m[0][1] = s; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = -s; m[1][1] = c; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	return *this;
}

Matrix & Matrix::SetRotationAngleAxis(float angle, float x, float y, float z)
{
	float sinAngle, cosAngle;
	float mag = sqrtf(x * x + y * y + z * z);

	sinAngle = sinf(angle);
	cosAngle = cosf(angle);
	if (mag > 0.0f)
	{
		float xx, yy, zz, xy, yz, zx, xs, ys, zs;
		float oneMinusCos;
		float magInv = 1.0f / mag;

		x *= magInv;
		y *= magInv;
		z *= magInv;

		xx = x * x;
		yy = y * y;
		zz = z * z;
		xy = x * y;
		yz = y * z;
		zx = z * x;
		xs = x * sinAngle;
		ys = y * sinAngle;
		zs = z * sinAngle;
		oneMinusCos = 1.0f - cosAngle;

		m[0][0] = (oneMinusCos * xx) + cosAngle;
		m[1][0] = (oneMinusCos * xy) - zs;
		m[2][0] = (oneMinusCos * zx) + ys;
		m[3][0] = 0.0F;

		m[0][1] = (oneMinusCos * xy) + zs;
		m[1][1] = (oneMinusCos * yy) + cosAngle;
		m[2][1] = (oneMinusCos * yz) - xs;
		m[3][1] = 0.0F;

		m[0][2] = (oneMinusCos * zx) - ys;
		m[1][2] = (oneMinusCos * yz) + xs;
		m[2][2] = (oneMinusCos * zz) + cosAngle;
		m[3][2] = 0.0F;

		m[0][3] = 0.0F;
		m[1][3] = 0.0F;
		m[2][3] = 0.0F;
		m[3][3] = 1.0F;
		return *this;
	}
	else
		return SetIdentity();
}


Matrix & Matrix::SetScale(GLfloat scale)
{
	m[0][0] = scale; m[0][1] = 0.0f;  m[0][2] = 0.0f;  m[0][3] = 0.0f;
	m[1][0] = 0.0f;  m[1][1] = scale; m[1][2] = 0.0f;  m[1][3] = 0.0f;
	m[2][0] = 0.0f;  m[2][1] = 0.0f;  m[2][2] = scale; m[2][3] = 0.0f;
	m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;

	return *this;
}

Matrix & Matrix::SetScale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ)
{
	m[0][0] = scaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
	m[1][0] = 0.0f;   m[1][1] = scaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
	m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = scaleZ; m[2][3] = 0.0f;
	m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
	return *this;
}

Matrix & Matrix::SetScale(GLfloat * pScale)
{
	m[0][0] = pScale[0];   m[0][1] = 0.0f;        m[0][2] = 0.0f;        m[0][3] = 0.0f;
	m[1][0] = 0.0f;        m[1][1] = pScale[1];   m[1][2] = 0.0f;        m[1][3] = 0.0f;
	m[2][0] = 0.0f;        m[2][1] = 0.0f;        m[2][2] = pScale[2];   m[2][3] = 0.0f;
	m[3][0] = 0.0f;        m[3][1] = 0.0f;        m[3][2] = 0.0f;        m[3][3] = 1.0f;

	return *this;
}

Matrix & Matrix::SetScale(Vector3 & scaleVec)
{
	m[0][0] = scaleVec.x; m[0][1] = 0.0f;       m[0][2] = 0.0f;       m[0][3] = 0.0f;
	m[1][0] = 0.0f;       m[1][1] = scaleVec.y; m[1][2] = 0.0f;       m[1][3] = 0.0f;
	m[2][0] = 0.0f;       m[2][1] = 0.0f;       m[2][2] = scaleVec.z; m[2][3] = 0.0f;
	m[3][0] = 0.0f;       m[3][1] = 0.0f;       m[3][2] = 0.0f;       m[3][3] = 1.0f;

	return *this;
}

Matrix & Matrix::SetTranslation(GLfloat x, GLfloat y, GLfloat z)
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = x; m[3][1] = y; m[3][2] = z; m[3][3] = 1.0f;

	return *this;
}

Matrix & Matrix::SetTranslation(GLfloat *pTrans)
{
	m[0][0] = 1.0f;      m[0][1] = 0.0f;      m[0][2] = 0.0f;      m[0][3] = 0.0f;
	m[1][0] = 0.0f;      m[1][1] = 1.0f;      m[1][2] = 0.0f;      m[1][3] = 0.0f;
	m[2][0] = 0.0f;      m[2][1] = 0.0f;      m[2][2] = 1.0f;      m[2][3] = 0.0f;
	m[3][0] = pTrans[0]; m[3][1] = pTrans[1]; m[3][2] = pTrans[2]; m[3][3] = 1.0f;

	return *this;
}

Matrix & Matrix::SetTranslation(Vector3 &vec)
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = vec.x; m[3][1] = vec.y; m[3][2] = vec.z; m[3][3] = 1.0f;

	return *this;
}

Matrix & Matrix::SetPerspective(GLfloat fovY, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane)
{
#ifndef OPENGL_PROJECT
	GLfloat height = 1.0f / tanf(fovY / 2.0f);
	GLfloat width = height/ aspect;
#else
	GLfloat height = aspect / tanf(fovY / 2.0f);
	GLfloat width = 1.0f / tanf(fovY / 2.0f);
#endif

	m[0][0] = width;
	m[1][0] = 0;
	m[2][0] = 0;
	m[3][0] = 0;

	m[0][1] = 0;
	m[1][1] = height;
	m[2][1] = 0;
	m[3][1] = 0;

	m[0][2] = 0;
	m[1][2] = 0;
#ifndef OPENGL_PROJECT
	m[2][2] = farPlane / (farPlane - nearPlane);
	m[3][2] = -nearPlane * farPlane / (farPlane - nearPlane);
#else
	m[2][2] = farPlane / (nearPlane - farPlane);
	m[3][2] = -nearPlane * farPlane / (farPlane - nearPlane);
#endif

	m[0][3] = 0;
	m[1][3] = 0;
#ifndef OPENGL_PROJECT
	m[2][3] = 1.f;
#else
	m[2][3] = -1.f;
#endif
	m[3][3] = 0;

	return *this;
}

Matrix & Matrix::LookAt(Vector3 pos, Vector3 target, Vector3 Up)
{
	Vector3 zaxis, xaxis, yaxis;
	zaxis = (pos - target).Normalize();
	xaxis = Vector3::Cross(Up, zaxis);
	yaxis = Vector3::Cross(zaxis, xaxis);

	m[0][0] = xaxis.x;
	m[1][0] = xaxis.y;
	m[2][0] = xaxis.z;
	m[3][0] = -(Vector3::Dot(xaxis, pos));

	m[0][1] = yaxis.x;
	m[1][1] = yaxis.y;
	m[2][1] = yaxis.z;
	m[3][1] = -(Vector3::Dot(yaxis, pos));

	m[0][2] = zaxis.x;
	m[1][2] = zaxis.y;
	m[2][2] = zaxis.z;
	m[3][2] = -(Vector3::Dot(zaxis, pos));

	m[0][3] = 0.0f;
	m[1][3] = 0.0f;
	m[2][3] = 0.0f;
	m[3][3] = 0.0f;

	return *this;
}

Matrix Matrix::Transpose()
{
	Matrix res;
	res.m[0][0] = m[0][0]; res.m[0][1] = m[1][0]; res.m[0][2] = m[2][0]; res.m[0][3] = m[3][0];
	res.m[1][0] = m[0][1]; res.m[1][1] = m[1][1]; res.m[1][2] = m[2][1]; res.m[1][3] = m[3][1];
	res.m[2][0] = m[0][2]; res.m[2][1] = m[1][2]; res.m[2][2] = m[2][2]; res.m[2][3] = m[3][2];
	res.m[3][0] = m[0][3]; res.m[3][1] = m[1][3]; res.m[3][2] = m[2][3]; res.m[3][3] = m[3][3];
	return res;
}
\

Matrix Matrix::operator + (Matrix & mat)
{
	Matrix res(*this);
	res += mat;
	return res;
}

Matrix & Matrix::operator += (Matrix & mat)
{
	m[0][0] += mat.m[0][0]; m[0][1] += mat.m[0][1]; m[0][2] += mat.m[0][2]; m[0][3] += mat.m[0][3];
	m[1][0] += mat.m[1][0]; m[1][1] += mat.m[1][1]; m[1][2] += mat.m[1][2]; m[1][3] += mat.m[1][3];
	m[2][0] += mat.m[2][0]; m[2][1] += mat.m[2][1]; m[2][2] += mat.m[2][2]; m[2][3] += mat.m[2][3];
	m[3][0] += mat.m[3][0]; m[3][1] += mat.m[3][1]; m[3][2] += mat.m[3][2]; m[3][3] += mat.m[3][3];

	return *this;
}

Matrix Matrix::operator - (Matrix & mat)
{
	Matrix res(*this);
	res -= mat;
	return res;
}

Matrix & Matrix::operator -= (Matrix & mat)
{
	m[0][0] -= mat.m[0][0]; m[0][1] -= mat.m[0][1]; m[0][2] -= mat.m[0][2]; m[0][3] -= mat.m[0][3];
	m[1][0] -= mat.m[1][0]; m[1][1] -= mat.m[1][1]; m[1][2] -= mat.m[1][2]; m[1][3] -= mat.m[1][3];
	m[2][0] -= mat.m[2][0]; m[2][1] -= mat.m[2][1]; m[2][2] -= mat.m[2][2]; m[2][3] -= mat.m[2][3];
	m[3][0] -= mat.m[3][0]; m[3][1] -= mat.m[3][1]; m[3][2] -= mat.m[3][2]; m[3][3] -= mat.m[3][3];

	return *this;
}

Matrix Matrix::operator * (Matrix & mat)
{
	Matrix res;
	res.m[0][0] = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0] + m[0][3] * mat.m[3][0];
	res.m[0][1] = m[0][0] * mat.m[0][1] + m[0][1] * mat.m[1][1] + m[0][2] * mat.m[2][1] + m[0][3] * mat.m[3][1];
	res.m[0][2] = m[0][0] * mat.m[0][2] + m[0][1] * mat.m[1][2] + m[0][2] * mat.m[2][2] + m[0][3] * mat.m[3][2];
	res.m[0][3] = m[0][0] * mat.m[0][3] + m[0][1] * mat.m[1][3] + m[0][2] * mat.m[2][3] + m[0][3] * mat.m[3][3];

	res.m[1][0] = m[1][0] * mat.m[0][0] + m[1][1] * mat.m[1][0] + m[1][2] * mat.m[2][0] + m[1][3] * mat.m[3][0];
	res.m[1][1] = m[1][0] * mat.m[0][1] + m[1][1] * mat.m[1][1] + m[1][2] * mat.m[2][1] + m[1][3] * mat.m[3][1];
	res.m[1][2] = m[1][0] * mat.m[0][2] + m[1][1] * mat.m[1][2] + m[1][2] * mat.m[2][2] + m[1][3] * mat.m[3][2];
	res.m[1][3] = m[1][0] * mat.m[0][3] + m[1][1] * mat.m[1][3] + m[1][2] * mat.m[2][3] + m[1][3] * mat.m[3][3];

	res.m[2][0] = m[2][0] * mat.m[0][0] + m[2][1] * mat.m[1][0] + m[2][2] * mat.m[2][0] + m[2][3] * mat.m[3][0];
	res.m[2][1] = m[2][0] * mat.m[0][1] + m[2][1] * mat.m[1][1] + m[2][2] * mat.m[2][1] + m[2][3] * mat.m[3][1];
	res.m[2][2] = m[2][0] * mat.m[0][2] + m[2][1] * mat.m[1][2] + m[2][2] * mat.m[2][2] + m[2][3] * mat.m[3][2];
	res.m[2][3] = m[2][0] * mat.m[0][3] + m[2][1] * mat.m[1][3] + m[2][2] * mat.m[2][3] + m[2][3] * mat.m[3][3];

	res.m[3][0] = m[3][0] * mat.m[0][0] + m[3][1] * mat.m[1][0] + m[3][2] * mat.m[2][0] + m[3][3] * mat.m[3][0];
	res.m[3][1] = m[3][0] * mat.m[0][1] + m[3][1] * mat.m[1][1] + m[3][2] * mat.m[2][1] + m[3][3] * mat.m[3][1];
	res.m[3][2] = m[3][0] * mat.m[0][2] + m[3][1] * mat.m[1][2] + m[3][2] * mat.m[2][2] + m[3][3] * mat.m[3][2];
	res.m[3][3] = m[3][0] * mat.m[0][3] + m[3][1] * mat.m[1][3] + m[3][2] * mat.m[2][3] + m[3][3] * mat.m[3][3];

	return res;
}

Matrix Matrix::operator * (GLfloat k)
{
	Matrix mat(*this);
	mat *= k;
	return mat;
}

Matrix & Matrix::operator *= (GLfloat k)
{
	m[0][0] *= k; m[0][1] *= k; m[0][2] *= k; m[0][3] *= k;
	m[1][0] *= k; m[1][1] *= k; m[1][2] *= k; m[1][3] *= k;
	m[2][0] *= k; m[2][1] *= k; m[2][2] *= k; m[2][3] *= k;
	m[3][0] *= k; m[3][1] *= k; m[3][2] *= k; m[3][3] *= k;

	return *this;
}


Vector4 Matrix::operator * (Vector4 & vec)
{
	Vector4 res;
	res.x = vec.x * m[0][0] + vec.y * m[0][1] + vec.z * m[0][2] + vec.w * m[0][3];
	res.y = vec.x * m[1][0] + vec.y * m[1][1] + vec.z * m[1][2] + vec.w * m[1][3];
	res.z = vec.x * m[2][0] + vec.y * m[2][1] + vec.z * m[2][2] + vec.w * m[2][3];
	res.w = vec.x * m[3][0] + vec.y * m[3][1] + vec.z * m[3][2] + vec.w * m[3][3];

	return res;
}

Matrix & Matrix::operator = (Matrix & mat)
{
	m[0][0] = mat.m[0][0]; m[0][1] = mat.m[0][1]; m[0][2] = mat.m[0][2]; m[0][3] = mat.m[0][3];
	m[1][0] = mat.m[1][0]; m[1][1] = mat.m[1][1]; m[1][2] = mat.m[1][2]; m[1][3] = mat.m[1][3];
	m[2][0] = mat.m[2][0]; m[2][1] = mat.m[2][1]; m[2][2] = mat.m[2][2]; m[2][3] = mat.m[2][3];
	m[3][0] = mat.m[3][0]; m[3][1] = mat.m[3][1]; m[3][2] = mat.m[3][2]; m[3][3] = mat.m[3][3];

	return *this;
}

//Line

GLfloat Line::DistanceToPoint(Vector3& P)
{
	GLfloat t0 = d.Dot(P - p0) / d.Dot(d);
	GLfloat distanceLine = Vector3::Dist(P, p0 + d * t0);
	return distanceLine;
}
bool Line::ClosestPoint(Line& L1, Line& L2, Vector3& Result1, Vector3& Result2)
{
	Vector3 newDir = L1.d.Cross(L2.d);
	float length = newDir.Length();
	if (length == 0.0f)
		return false;
	Plane P1 = Plane::ConstructFromPointVectors(L1.p0, newDir, L1.d);
	Plane P2 = Plane::ConstructFromPointVectors(L2.p0, newDir, L2.d);

	Result1 = P2.IntersectLine(L1);
	Result2 = P1.IntersectLine(L2);
	return true;
}
GLfloat Line::Distance(Line& L1, Line& L2)
{
	Vector3 Cross = Vector3::Cross(L1.d, L2.d);
	return fabsf(Vector3::Dot(L2.p0 - L1.p0, Cross)) / Cross.Length();
}
GLfloat Line::DistanceSq(Line& L1, Line& L2)
{
	Vector3 Cross = Vector3::Cross(L1.d, L2.d);
	GLfloat Dot = Vector3::Dot(L2.p0 - L1.p0, Cross);
	return(Dot*Dot);
}

//Plane
Plane::Plane()
{

}

Plane::Plane(const float* arg)
{
	a = arg[0];
	b = arg[1];
	c = arg[2];
	d = arg[3];
}

Plane::Plane(Vector4 vec)
{
	a = vec.x;
	b = vec.y;
	c = vec.z;
	d = vec.w;
}
Plane::Plane(GLfloat _a, GLfloat _b, GLfloat _c, GLfloat _d)
{
	a = _a;
	b = _b;
	c = _c;
	d = _d;
}

bool Plane::operator == (const Plane& p) const
{
	return (a == p.a) && (b == p.b) && (c == p.c) && (d == p.d);
}

bool Plane::operator != (const Plane& p) const
{
	return (a != p.a) || (b != p.b) || (c != p.c) || (d != p.c);
}
Plane operator *(GLfloat k, const Plane& p)
{
	Plane Result;
	Result.a = p.a*k;
	Result.b = p.b*k;
	Result.c = p.c*k;
	Result.d = p.d*k;
	return Result;
}

Plane Plane::operator * (float k) const
{
	Plane Result;
	Result.a = a*k;
	Result.b = b*k;
	Result.c = c*k;
	Result.d = d*k;
	return Result;
}
Plane Plane::operator / (float k) const
{
	Plane Result;
	Result.a = a/k;
	Result.b = b/k;
	Result.c = c/k;
	Result.d = d/k;
	return Result;
}

Plane& Plane::operator *= (float k) 
{
	a *= k;
	b *= k;
	c *= k;
	d *= k;
	return *this;
}
Plane& Plane::operator /= (float k) 
{
	a /= k;
	b /= k;
	c /= k;
	d /= k;
	return *this;
}

Plane::operator const GLfloat*() const
{
	GLfloat res[4];
	res[0] = a; res[1] = b; res[2] = c; res[3] = d;
	return res;
}
Plane::operator GLfloat*()
{
	GLfloat res[4];
	res[0] = a; res[1] = b; res[2] = c; res[3] = d;
	return res;
}

Plane::Plane(Vector3& Normal, GLfloat _d)
{
	Vector3 NormalizedNormal = Normal.Normalize();
	a = NormalizedNormal.x;
	b = NormalizedNormal.y;
	c = NormalizedNormal.z;
	d = _d;
}
Plane Plane::ConstructFromPointNormal(Vector3& point, Vector3& Normal)
{
	Plane Result;
	Vector3 NormalizedNormal = Normal.Normalize();
	Result.a = NormalizedNormal.x;
	Result.b = NormalizedNormal.y;
	Result.c = NormalizedNormal.z;
	Result.d = point.Dot(NormalizedNormal);
	return Result;
}

Plane Plane::ConstructFromPointVectors(Vector3& point, Vector3& v1, Vector3& v2)
{
	Vector3 Normal = v1.Cross(v2);
	return ConstructFromPointNormal(point, Normal);
}
Plane Plane::ConstructFromPoints(Vector3& p0, Vector3& p1, Vector3& p2)
{
	Vector3 Normal = Vector3::Cross(p1-p0,p2-p0);

	return ConstructFromPointNormal(p0, Normal);
}

Plane Plane::Normalize()
{
	Plane Result;
	GLfloat Distance = sqrtf(a*a + b*b + c*c);
	Result.a = a / Distance;
	Result.b = b / Distance;
	Result.c = c / Distance;
	Result.d = d / Distance;
	return Result;
}

GLfloat Plane::UnsignedDistance(Vector3& Point)
{
	return fabsf(a* Point.x + b*Point.y + c*Point.z + d);
}
GLfloat Plane::SignedDistance(Vector3& Point)
{
	return (a* Point.x + b*Point.y + c*Point.z + d);
}
Vector3 Plane::ClosestPoint(Vector3& Point) 
{
	return (Point - Normal() * SignedDistance(Point));
}

Vector3 Plane::IntersectLine(Vector3& V1, Vector3& V2, bool& Hit)
{
	Hit = true;
	Vector3 Diff = V1 - V2;
	GLfloat Denominator = a * Diff.x + b * Diff.y + c * Diff.z;
	if (Denominator == 0) { Hit = false; return V1; }
	GLfloat u = (a * V1.x + b*V1.y + c*V1.z + d) / Denominator;
	return (V1 + (V2 - V1) * u);
}
Vector3 Plane::IntersectLine(Vector3& V1, Vector3& V2)
{
	Vector3 Diff = V1 - V2;
	GLfloat Denominator = a * Diff.x + b * Diff.y + c * Diff.z;
	if (Denominator == 0.0f)
		return (V1 + V2) * 0.5f;
	GLfloat u = (a * V1.x + b*V1.y + c*V1.z + d) / Denominator;
	return (V1 + (V2 - V1) * u);
}
Vector3 Plane::IntersectLine(Line& L)
{
	return IntersectLine(L.p0, L.d);
}
GLfloat Plane::IntersectLineRatio(Vector3& V1, Vector3& V2) 
{
	Vector3 Diff = V1 - V2;
	GLfloat Denominator = a * Diff.x + b * Diff.y + c * Diff.z;
	if (Denominator == 0.0f)
		return 0.0f;
	return (a * V1.x + b*V1.y + c*V1.z + d) / -Denominator;
}

bool Plane::PlanePlaneIntersection(Plane& P1, Plane& P2, Line& L)
{
	GLfloat Denominator = P1.a * P2.b - P1.b * P2.a;
	if (Denominator == 0.0f)
		return false;

	L.p0 = Vector3((P2.d*P1.b - P1.b * P2.b) / Denominator, (P1.d*P2.a - P2.d*P1.a) / Denominator, 0.0f);
	L.d - Vector3::Cross(P1.Normal(), P2.Normal());
	if (L.d.Length() == 0.0f)
		return false;
	L.d = L.d.Normalize();
	return true;
}

GLfloat Plane::Dot(Plane& P, Vector4& V)
{
	return P.a*V.x + P.b*V.y + P.c*V.z + P.d*V.w;
}
GLfloat Plane::DotCoord(Plane& P , Vector3& V)
{
	GLfloat res = P.a*V.x + P.b*V.y + P.c*V.z + P.d;
	return res;
}
GLfloat Plane::DotNormal(Plane& P, Vector3& V)
{
	GLfloat res = P.a*V.x + P.b*V.y + P.c*V.z;
	return res;
}