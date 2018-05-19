#include "EffectManager.h"
#include "Shaders.h"
#include "Pass.h"
#include "Globals.h"
#include "Quad.h"
#include "Effect.h"
#include "FrameBufferObject.h"
#include <stdio.h>
#include <tchar.h>
#include <D3D11.h>
#include "esUtil.h"
EffectManager::EffectManager()
{
}


EffectManager::~EffectManager()
{
	m_Shaders = NULL;
	m_Effects = NULL;
	m_FBOs = NULL;
	m_Quads = NULL;
}

EffectManager* EffectManager::m_pInstance = NULL;

EffectManager* EffectManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new EffectManager;
	}
	return m_pInstance;
}

void EffectManager::DestroyInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
void EffectManager::Clean()
{
	if (m_Shaders)
	{
		for (int i = 0; i < m_ShaderCount; ++i)
		{
			m_Shaders[i].Clean();
		}
		delete[] m_Shaders;
		m_Shaders = NULL;
	}
	if (m_Effects)
	{
		for (int i = 0; i < m_EffectCount; ++i)
		{
			m_Effects[i].Clean();
		}
		delete[] m_Effects;
		m_Effects = NULL;
	}
	if (m_FBOs)
	{
		delete[] m_FBOs;
		m_FBOs = NULL;
	}
	if (m_Quads)
	{
		delete m_Quads;
		m_Quads = NULL;
	}
}

void EffectManager::Draw(unsigned int iEffect, ID3D11DeviceContext* devcon)
{
	for (int i = 0; i < m_EffectCount; ++i)
	{
		if (m_Effects[i].GetId() == iEffect)
		{
			m_Effects[i].Draw(devcon);
			return;
		}
	}
}

FramebufferObject* EffectManager::GetFBO(int index)
{
	if (index >= 0 && index < m_FBOCount)
	{
		return m_FBOs + index;
	}
	return NULL;
}

Effect* EffectManager::GetEffects()
{
	return m_Effects;
}

Quad* EffectManager::GetQuads()
{
	return m_Quads;
}

Shaders* EffectManager::GetShader(unsigned int ShaderId)
{
	for (int i = 0; i < m_ShaderCount; ++i)
	{
		if (m_Shaders[i].getId() == ShaderId)
		{
			return m_Shaders + i;
		}
	}
}
void EffectManager::PreDraw(bool activate, ID3D11DeviceContext* devcon)
{
	if (!activate)
	{
		
		devcon->OMSetRenderTargets(1, &backbuffer, NULL);
		float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
		devcon->ClearRenderTargetView(backbuffer, color);
		
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		ID3D11RenderTargetView* currentTargetView = m_FBOs[0].GetColorTexture();
		devcon->OMSetRenderTargets(1, &currentTargetView, m_FBOs[0].GetDepthTexture());
		float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
		devcon->ClearRenderTargetView(m_FBOs[0].GetColorTexture(), color);
		devcon->ClearDepthStencilView(m_FBOs[0].GetDepthTexture(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		//glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[0].GetFrameBuffer());
	}
}
bool EffectManager::Init(char* FileName, ID3D11Device* dev, ID3D11DeviceContext* devcon)
{
	if (FileName == NULL)
	{
		return false;
	}
	FILE* EMFile;
	fopen_s(&EMFile, FileName, "r");
	if (EMFile == NULL)
	{
		return false;
	}
	char remark[50];
	while (!feof(EMFile))
	{
		strcpy_s(remark, "");
		fscanf_s(EMFile, "%s", remark, _countof(remark));
		if (strcmp(remark, "#Shaders:") == 0)
		{
			fscanf_s(EMFile, "%d", &m_ShaderCount);
			m_Shaders = new Shaders[m_ShaderCount];
			for (int i = 0; i < m_ShaderCount; ++i)
			{
				char VertexSName[255];
				char FragmentSName[255];
				unsigned int Id;
				fscanf_s(EMFile, "%*s %u", &Id);
				fscanf_s(EMFile, "%*s \"%s\"", VertexSName, _countof(VertexSName));
				VertexSName[strlen(VertexSName) - 1] = '\0';
				fscanf_s(EMFile, "%*s \"%s\"", FragmentSName);
				FragmentSName[strlen(FragmentSName) - 1] = '\0';

				if (m_Shaders[i].Init(VertexSName, FragmentSName, dev, devcon, Id) < 0)
				{
					//return false;
					esLogMessage("Can't use shader %d\n", i);
				}
			}
		}
		if (strcmp(remark, "#FBO") == 0)
		{
			fscanf_s(EMFile, "%*s %d", &m_FBOCount);
			m_FBOs = new FramebufferObject[m_FBOCount];
		}
		if (strcmp(remark, "#Effects:") == 0)
		{
			fscanf_s(EMFile, "%d", &m_EffectCount);
			m_Effects = new Effect[m_EffectCount];
			for (int i = 0; i < m_EffectCount; ++i)
			{
				unsigned int id;
				fscanf_s(EMFile, "%*s %u", &id);

				char Name[25];
				fscanf_s(EMFile, "%*s %s", Name, _countof(Name));

				int passes;
				fscanf_s(EMFile, "%*s %d", &passes);
				if (!m_Effects[i].Init(id, Name, passes))
				{
					return false;
				}
				for (int j = 0; j < passes; ++j)
				{
					Pass* pass = NULL;
					pass = m_Effects[i].GetPass(j);
					if (pass == NULL)
					{
						return false;
					}

					unsigned int PassId;
					fscanf_s(EMFile, " %*s %u", &PassId);
					char PassName[25];
					fscanf_s(EMFile, "%*s %s", PassName, _countof(PassName));
					unsigned int ShaderId;
					fscanf_s(EMFile, " %*s %u", &ShaderId);
					if (!(pass->Init(PassId, PassName, ShaderId)))
					{
						return false;
					}
					fscanf_s(EMFile, "%*s");
					int txCount = 0;
					TextureIdentifier TXId[Globals::maxTextureIdetifierCount];
					char start = '\0';
					//while (start != '\n')
					//{
					//	start = fgetc(EMFile);
					//}
					start = fgetc(EMFile);
					TXId[0].m_type = TEXTURE_NOTUSED;
					TXId[0].m_TextureId = 0;
					while (start != '\n' && !feof(EMFile))
					{
						if (txCount < Globals::maxTextureIdetifierCount)
						{

							switch (start)
							{
							case '0':
							case '1':
							case '2':
							case '3':
							case '4':
							case '5':
							case '6':
							case '7':
							case '8':
							case '9':
								TXId[txCount].m_TextureId = TXId[txCount].m_TextureId * 10 + (start - '0');
								break;
							case 'c':
								TXId[txCount].m_type = TEXTURE_COLOR;
								break;
							case 'd':
								TXId[txCount].m_type = TEXTURE_DEPTH;
								break;
							case ',':
								txCount++;
								if (txCount < Globals::maxTextureIdetifierCount)
								{
									TXId[txCount].m_type = TEXTURE_NOTUSED;
									TXId[txCount].m_TextureId = 0;
								}
								break;
							default:
								break;
							}
						}
						start = fgetc(EMFile);
					}
					pass->SetTextures(txCount + 1, TXId);
					unsigned int Target;
					fscanf_s(EMFile, " %*s %u", &Target);
					pass->SetTarget(Target);

					start = '\0';
					int odCount = 0;
					float OtherData[Globals::maxOtherDataCount];
					while (start != '\n' && !feof(EMFile))
					{
						start = fgetc(EMFile);
					}
					start = fgetc(EMFile);
					while (start == ' ')
					{
						if (odCount >= Globals::maxOtherDataCount)
						{
							return false;
						}
						fscanf_s(EMFile, "%*s %f", &OtherData[odCount++]);
						start = '\0';
						while (start != '\n' && !feof(EMFile))
						{
							start = fgetc(EMFile);
						}
						start = fgetc(EMFile);
					}
					ungetc(start, EMFile);
					pass->SetOtherData(odCount, OtherData);
				}

			}
		}
	}
	fclose(EMFile);
	for (int i = 0; i < m_FBOCount; ++i)
	{
		if (m_FBOs[i].Init(dev, Globals::screenWidth, Globals::screenHeight, i == 0) != FBO_SUCCESS)
		{
			return false;
		}
	}
	m_Quads = new Quad;
	m_Quads->Init(dev, devcon);

	return true;
}
void EffectManager::setDefaultRenderTarget(ID3D11RenderTargetView* renderTarget)
{
	backbuffer = renderTarget;
}
ID3D11RenderTargetView* EffectManager::getBackBuffer()
{
	return backbuffer;
}