#include "ResourceManager.h"
#include "Model.h"
#include "Plane.h"
#include "Texture.h"
#include "Shader.h"
#include <stdio.h>
#include <tchar.h>
#ifndef _countof
#define _countof(a) (sizeof(a)/sizeof(*(a))) 
#endif
/* EnableCap */
#define GL_TEXTURE_2D                     0x0DE1
#define GL_CULL_FACE                      0x0B44
#define GL_BLEND                          0x0BE2
#define GL_DITHER                         0x0BD0
#define GL_STENCIL_TEST                   0x0B90
#define GL_DEPTH_TEST                     0x0B71
#define GL_SCISSOR_TEST                   0x0C11
#define GL_POLYGON_OFFSET_FILL            0x8037
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_COVERAGE                0x80A0

/* ErrorCode */
#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_OUT_OF_MEMORY                  0x0505

/* FrontFaceDirection */
#define GL_CW                             0x0900
#define GL_CCW                            0x0901

/* GetPName */
#define GL_LINE_WIDTH                     0x0B21
#define GL_ALIASED_POINT_SIZE_RANGE       0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE       0x846E
#define GL_CULL_FACE_MODE                 0x0B45
#define GL_FRONT_FACE                     0x0B46
#define GL_DEPTH_RANGE                    0x0B70
#define GL_DEPTH_WRITEMASK                0x0B72
#define GL_DEPTH_CLEAR_VALUE              0x0B73
#define GL_DEPTH_FUNC                     0x0B74
#define GL_STENCIL_CLEAR_VALUE            0x0B91
#define GL_STENCIL_FUNC                   0x0B92
#define GL_STENCIL_FAIL                   0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS        0x0B96
#define GL_STENCIL_REF                    0x0B97
#define GL_STENCIL_VALUE_MASK             0x0B93
#define GL_STENCIL_WRITEMASK              0x0B98
#define GL_STENCIL_BACK_FUNC              0x8800
#define GL_STENCIL_BACK_FAIL              0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL   0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS   0x8803
#define GL_STENCIL_BACK_REF               0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK        0x8CA4
#define GL_STENCIL_BACK_WRITEMASK         0x8CA5
#define GL_VIEWPORT                       0x0BA2
#define GL_SCISSOR_BOX                    0x0C10
/*      GL_SCISSOR_TEST */
#define GL_COLOR_CLEAR_VALUE              0x0C22
#define GL_COLOR_WRITEMASK                0x0C23
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_MAX_TEXTURE_SIZE               0x0D33
#define GL_MAX_VIEWPORT_DIMS              0x0D3A
#define GL_SUBPIXEL_BITS                  0x0D50
#define GL_RED_BITS                       0x0D52
#define GL_GREEN_BITS                     0x0D53
#define GL_BLUE_BITS                      0x0D54
#define GL_ALPHA_BITS                     0x0D55
#define GL_DEPTH_BITS                     0x0D56
#define GL_STENCIL_BITS                   0x0D57
#define GL_POLYGON_OFFSET_UNITS           0x2A00
/*      GL_POLYGON_OFFSET_FILL */
#define GL_POLYGON_OFFSET_FACTOR          0x8038
#define GL_TEXTURE_BINDING_2D             0x8069
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLE_COVERAGE_VALUE          0x80AA
#define GL_SAMPLE_COVERAGE_INVERT         0x80AB



namespace MoebiusEngine
{
	ResourceManager* ResourceManager::m_pInstance = NULL;

	ResourceManager::ResourceManager()
	{
		m_pInstance = NULL;
		m_Models = NULL;
		m_Textures = NULL;
		m_cubeTextures = NULL;
		m_Shaders = NULL;
	}


	ResourceManager::~ResourceManager()
	{
	}

	ResourceManager* ResourceManager::GetInstance()
	{
		if (m_pInstance == NULL)
			m_pInstance = new ResourceManager;
		return m_pInstance;
	}
	void ResourceManager::DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
	ResourceManager::ResourceManager(const ResourceManager &)
	{
		m_pInstance = NULL;
		m_Models = NULL;
		m_Textures = NULL;
		m_cubeTextures = NULL;
		m_Shaders = NULL;
	}

#ifdef OPENGL_PROJECT
	bool ResourceManager::Init(char* fileName, OpenGLClass* openGL)
#else
	bool ResourceManager::Init(char* fileName, ID3D11Device* dev, ID3D11DeviceContext* devcon)
#endif
	{
		if (fileName == NULL)
			return false;
		FILE* RMFile;
		fopen_s(&RMFile, fileName, "r");
		if (RMFile == NULL)
			return false;
		char remark[50];
		while (!feof(RMFile))
		{
			strcpy_s(remark, "");
			fscanf_s(RMFile, "%s", remark, _countof(remark));
			if (strcmp(remark, "#Models:") == 0)
			{
				fscanf_s(RMFile, "%d", &m_modelCount);
				m_Models = new Model[m_modelCount];
				for (int i = 0; i < m_modelCount; ++i)
				{
					int id;
					char modeltype[50];


					fscanf_s(RMFile, "%*s %d", &id);
					fscanf_s(RMFile, "%s", modeltype, _countof(modeltype));


					if (strcmp(modeltype, "PLANE") == 0)
					{
						int w, h;
#ifdef OPENGL_PROJECT
						Rectangle plane(openGL);
#else
						Rectangle plane;
#endif
						fscanf_s(RMFile, " %d %d", &w, &h);
#ifdef OPENGL_PROJECT
						plane.Init(w, h, id);
#else
						plane.Init(w, h, dev, id);
#endif
						m_Models[i] = plane;
						plane.disconnect();
					}
					else if (strcmp(modeltype, "FILE") == 0)
					{
						char modelName[255];
						fscanf_s(RMFile, " \"%s\"", modelName, _countof(modelName));
						modelName[strlen(modelName) - 1] = '\0';
#ifdef OPENGL_PROJECT
						m_Models[i] = Model(openGL);
						MODEL_ERROR result = m_Models[i].InitModel(modelName, id);
#else
						MODEL_ERROR result = m_Models[i].InitModel(modelName, id, dev);
#endif
						if (result != MODEL_SUCCESS)
						{
							fclose(RMFile);
							return false;
						}
					}
				}
			}
			if (strcmp(remark, "#2D_Textures:") == 0)
			{
				fscanf_s(RMFile, "%d", &m_textureCount);
				m_Textures = new Texture[m_textureCount];
				for (int i = 0; i < m_textureCount; ++i)
				{
					int id;
					char textureName[255];
					char textureTiling[50];
					fscanf_s(RMFile, "%*s %d", &id);
					fscanf_s(RMFile, "%*s \"%s\"", textureName, _countof(textureName));
					textureName[strlen(textureName) - 1] = '\0';
					fscanf_s(RMFile, "%*s %s", textureTiling, _countof(textureTiling));
#ifdef OPENGL_PROJECT
					m_Textures[i] = Texture(openGL);
#endif
					if (strcmp(textureTiling, "GL_CLAMP_TO_EDGE") == 0)
					{
						m_Textures[i].setWrapMode(CLAMP_TO_EDGE);
					}
					if (strcmp(textureTiling, "GL_REPEAT") == 0)
					{
						m_Textures[i].setWrapMode(REPEAT);
					}
					if (strcmp(textureTiling, "GL_MIRRORED_REPEAT") == 0)
					{
						m_Textures[i].setWrapMode(MIRRORED_REPEAT);
					}
#ifdef OPENGL_PROJECT
					if (!m_Textures[i].Init(textureName, id))
#else
					if (!m_Textures[i].Init(textureName, id, dev, devcon))
#endif
					{
						fclose(RMFile);
						return false;
					}
				}
			}
			if (strcmp(remark, "#Cube_Textures:") == 0)
			{
				fscanf_s(RMFile, "%d", &m_cubeTextureCount);
				m_cubeTextures = new Texture[m_cubeTextureCount];
				for (int i = 0; i < m_cubeTextureCount; ++i)
				{
					int id;
					char textureName[255];
					fscanf_s(RMFile, "%*s %d", &id);
					fscanf_s(RMFile, "%*s \"%s\"", textureName, _countof(textureName));
					textureName[strlen(textureName) - 1] = '\0';
#ifdef OPENGL_PROJECT
					m_cubeTextures[i] = Texture(openGL);
#endif
					m_cubeTextures[i].setWrapMode(REPEAT);//always to avoid weird visual objects
#ifdef OPENGL_PROJECT
					if (!m_cubeTextures[i].InitCubeTexture(textureName, id))
#else
					if (!m_cubeTextures[i].InitCubeTexture(textureName, id, dev, devcon))
#endif
					{
						fclose(RMFile);
						return false;
					}
				}
			}
			if (strcmp(remark, "#Shaders:") == 0)
			{
				fscanf_s(RMFile, "%d", &m_shaderCount);
				m_Shaders = new Shader[m_shaderCount];
				for (int i = 0; i < m_shaderCount; ++i)
				{
					char VertexSName[255];
					char FragmentSName[255];
					int Id;
					fscanf_s(RMFile, "%*s %d", &Id);
					fscanf_s(RMFile, "%*s \"%s\"", VertexSName, _countof(VertexSName));
					VertexSName[strlen(VertexSName) - 1] = '\0';
					fscanf_s(RMFile, "%*s \"%s\"", FragmentSName, _countof(FragmentSName));
					FragmentSName[strlen(FragmentSName) - 1] = '\0';
#ifdef OPENGL_PROJECT
					m_Shaders[i] = Shader(openGL);
					if (m_Shaders[i].Init(VertexSName, FragmentSName, Id) < 0)
#else
					if (m_Shaders[i].Init(VertexSName, FragmentSName, dev, Id) < 0)
#endif
					{
						fclose(RMFile);
						return false;
					}

					int stateCount;
					fscanf_s(RMFile, "%*s %d", &stateCount);
#ifdef OPENGL_PROJECT
					m_Shaders[i].InitStates(stateCount);
					for (int j = 0; j < stateCount; ++j)
					{
						char state[50];
						fscanf(RMFile, "%s", state);
						if (strcmp(state, "GL_CULL_FACE") == 0)
						{
							m_Shaders[i].SetState(j, GL_CULL_FACE);
						}
						if (strcmp(state, "GL_DEPTH_TEST") == 0)
						{
							m_Shaders[i].SetState(j, GL_DEPTH_TEST);
						}
						if (strcmp(state, "GL_BLEND") == 0)
						{
							m_Shaders[i].SetState(j, GL_BLEND);
						}
					}
#else
					D3D11_RASTERIZER_DESC rasterizerDesc;
					rasterizerDesc.FillMode = D3D11_FILL_SOLID;
					rasterizerDesc.CullMode = D3D11_CULL_BACK;
					rasterizerDesc.FrontCounterClockwise = FALSE;
					rasterizerDesc.DepthClipEnable = TRUE;
					rasterizerDesc.ScissorEnable = FALSE;
					rasterizerDesc.AntialiasedLineEnable = FALSE;
					rasterizerDesc.MultisampleEnable = FALSE;
					rasterizerDesc.DepthBias = 0;
					rasterizerDesc.DepthBiasClamp = 0.0f;
					rasterizerDesc.SlopeScaledDepthBias = 0.0f;

					for (int j = 0; j < stateCount; ++j)
					{
						char state[50];
						fscanf_s(RMFile, "%s", state, _countof(state));
						if (strcmp(state, "D3D11_FILL_WIREFRAME") == 0)
							rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
						else if (strcmp(state, "D3D11_FILL_SOLID") == 0)
							rasterizerDesc.FillMode = D3D11_FILL_SOLID;
						else if (strcmp(state, "D3D11_CULL_NONE") == 0)
							rasterizerDesc.CullMode = D3D11_CULL_NONE;
						else if (strcmp(state, "D3D11_CULL_FRONT") == 0)
							rasterizerDesc.CullMode = D3D11_CULL_FRONT;
						else if (strcmp(state, "D3D11_CULL_BACK") == 0)
							rasterizerDesc.CullMode = D3D11_CULL_BACK;
						else if (strcmp(state, "SCISSOR_ENABLE") == 0)
							rasterizerDesc.ScissorEnable = TRUE;
						else if (strcmp(state, "SCISSOR_DISABLE") == 0)
							rasterizerDesc.ScissorEnable = FALSE;
						else if (strcmp(state, "MULTISAMPLE_ENABLED") == 0)
							rasterizerDesc.MultisampleEnable = TRUE;
						else if (strcmp(state, "MULTISAMPLE_DISABLED") == 0)
							rasterizerDesc.MultisampleEnable = FALSE;
						else if (strcmp(state, "GL_BLEND") == 0)
							m_Shaders[i].InitBlendState(dev);
					}
					if (!m_Shaders[i].InitRasterizerState(&rasterizerDesc, dev))
					{
						OutputDebugStringA("Error generating Rasterizer State");
						fclose(RMFile);
						return false;
					}
#endif
				}
			}
			if (strcmp(remark, "STATES") == 0)
			{

			}
		}
		fclose(RMFile);
		return true;
	}

	Model* ResourceManager::GetModelById(unsigned int id)
	{
		for (int i = 0; i < m_modelCount; ++i)
		{
			if (m_Models[i].getId() == id)
			{
				return &m_Models[i];
			}
		}
		return NULL;
	}

	Texture* ResourceManager::GetTextureById(unsigned int id)
	{
		for (int i = 0; i < m_textureCount; ++i)
		{
			if (m_Textures[i].getId() == id)
			{
				return &m_Textures[i];
			}
		}
		return NULL;
	}

	Texture* ResourceManager::GetCubeTextureById(unsigned int id)
	{
		for (int i = 0; i < m_cubeTextureCount; ++i)
		{
			if (m_cubeTextures[i].getId() == id)
			{
				return &m_cubeTextures[i];
			}
		}
		return NULL;
	}

	Shader* ResourceManager::GetShaderById(unsigned int id)
	{
		for (int i = 0; i < m_shaderCount; ++i)
		{
			if (m_Shaders[i].getId() == id)
			{
				return &m_Shaders[i];
			}
		}
		return NULL;
	}

	/*void ResourceManager::EnableStates()
	{
	}
	void ResourceManager::DisableStates()
	{
	}*/

	void ResourceManager::Clean()
	{
		/*if (m_states)
		{
			delete[] m_states;
			m_states = NULL;
		}*/
		if (m_Models)
		{
			delete[] m_Models;
			m_Models = NULL;
		}
		if (m_Textures)
		{
			delete[] m_Textures;
			m_Textures = NULL;
		}
		if (m_cubeTextures)
		{
			delete[] m_cubeTextures;
			m_cubeTextures = NULL;
		}
		if (m_Shaders)
		{
			for (int i = 0; i < m_shaderCount; ++i)
			{
				m_Shaders[i].Clean();
			}
			delete[] m_Shaders;
			m_Shaders = NULL;
		}

	}
}