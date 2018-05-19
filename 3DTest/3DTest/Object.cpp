#include "Object.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Texture.h"
#include "Model.h"
#include "Shaders.h"
#include "Camera.h"
#include "Vertex.h"
#include "Globals.h"
#include "Light.h"
#include <d3d11.h>

Object::Object()
{
	m_model = NULL;
	m_Textures = NULL;
	m_CubeTextures = NULL;
	m_Lights = NULL;
	m_tilingFactor = 0.0f;
	m_LightCount = 0;
	DepthAdjustment = 0.1;
}


Object::~Object()
{
}
bool Object::Init(unsigned int ID, unsigned modelID, TextureInit* textures2D, TextureInit* cubicTextures, unsigned int shaderID,
	Vector3* position, Vector3* rotation, Vector3* scale)
{
	m_ID = ID;
	m_model = ResourceManager::GetInstance()->GetModelById(modelID);
	if (m_model == NULL)
	{
		return false;
	}
	m_TextureCount = textures2D->m_textureCount;
	m_Textures = new Texture*[m_TextureCount];
	for (int i = 0; i < m_TextureCount; ++i)
	{
		m_Textures[i] = ResourceManager::GetInstance()->GetTextureById(textures2D->m_textureIDs[i]);
	}

	m_CubeTexturesCount = cubicTextures->m_textureCount;
	m_CubeTextures = new Texture*[m_CubeTexturesCount];
	for (int i = 0; i < m_CubeTexturesCount; ++i)
	{
		m_CubeTextures[i] = ResourceManager::GetInstance()->GetCubeTextureById(cubicTextures->m_textureIDs[i]);
	}
	m_Shader = ResourceManager::GetInstance()->GetShaderById(shaderID);
	m_position = *position;
	m_rotation = *rotation;
	m_scale = *scale;
	CalcWorldMatrix();
	return true;
}
void Object::Update(float deltaTime, int pressedKey)
{
	//do something:
}

void Object::CalcWorldMatrix()
{
	Matrix scaleMatrix;
	Matrix rotationXMatrix;
	Matrix rotationYMatrix;
	Matrix rotationZMatrix;
	Matrix translationMatrix;
	scaleMatrix.SetIdentity();
	rotationXMatrix.SetIdentity();
	rotationYMatrix.SetIdentity();
	rotationZMatrix.SetIdentity();
	translationMatrix.SetIdentity();

	scaleMatrix.SetScale(m_scale);
	rotationXMatrix.SetRotationX(m_rotation.x);
	rotationYMatrix.SetRotationY(m_rotation.y);
	rotationZMatrix.SetRotationZ(m_rotation.z);
	translationMatrix.SetTranslation(m_position);

	m_worldMatrix = scaleMatrix * rotationZMatrix * rotationXMatrix * rotationYMatrix * translationMatrix;//OPENGL
	//m_worldMatrix = rotationXMatrix * rotationYMatrix * rotationZMatrix * scaleMatrix * translationMatrix;

}
void Object::Draw(Camera* cam, ID3D11DeviceContext* devcon)
{
	m_Shader->EnableStates(devcon);
	devcon->VSSetShader(m_Shader->getVertexShader(), 0, 0);
	devcon->PSSetShader(m_Shader->getPixelShader(), 0, 0);
	for (int i = 0; i < m_Shader->getNumVertexConstBuffer(); ++i)
	{
		ID3D11Buffer* VSConstBuffer = m_Shader->getVertexConstBufferbyIndex(i);
		
		D3D11_MAPPED_SUBRESOURCE ms;
		devcon->Map(VSConstBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
		if (m_Shader->uWVP.constantbuffer == i)
		{
			Matrix wMatrix = m_worldMatrix * cam->getViewMatrix()*cam->getProjectionMatrix();
			UniformMatrix4f(ms.pData, m_Shader->uWVP.offset, &wMatrix);
		}
		if (m_Shader->uworldMatrix.constantbuffer == i)
			UniformMatrix4f(ms.pData, m_Shader->uworldMatrix.offset, &m_worldMatrix);
		if (m_Shader->uTilingFactor.constantbuffer == i)
			Uniform1f(ms.pData, m_Shader->uTilingFactor.offset, m_tilingFactor);
		devcon->Unmap(VSConstBuffer, NULL);
		
		devcon->VSSetConstantBuffers(0, 1, &VSConstBuffer);
	}
		
	devcon->IASetInputLayout(m_Shader->getVertexBufferLayout());
	UINT stride = sizeof(VERTEX);
	UINT offset;
	if (m_Shader->positionAttribute != -1)
	{
		offset = 0;
		devcon->IASetVertexBuffers(m_Shader->positionAttribute, 1, &(m_model->m_vertexBuffer), &stride, &offset);
	}
	if (m_Shader->colorAttribute != -1)
	{
		offset = 3*sizeof(float);
		devcon->IASetVertexBuffers(m_Shader->colorAttribute, 1, &(m_model->m_vertexBuffer), &stride, &offset);
	}
	if (m_Shader->normalAttribute != -1)
	{
		offset = 7*sizeof(float);
		devcon->IASetVertexBuffers(m_Shader->normalAttribute, 1, &(m_model->m_vertexBuffer), &stride, &offset);
	}
	if (m_Shader->binormalAttribute != -1)
	{
		offset = 10 * sizeof(float);
		devcon->IASetVertexBuffers(m_Shader->binormalAttribute, 1, &(m_model->m_vertexBuffer), &stride, &offset);
	}
	if (m_Shader->tangentAttribute != -1)
	{
		offset = 13 * sizeof(float);
		devcon->IASetVertexBuffers(m_Shader->tangentAttribute, 1, &(m_model->m_vertexBuffer), &stride, &offset);
	}
	if (m_Shader->uvAttribute != -1)
	{
		offset = 16*sizeof(float);
		devcon->IASetVertexBuffers(m_Shader->uvAttribute, 1, &(m_model->m_vertexBuffer), &stride, &offset);
	}
	


	for (int i = 0; i < m_Shader->getNumPixelConstBuffer(); ++i)
	{
		ID3D11Buffer* PSConstBuffer = m_Shader->getPixelConstBufferbyIndex(i);
		devcon->PSSetConstantBuffers(0, 1, &PSConstBuffer);
		D3D11_MAPPED_SUBRESOURCE ms;
		devcon->Map(PSConstBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
		if (m_Shader->ucamPos.constantbuffer == i)
			Uniform3fv(ms.pData, m_Shader->ucamPos.offset, cam->getPosition());
		if (m_Shader->ufogColor.constantbuffer == i)
			Uniform3fv(ms.pData, m_Shader->ufogColor.offset, cam->getFogColor());
		if (m_Shader->ufogStart.constantbuffer == i)
			Uniform1f(ms.pData, m_Shader->ufogStart.offset, cam->getFogStart());
		if (m_Shader->ufogRange.constantbuffer == i)
			Uniform1f(ms.pData, m_Shader->ufogRange.offset, cam->getFogRange());
		if (m_Shader->uTime.constantbuffer == i)
			Uniform1f(ms.pData, m_Shader->uTime.offset, SceneManager::GetInstance()->GetTime());
		if (m_Shader->uFireAmp.constantbuffer == i)
			Uniform1f(ms.pData, m_Shader->uFireAmp.offset, Globals::FireAmplitude);
		if (m_Shader->uWaterColor.constantbuffer == i)
			Uniform3f(ms.pData, m_Shader->uWaterColor.offset, Globals::WaterColor.x, Globals::WaterColor.y, Globals::WaterColor.z);
		if (m_Shader->uFresnelPower.constantbuffer == i)
			Uniform1f(ms.pData, m_Shader->uFresnelPower.offset, Globals::FresnelPower);
		if (m_Shader->uDepthAdjust.constantbuffer == i)
		{
			float depthAdj = (m_model->getMaxU()) ? 1.0 / m_model->getMaxU() : 0.1;
			Uniform1f(ms.pData, m_Shader->uDepthAdjust.offset, depthAdj);
		}
		if (m_Shader->uDepthAdjustDisplacement.constantbuffer == i)
			Uniform1f(ms.pData, m_Shader->uDepthAdjustDisplacement.offset, Globals::WaterDepthAdjustDisplacement);
		if (m_Shader->uMaxReflection.constantbuffer == i)
			Uniform1f(ms.pData, m_Shader->uMaxReflection.offset, Globals::WaterMaxReflection);
		if (m_LightCount)
		{
			if (m_Shader->uNumLights.constantbuffer == i)
				Uniform1i(ms.pData, m_Shader->uNumLights.offset, m_LightCount);
			if (m_Shader->uAmbientColor.constantbuffer == i)
				Uniform3fv(ms.pData, m_Shader->uAmbientColor.offset, SceneManager::GetInstance()->GetAmbientColor());
			if (m_Shader->uAmbientWeight.constantbuffer == i)
				Uniform1f(ms.pData, m_Shader->uAmbientWeight.offset, SceneManager::GetInstance()->GetAmbientWeight());
			if (m_Shader->uSpecularPower.constantbuffer == i)
				Uniform1f(ms.pData, m_Shader->uSpecularPower.offset, m_SpecularPower);
			for (int j = 0; j < m_LightCount; ++j)
			{
				Light* light = NULL;
				light = SceneManager::GetInstance()->GetLightById(m_Lights[j]);
				if (light != NULL)
				{
					if (m_Shader->uLightTypes[j].constantbuffer == i)
						Uniform1i(ms.pData, m_Shader->uLightTypes[j].offset, light->GetType());
					if (m_Shader->uLightColors[j].constantbuffer == i)
						Uniform3fv(ms.pData, m_Shader->uLightColors[j].offset, light->GetColor());
					if(m_Shader->uPosDirs[j].constantbuffer == i)
						Uniform3fv(ms.pData, m_Shader->uPosDirs[j].offset, light->GetPosDir());
				}
			}
		}
		devcon->Unmap(PSConstBuffer, NULL);
		
	}
	
	if(m_Shader->u2DTexturesCount > 0)
	{
		ID3D11ShaderResourceView **textures = new ID3D11ShaderResourceView*[m_Shader->u2DTexturesCount];
		ID3D11SamplerState **samplers = new ID3D11SamplerState*[m_Shader->u2DTexturesCount];
		UINT textureUnit = 0;
		while (textureUnit < m_Shader->u2DTexturesCount)
		{
			textures[textureUnit] = m_Textures[textureUnit]->getTexture();
			samplers[textureUnit] = m_Textures[textureUnit]->getSampler();
			textureUnit++;
		}
		devcon->PSSetSamplers(m_Shader->samplers2DSlot, textureUnit, samplers);
		devcon->PSSetShaderResources(m_Shader->samplers2DSlot, textureUnit, textures);
		delete[] textures;
		delete[] samplers;
	}
	if (m_Shader->uCubeTexturesCount > 0)
	{
		ID3D11ShaderResourceView** cubeTextures = new ID3D11ShaderResourceView*[m_Shader->uCubeTexturesCount];
		ID3D11SamplerState** samplers = new ID3D11SamplerState*[m_Shader->uCubeTexturesCount];
		UINT textureUnit = 0;
		while (textureUnit < m_Shader->uCubeTexturesCount)
		{
			cubeTextures[textureUnit] = m_CubeTextures[textureUnit]->getTexture();
			samplers[textureUnit] = m_CubeTextures[textureUnit]->getSampler();
			textureUnit++;
		}
		devcon->PSSetSamplers(m_Shader->SamplersCubeSlot, textureUnit, samplers);
		devcon->PSSetShaderResources(m_Shader->SamplersCubeSlot, textureUnit, cubeTextures);
		delete[] cubeTextures;
		delete[] samplers;
	}

	devcon->IASetIndexBuffer(m_model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(m_model->getIndexSize(), 0, 0);
	
}

void Object::InitLights(int LightCount, unsigned int* Lights, float specularPower)
{
	m_SpecularPower = specularPower;
	m_LightCount = LightCount;
	if (m_LightCount)
	{
		m_Lights = new unsigned int[m_LightCount];
		for (int i = 0; i < m_LightCount; ++i)
		{
			m_Lights[i] = Lights[i];
		}
	}
}

void Object::InitTilingFactor(float TilingFactor)
{
	m_tilingFactor = TilingFactor;
}

void Object::Clean()
{
	if (m_CubeTextures)
	{
		delete[] m_CubeTextures;
		m_CubeTextures = NULL;
	}
	if (m_Textures)
	{
		delete[] m_Textures;
		m_Textures = NULL;
	}
	if (m_Lights)
	{
		delete[] m_Lights;
		m_Lights = NULL;
	}
}

