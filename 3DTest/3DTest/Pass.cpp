#include "Pass.h"
#include "Shaders.h"
#include "Quad.h"
#include "Globals.h"
#include "EffectManager.h"
#include "FramebufferObject.h"
#include <d3d11.h>
#include <tchar.h>
#include <math.h>
#include "SceneManager.h"

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
				ID3D11RenderTargetView* currentTargetView = EffectManager::GetInstance()->GetFBO(m_TargetId -1)->GetRenderTarget();
				devcon->OMSetRenderTargets(1, &currentTargetView, EffectManager::GetInstance()->GetFBO(m_TargetId - 1)->GetDepthStencil());
				float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
				devcon->ClearRenderTargetView(currentTargetView, color);
				if(EffectManager::GetInstance()->GetFBO(m_TargetId - 1)->GetDepthStencil())
					devcon->ClearDepthStencilView(EffectManager::GetInstance()->GetFBO(m_TargetId - 1)->GetDepthStencil(), D3D11_CLEAR_DEPTH, 1.0f, 0);
			}
			else
			{
				ID3D11RenderTargetView* currentTargetView = EffectManager::GetInstance()->getDefaultBuffer();
				float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
				devcon->OMSetRenderTargets(1, &currentTargetView, EffectManager::GetInstance()->getDefaultStencil());
				devcon->ClearRenderTargetView(currentTargetView, color);
				devcon->ClearDepthStencilView(EffectManager::GetInstance()->getDefaultStencil(), D3D11_CLEAR_DEPTH, 1.0f, 0);
				
			}
			Shader->EnableStates(devcon);
			devcon->VSSetShader(Shader->getVertexShader(), 0, 0);
			devcon->PSSetShader(Shader->getPixelShader(), 0, 0);
			ID3D11ShaderResourceView** textures = new ID3D11ShaderResourceView*[m_TextureCount];
			ID3D11SamplerState** samplers = new ID3D11SamplerState*[m_TextureCount];
			int texcount = 0;
			for (int i = 0; i < m_TextureCount; ++i)
			{
				if (m_Textures[i].m_type != TEXTURE_NOTUSED)
				{
					if (m_Textures[i].m_type == TEXTURE_DEPTH)
					{
						textures[i] = EffectManager::GetInstance()->GetFBO(m_Textures[i].m_TextureId - 1)->GetDepthTexture();
						samplers[i] = EffectManager::GetInstance()->GetFBO(m_Textures[i].m_TextureId - 1)->GetDepthSampler();
						texcount++;
					}
					else
					{
						textures[i] = EffectManager::GetInstance()->GetFBO(m_Textures[i].m_TextureId - 1)->GetColorTexture();
						samplers[i] = EffectManager::GetInstance()->GetFBO(m_Textures[i].m_TextureId - 1)->GetColorSampler();
						texcount++;
					}
				}
			}
			devcon->PSSetSamplers(Shader->samplers2DSlot, texcount, samplers);
			devcon->PSSetShaderResources(Shader->samplers2DSlot, texcount, textures);
			
			
			for (int i = 0; i < Shader->getNumPixelConstBuffer(); ++i)
			{
				ID3D11Buffer* PSConstBuffer = Shader->getPixelConstBufferbyIndex(i);
				devcon->PSSetConstantBuffers(0, 1, &PSConstBuffer);
				D3D11_MAPPED_SUBRESOURCE ms;
				devcon->Map(PSConstBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
				if (Shader->uTime.constantbuffer == i)
					Uniform1f(ms.pData, Shader->uTime.offset, SceneManager::GetInstance()->GetTime());
				if (Shader->uFireAmp.constantbuffer == i)
					Uniform1f(ms.pData, Shader->uFireAmp.offset, Globals::FireAmplitude);
				if (Shader->uWaterColor.constantbuffer == i)
					Uniform3f(ms.pData, Shader->uWaterColor.offset, Globals::WaterColor.x, Globals::WaterColor.y, Globals::WaterColor.z);
				if (Shader->uFresnelPower.constantbuffer == i)
					Uniform1f(ms.pData, Shader->uFresnelPower.offset, Globals::FresnelPower);
				
				if (Shader->uDepthAdjustDisplacement.constantbuffer == i)
					Uniform1f(ms.pData, Shader->uDepthAdjustDisplacement.offset, Globals::WaterDepthAdjustDisplacement);
				if (Shader->uMaxReflection.constantbuffer == i)
					Uniform1f(ms.pData, Shader->uMaxReflection.offset, Globals::WaterMaxReflection);
				if (Shader->uStep.constantbuffer == i)
				{
					float x, y, z, w;
					x = 1.0f / (float)(Globals::screenWidth);
					y = 1.0f / (float)(Globals::screenHeight);
					z = sqrt(2.0f) / 2.0f * x;
					w = sqrt(2.0f) / 2.0f * y;
					Uniform4f(ms.pData, Shader->uStep.offset, m_otherData[0] * x, m_otherData[0] * y, m_otherData[0] * z, m_otherData[0] * w);
				}
				if (Shader->uLimit.constantbuffer == i)
				{
					Uniform1f(ms.pData, Shader->uLimit.offset, m_otherData[0]);
				}
				if (Shader->uNear.constantbuffer == i)
				{
					if (m_otherDataCount > 1)
					{
						Uniform1f(ms.pData, Shader->uNear.offset, m_otherData[1]);
					}
				}
				if (Shader->uFar.constantbuffer == i)
				{
					if (m_otherDataCount > 2)
					{
						Uniform1f(ms.pData, Shader->uFar.offset, m_otherData[2]);
					}
				}
				if (Shader->uFade.constantbuffer == i)
				{
					if (m_otherDataCount > 3)
					{
						Uniform1f(ms.pData, Shader->uFade.offset, m_otherData[3]);
					}
				}
				if (Shader->uClarity.constantbuffer == i)
				{
					if (m_otherDataCount > 4)
					{
						Uniform1f(ms.pData, Shader->uClarity.offset, m_otherData[4]);
					}
				}
				devcon->Unmap(PSConstBuffer, NULL);
			}
			
			
			EffectManager::GetInstance()->GetQuads()->Draw(Shader, devcon);
			for (int i = 0; i < m_TextureCount; ++i)
			{
				textures[i] = nullptr;
			}
			devcon->PSSetShaderResources(Shader->samplers2DSlot, texcount, textures);
			delete[] textures;
			delete[] samplers;

		}
	}
}