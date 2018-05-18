#include "SceneManager.h"
#include "Camera.h"
#include "Globals.h"
#include "Object.h"
#include <d3d11.h>
#include <stdio.h>
#include <tchar.h>


SceneManager::SceneManager()
{
	m_pInstance = NULL;
	m_camera = NULL;
	m_Objects = NULL;
	m_Lights = NULL;
	m_time = 0;
}
SceneManager::SceneManager(const SceneManager &)
{
	m_pInstance = NULL;
	m_camera = NULL;
	m_Objects = NULL;
	m_Lights = NULL;
}

SceneManager::~SceneManager()
{
}

SceneManager* SceneManager::m_pInstance = NULL;

SceneManager* SceneManager::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new SceneManager;
	}
	return m_pInstance;
}


void SceneManager::DestroyInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

bool SceneManager::Init(char* FileName)
{
	if (FileName == NULL)
	{
		return false;
	}
	FILE* SMFile;
	fopen_s(&SMFile,FileName, "r");
	if (SMFile == NULL)
	{
		return false;
	}
	char remark[50];
	while (!feof(SMFile))
	{
		strcpy_s(remark, "");
		fscanf_s(SMFile, "%s", remark, _countof(remark));
		if (strcmp(remark, "#CAMERA") == 0)
		{
			if(m_camera == NULL)
				m_camera = new Camera;

			float l_near, l_far, fov, speed;
			fscanf_s(SMFile, "%*s %f", &l_near);
			fscanf_s(SMFile, "%*s %f", &l_far);
			fscanf_s(SMFile, "%*s %f", &fov);
			fscanf_s(SMFile, "%*s %f", &speed);
			m_camera->Init(fov, l_near, l_far, speed);
			m_camera->calcWorldMatrix();
			m_camera->calcViewMatrix();
		}
		if (strcmp(remark, "#FOG") == 0)
		{
			if (m_camera == NULL)
				m_camera = new Camera;
			Vector3 fogColor;
			float fogStart, fogRange;
			fscanf_s(SMFile, "%*s %f, %f, %f", &fogColor.x, &fogColor.y, &fogColor.z);
			fscanf_s(SMFile, "%*s %f", &fogStart);
			fscanf_s(SMFile, "%*s %f", &fogRange);
			m_camera->initFog(fogColor, fogStart, fogRange);
		}
		if (strcmp(remark, "#Objects:") == 0)
		{
			fscanf_s(SMFile, "%d", &m_ObjectCount);
			m_Objects = new Object[m_ObjectCount];
			for (int i = 0; i < m_ObjectCount; ++i)
			{
				unsigned int id;
				fscanf_s(SMFile, "%*s %u", &id);
				unsigned int modelId;
				fscanf_s(SMFile, "%*s %u", &modelId);
				TextureInit Textures2D;
				fscanf_s(SMFile, "%*s %d", &Textures2D.m_textureCount);
				if (Textures2D.m_textureCount > 0)
				{
					Textures2D.m_textureIDs = new unsigned int[Textures2D.m_textureCount];
				}
				else
				{
					Textures2D.m_textureIDs = NULL;//don't reserve memory when not needed
				}
				for (int i = 0; i < Textures2D.m_textureCount; ++i)
				{
					fscanf_s(SMFile, "%*s %u", &Textures2D.m_textureIDs[i]);
				}
				TextureInit cubicTextures;
				fscanf_s(SMFile, "%*s %d", &cubicTextures.m_textureCount);
				if (cubicTextures.m_textureCount > 0)
				{
					cubicTextures.m_textureIDs = new unsigned int[cubicTextures.m_textureCount];
				}
				else
				{
					cubicTextures.m_textureIDs = NULL;//don't reserve memory when not needed
				}
				for (int i = 0; i < cubicTextures.m_textureCount; ++i)
				{
					fscanf_s(SMFile, "%*s %u", &cubicTextures.m_textureIDs[i]);
				}
				unsigned int shaderId;
				fscanf_s(SMFile, "%*s %u", &shaderId);

				Vector3 position, rotation, scale;
				fscanf_s(SMFile, "%*s %f, %f, %f", &position.x, &position.y, &position.z);
				fscanf_s(SMFile, "%*s %f, %f, %f", &rotation.x, &rotation.y, &rotation.z);
				fscanf_s(SMFile, "%*s %f, %f, %f", &scale.x, &scale.y, &scale.z);

				if (!m_Objects[i].Init(id, modelId, &Textures2D, &cubicTextures, shaderId, &position, &rotation, &scale))///Init Object
					return false;

																													////free memory
				if (Textures2D.m_textureIDs)
				{
					delete Textures2D.m_textureIDs;
					Textures2D.m_textureIDs = NULL;
				}

				if (cubicTextures.m_textureIDs)
				{
					delete cubicTextures.m_textureIDs;
					cubicTextures.m_textureIDs = NULL;
				}
				//fscanf(SMFile, "%*s");//ignore next line(empty line between objects)
			}
		}
	}
	m_time = 0;
	return true;
}

void SceneManager::Update(float deltaTime, int pressedKey)
{
	m_time += deltaTime;
	if (pressedKey&(Globals::_KEY_A))
	{
		m_camera->moveLeft(deltaTime);
	}
	if (pressedKey&(Globals::_KEY_W))
	{
		m_camera->moveForward(deltaTime);
	}
	if (pressedKey&(Globals::_KEY_D))
	{
		m_camera->moveRight(deltaTime);
	}
	if (pressedKey&(Globals::_KEY_S))
	{
		m_camera->moveBackward(deltaTime);
	}
	if (pressedKey&(Globals::_KEY_LEFT))
	{
		m_camera->rotateLeft(deltaTime);
	}
	if (pressedKey&(Globals::_KEY_UP))
	{
		m_camera->rotateUp(deltaTime);
	}
	if (pressedKey&(Globals::_KEY_RIGHT))
	{
		m_camera->rotateRight(deltaTime);
	}
	if (pressedKey&(Globals::_KEY_DOWN))
	{
		m_camera->rotateDown(deltaTime);
	}
}

void SceneManager::Draw(ID3D11DeviceContext* devcon)
{
	for (int i = 0; i < m_ObjectCount; ++i)
	{
		m_Objects[i].Draw(m_camera, devcon);
	}
}

void SceneManager::Clean()
{
	if (m_camera)
	{
		delete m_camera;
		m_camera = NULL;
	}
}
float SceneManager::GetTime()
{
	return m_time;
}