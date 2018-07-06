#pragma once
#include "MathUtils.h"
#ifdef OPENGL_PROJECT
#include "OpenGLClass.h"
#else
class ID3D11DeviceContext;
#endif
namespace MoebiusEngine
{
	class Light;
	class Object;
	class Camera;
	class Frustum;
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
		
#ifdef OPENGL_PROJECT
		bool Init(char*, OpenGLClass*);
		void Draw(OpenGLClass* openGL);
#else
		bool Init(char* FileName);
		void Draw(ID3D11DeviceContext* devcon);
#endif
		void Update(float deltaTime);
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
		Object** m_Objects;
		Camera* m_camera;
		Frustum* m_frustum;
		float m_time;
		Vector3 m_ambientColor;
		float m_ambientWeight;
		
	};
}

