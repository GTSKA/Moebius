#pragma once
class Lights;
class Object;
class Camera;
class ID3D11DeviceContext;
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
protected:
	static SceneManager* m_pInstance;
	int m_LightCount;
	Lights* m_Lights;
	int m_ObjectCount;
	Object* m_Objects;
	Camera* m_camera;
	float m_time;
};

