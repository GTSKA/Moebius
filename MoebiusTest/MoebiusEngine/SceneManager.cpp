#include "SceneManager.h"
#include "Camera.h"
#include "Globals.h"
#include "Object.h"
#include "Light.h"
#include "Frustum.h"
#include <d3d11.h>
#include <stdio.h>
#include <tchar.h>
#include "inputManager.h"

#define max3(a,b,c) ((a)>(b)) ? (((a)>(c))? (a) : (c)) : (((b)>(c))? (b) : (c))

namespace MoebiusEngine
{
	SceneManager::SceneManager()
	{
		m_pInstance = NULL;
		m_camera = NULL;
		m_Objects = NULL;
		m_Lights = NULL;
		m_frustum = NULL;
		m_time = 0;
	}
	SceneManager::SceneManager(const SceneManager &)
	{
		m_pInstance = NULL;
		m_camera = NULL;
		m_Objects = NULL;
		m_Lights = NULL;
		m_frustum = NULL;
		m_time = 0;
	}

	SceneManager::~SceneManager()
	{
		Clean();
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
#ifdef OPENGL_PROJECT
	bool SceneManager::Init(char* FileName, OpenGLClass* openGL)
#else
	bool SceneManager::Init(char* FileName)
#endif
	{
		if (FileName == NULL)
		{
			return false;
		}
		FILE* SMFile;
		fopen_s(&SMFile, FileName, "r");
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
				if (m_camera == NULL)
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
			if (strcmp(remark, "#LIGHTS:") == 0)
			{
				fscanf_s(SMFile, "%d", &m_LightCount);
				fscanf_s(SMFile, "%*s %f, %f, %f", &m_ambientColor.x, &m_ambientColor.y, &m_ambientColor.z);
				fscanf_s(SMFile, "%*s %f", &m_ambientWeight);
				m_Lights = new Light[m_LightCount];
				for (int i = 0; i < m_LightCount; ++i)
				{
					int id;
					LightType lType;
					Vector3 dir_pos;
					Vector3 color;
					char type[25];
					fscanf_s(SMFile, "%*s %d", &id);
					fscanf_s(SMFile, "%*s %f, %f, %f", &dir_pos.x, &dir_pos.y, &dir_pos.z);
					fscanf_s(SMFile, "%*s %s", type, _countof(type));
					if (strcmp(type, "POINT") == 0)
					{
						lType = LIGHT_POINT;
					}
					if (strcmp(type, "DIRECTIONAL") == 0)
					{
						lType = LIGHT_DIRECTIONAL;
					}
					fscanf_s(SMFile, "%*s %f, %f, %f", &color.x, &color.y, &color.z);
					MovingAttributes ma;
					ma.moving = false;
					if (strcmp(type, "POINT") == 0)
					{
						char moving[50];
						fscanf_s(SMFile, "%s", moving, _countof(moving));
						if (strcmp(moving, "MOVING") == 0)
						{
							ma.moving = true;
						}
						char shape[50];
						fscanf_s(SMFile, "%s", shape, _countof(shape));
						if (strcmp(shape, "CIRCLE") == 0)
						{
							ma.type = LIGHTMOVE_CIRCLE;
						}
						fscanf_s(SMFile, "%*s %f", &ma.radius);
						fscanf_s(SMFile, "%*s %f", &ma.speed);
					}
					m_Lights[i].Init(id, lType, dir_pos, color, &ma);
				}
			}
			if (strcmp(remark, "#Objects:") == 0)
			{
				fscanf_s(SMFile, "%d", &m_ObjectCount);
				m_Objects = new Object*[m_ObjectCount];
				int frustum;
				for (int i = 0; i < m_ObjectCount; ++i)
				{
					unsigned int id;
					fscanf_s(SMFile, "%*s %u", &id);
					//char objectclass[50];
					//fscanf_s(SMFile, "%s", objectclass, _countof(objectclass));
					//if (strcmp(objectclass, "OBJECT") == 0)
					//{
						m_Objects[i] = new Object;
					//}
					/*else if (strcmp(objectclass, "WATERWAVES") == 0)
					{
						m_Objects[i] = new WaterWaves;
					}*/

					unsigned int modelId;
					fscanf_s(SMFile, "%*s %d", &frustum);
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
					for (int j = 0; j < Textures2D.m_textureCount; ++j)
					{
						fscanf_s(SMFile, "%*s %u", &Textures2D.m_textureIDs[j]);
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
					for (int j = 0; j < cubicTextures.m_textureCount; ++j)
					{
						fscanf_s(SMFile, "%*s %u", &cubicTextures.m_textureIDs[j]);
					}
					unsigned int shaderId;
					fscanf_s(SMFile, "%*s %u", &shaderId);

					int LightCount;
					fscanf_s(SMFile, "%*s %d", &LightCount);
					if (LightCount)
					{
						unsigned int *Lights;
						float specPow = 1.0f;
						unsigned int TextureId;
						Lights = new unsigned int[LightCount];
						for (int k = 0; k < LightCount; ++k)
						{
							fscanf_s(SMFile, "%*s %u", &Lights[k]);
						}
						int sParametersCount;
						fscanf_s(SMFile, "%*s %d", &sParametersCount);
						for (int k = 0; k < sParametersCount; ++k)
						{
							char pType[25];
							fscanf_s(SMFile, "%s", pType, _countof(pType));
							if (strcmp(pType, "SPECULAR_POWER") == 0)
							{
								fscanf_s(SMFile, "%f", &specPow);
							}
							if (strcmp(pType, "CTEXTURE") == 0)
							{
								fscanf_s(SMFile, "%u", &TextureId);
							}
						}
						m_Objects[i]->InitLights(LightCount, Lights, specPow);
						delete[] Lights;
					}
					Vector3 position, rotation, scale;
					fscanf_s(SMFile, "%*s %f, %f, %f", &position.x, &position.y, &position.z);
					fscanf_s(SMFile, "%*s %f, %f, %f", &rotation.x, &rotation.y, &rotation.z);
					fscanf_s(SMFile, "%*s %f, %f, %f", &scale.x, &scale.y, &scale.z);

					if (!m_Objects[i]->Init(id, modelId, &Textures2D, &cubicTextures, shaderId, &position, &rotation, &scale))///Init Object
					{
						fclose(SMFile);
						return false;
					}
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
					float tilingFactor;
					fscanf_s(SMFile, "%*s %f", &tilingFactor);
					m_Objects[i]->InitTilingFactor(tilingFactor);
					m_Objects[i]->setFrustum(frustum);
#ifdef OPENGL_PROJECT
					m_Objects[i]->InitVAO(openGL);
#endif
					//fscanf(SMFile, "%*s");//ignore next line(empty line between objects)
				}
			}
		}
		fclose(SMFile);
		m_time = 0;
		m_frustum = new Frustum;
		if (!m_frustum)
		{
			OutputDebugStringA("Error while creating frustum\n");
			return false;
		}
		return true;
	}

	void SceneManager::Update(float deltaTime)
	{
		m_time += deltaTime;
		for (int i = 0; i < m_LightCount; ++i)
		{
			m_Lights[i].Update(deltaTime);
		}
		for (int i = 0; i < m_ObjectCount; ++i)
		{
			m_Objects[i]->Update(deltaTime);
		}
		if (InputManager::getInstance()->isKeyDown(Globals::KEY_W))
			m_camera->moveForward(deltaTime);
		if (InputManager::getInstance()->isKeyDown(Globals::KEY_A))
			m_camera->moveLeft(deltaTime);
		if (InputManager::getInstance()->isKeyDown(Globals::KEY_S))
			m_camera->moveBackward(deltaTime);
		if (InputManager::getInstance()->isKeyDown(Globals::KEY_D))
			m_camera->moveRight(deltaTime);

		if (InputManager::getInstance()->isKeyDown(Globals::KEY_LEFT))
			m_camera->rotateLeft(deltaTime);
		if (InputManager::getInstance()->isKeyDown(Globals::KEY_UP))
			m_camera->rotateUp(deltaTime);
		if (InputManager::getInstance()->isKeyDown(Globals::KEY_RIGHT))
			m_camera->rotateRight(deltaTime);
		if (InputManager::getInstance()->isKeyDown(Globals::KEY_DOWN))
			m_camera->rotateDown(deltaTime);
	}
#ifdef OPENGL_PROJECT
	void SceneManager::Draw(OpenGLClass* openGL)
#else
	void SceneManager::Draw(ID3D11DeviceContext* devcon)
#endif
	{
		bool renderObject;
		m_frustum->constructFrustum(Globals::SCREEN_DEPTH, m_camera->getProjectionMatrix(), m_camera->getViewMatrix());
		float radius;
		for (int i = 0; i < m_ObjectCount; ++i)
		{
			if (m_Objects[i]->shouldCalcFrustum())
			{
				radius = max3(m_Objects[i]->getSize().x, m_Objects[i]->getSize().y, m_Objects[i]->getSize().z);
				if (true)
				{
#ifdef OPENGL_PROJECT
					m_Objects[i]->Draw(m_camera, openGL);
#else
					m_Objects[i]->Draw(m_camera, devcon);
#endif
				}
			}
			else
#ifdef OPENGL_PROJECT
				m_Objects[i]->Draw(m_camera, openGL);
#else
				m_Objects[i]->Draw(m_camera, devcon);
#endif

		}
	}


	void SceneManager::Clean()
	{
		if (m_camera)
		{
			delete m_camera;
			m_camera = NULL;
		}
		if (m_Objects)
		{
			for (int i = 0; i < m_ObjectCount; ++i)
				delete m_Objects[i];
			delete[] m_Objects;
			m_Objects = NULL;
		}
		if (m_Lights)
		{
			delete[] m_Lights;
			m_Lights = NULL;
		}
		if (m_frustum)
		{
			delete m_frustum;
			m_frustum = NULL;
		}

	}
	float SceneManager::GetTime()
	{
		return m_time;
	}
	Vector3 SceneManager::GetAmbientColor()
	{
		return m_ambientColor;
	}
	float SceneManager::GetAmbientWeight()
	{
		return m_ambientWeight;
	}
	Light* SceneManager::GetLightById(unsigned int Id)
	{
		for (int i = 0; i < m_LightCount; ++i)
		{
			if (m_Lights[i].GetId() == Id)
			{
				return &m_Lights[i];
			}
		}
		return NULL;
	}
}
