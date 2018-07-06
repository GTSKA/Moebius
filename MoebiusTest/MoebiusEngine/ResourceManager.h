#pragma once
#ifdef OPENGL_PROJECT
#include "OpenGLClass.h"
#else
#include <d3d11.h>
#endif

namespace MoebiusEngine
{
	class Model;
	class Texture;
	class Shader;
	class ResourceManager
	{
	protected:
		ResourceManager();
		~ResourceManager();

		ResourceManager(const ResourceManager &);
		ResourceManager& operator=(const ResourceManager &) {}
	public:
		static ResourceManager* GetInstance();
		static void DestroyInstance();

#ifdef OPENGL_PROJECT
		bool Init(char* fileName, OpenGLClass* openGL);
#else
		bool Init(char* fileName, ID3D11Device* dev, ID3D11DeviceContext* devcon);
#endif
		Model* GetModelById(unsigned int id);
		Texture* GetTextureById(unsigned int id);
		Texture* GetCubeTextureById(unsigned int id);
		Shader* GetShaderById(unsigned int id);
		void Clean();


	protected:
		static ResourceManager* m_pInstance;
		int m_modelCount;
		Model* m_Models;
		int m_textureCount;
		Texture* m_Textures;
		int m_cubeTextureCount;
		Texture* m_cubeTextures;
		int m_shaderCount;
		Shader* m_Shaders;
		//int m_statesCount;
		//unsigned int *m_states;
	};
}

