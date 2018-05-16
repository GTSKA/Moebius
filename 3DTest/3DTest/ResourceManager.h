#pragma once
#include <d3d11.h>
class Model;
class Texture;
class Shaders;
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
	
	bool Init(char* fileName, ID3D11Device* dev, ID3D11DeviceContext* devcon);
	Model* GetModelById(unsigned int id);
	Texture* GetTextureById(unsigned int id);
	Texture* GetCubeTextureById(unsigned int id);
	Shaders* GetShaderById(unsigned int id);
	void EnableStates();
	void DisableStates();
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
	Shaders* m_Shaders;
	int m_statesCount;
	unsigned int *m_states;
};

