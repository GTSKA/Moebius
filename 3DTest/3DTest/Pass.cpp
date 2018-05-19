#include "Pass.h"
#include "Shaders.h"
#include "Quad.h"
#include "Globals.h"
#include "EffectManager.h"
#include "FramebufferObject.h"
#include <d3d11.h>
#include <tchar.h>

Pass::Pass()
{
}


Pass::~Pass()
{

}

void Pass::Clean()
{
	if (m_otherData)
	{
		delete[] m_otherData;
		m_otherData = NULL;
	}
	if (m_Textures)
	{
		delete[] m_Textures;
		m_Textures = NULL;
	}
}
bool Pass::Init(unsigned int Id, const char* PassName, unsigned int ShaderId)
{
	m_Id = Id;
	if (PassName == NULL)
	{
		return false;
	}
	if (strlen(PassName) > 25)
	{
		return false;
	}
	strcpy_s(m_PassName, PassName);
	m_shaderId = ShaderId;
	return true;
}
void Pass::SetTarget(unsigned int Target)
{
	m_TargetId = Target;
}
void Pass::SetOtherData(int OtherDataCount, float* OtherData)
{
	m_otherDataCount = OtherDataCount;
	m_otherData = new float[m_otherDataCount];
	for (int i = 0; i < m_otherDataCount; ++i)
	{
		m_otherData[i] = OtherData[i];
	}
}
void Pass::SetTextures(int TextureCount, TextureIdentifier* Textures)
{
	m_TextureCount = TextureCount;
	m_Textures = new TextureIdentifier[m_TextureCount];
	for (int i = 0; i < m_TextureCount; ++i)
	{
		m_Textures[i] = Textures[i];
	}
}

void Pass::Draw(ID3D11DeviceContext* devcon)
{
	Shaders* Shader = EffectManager::GetInstance()->GetShader(m_shaderId);
	for (int j = 0; j < m_TextureCount; ++j)
	{
		if (m_Textures[j].m_type != TEXTURE_NOTUSED)
		{
			if (m_TargetId > 0)
			{
				//bindframebuffer
				ID3D11RenderTargetView* currentTargetView = EffectManager::GetInstance()->GetFBO(m_TargetId -1)->GetColorTexture();
				devcon->OMSetRenderTargets(1, &currentTargetView, EffectManager::GetInstance()->GetFBO(m_TargetId - 1)->GetDepthTexture());
				float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
				devcon->ClearRenderTargetView(currentTargetView, color);
				devcon->ClearDepthStencilView(EffectManager::GetInstance()->GetFBO(m_TargetId - 1)->GetDepthTexture(), D3D11_CLEAR_DEPTH, 1.0f, 0);
			}
			else
			{
				ID3D11RenderTargetView* currentTargetView = EffectManager::GetInstance()->getBackBuffer();
				float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
				devcon->OMSetRenderTargets(1, &currentTargetView, NULL);
				devcon->ClearRenderTargetView(currentTargetView, color);
			}
			devcon->VSSetShader(Shader->getVertexShader(), 0, 0);
			devcon->PSSetShader(Shader->getPixelShader(), 0, 0);
			for (int i = 0; i < m_TextureCount; ++i)
			{
				if (m_Textures[i].m_type != TEXTURE_NOTUSED)
				{
					if (m_Textures[i].m_type == TEXTURE_DEPTH)
					{
						
					}
					else
					{
						
					}
				}
			}
			if (Shader->uStep != -1)
			{
				float x, y, z, w;
				x = 1.0f / (float)(Globals::screenWidth);
				y = 1.0f / (float)(Globals::screenHeight);
				z = sqrt(2.0f) / 2.0f * x;
				w = sqrt(2.0f) / 2.0f * y;
				glUniform4f(Shader->uStep, m_otherData[0] * x, m_otherData[0] * y, m_otherData[0] * z, m_otherData[0] * w);
			}
			if (Shader->uLimit != -1)
			{
				glUniform1f(Shader->u_limit, m_otherData[0]);
			}
			if (Shader->uNear.constantbuffer != -1)
			{
				if (m_otherDataCount > 1)
				{
					Uniform1f(Shader->u_near, m_otherData[1]);
				}
			}
			if (Shader->uFar.constantbuffer != -1)
			{
				if (m_otherDataCount > 2)
				{
					glUniform1f(Shader->u_far, m_otherData[2]);
				}
			}
			if (Shader->uFade.constantbuffer != -1)
			{
				if (m_otherDataCount > 3)
				{
					glUniform1f(Shader->u_fade, m_otherData[3]);
				}
			}
			if (Shader->uClarity.constantbuffer != -1)
			{
				if (m_otherDataCount > 4)
				{
					glUniform1f(Shader->u_clarity, m_otherData[4]);
				}
			}
			EffectManager::GetInstance()->GetQuads()->Draw(Shader, devcon);
		}
	}
}