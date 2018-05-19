#pragma once
class Light;
class Object;
class Camera;
class ID3D11DeviceContext;
#include "MathUtil.h"
class SceneManager
{
protected:
	SceneManager();
	~SceneManager();
	SceneManager(const SceneManager &);
	SceneManager& operator =(const SceneManager &) {}
public:
	static SceneManager* GetInstance();
	static void DestroyInstance();
	bool Init(char* FileName);
	void Draw(ID3D11DeviceContext* devcon);
	void Update(float deltaTime, int pressedKey);
	void Clean();
	float GetTime();
	Vector3 GetAmbientColor();
	float GetAmbientWeight();
	Light* GetLightById(unsigned int Id);
protected:
	static SceneManager* m_pInstance;
	int m_LightCount;
	Light* m_Lights;
	int m_ObjectCount;
	Object* m_Objects;
	Camera* m_camera;
	float m_time;
	Vector3 m_ambientColor;
	float m_ambientWeight;
};

