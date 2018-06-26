#pragma once
#include "MathUtil.h"

class Texture;
class Model;
class Shaders;
class Camera;
class ID3D11DeviceContext;
struct TextureInit
{
	int m_textureCount;
	unsigned int* m_textureIDs;
};
class Object
{
public:
	Object();
	~Object();
	virtual void Update(float deltaTime, int pressedKey);
	void Draw(Camera* cam, ID3D11DeviceContext* devcon);
	virtual bool Init(unsigned int ID, unsigned modelID, TextureInit* textures2D, TextureInit* cubicTextures, unsigned int shaderID,
		Vector3* position, Vector3* rotation, Vector3* scale);
	void Clean();
	void InitLights(int LightCount, unsigned int* Lights, float specularPower = 1.0f);
	void InitTilingFactor(float TilingFactor);
protected:
	int m_TextureCount;
	Texture** m_Textures;
	int m_CubeTexturesCount;
	Texture** m_CubeTextures;
	Model* m_model;
	Shaders* m_Shader;
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;
	Matrix m_worldMatrix;
	unsigned int m_ID;
	void CalcWorldMatrix();
	int m_LightCount;
	unsigned int* m_Lights;
	float m_SpecularPower;
	float m_tilingFactor;
	float DepthAdjustment;
};

