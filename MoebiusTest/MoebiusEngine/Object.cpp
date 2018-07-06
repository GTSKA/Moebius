#include "Object.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Vertex.h"
#include "Globals.h"
#include "Light.h"
#ifdef OPENGL_PROJECT
#include "OpenGLClass.h"
#else
#include <d3d11.h>
#endif
namespace MoebiusEngine
{
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
	void Object::Update(float deltaTime)
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
		m_size = (scaleMatrix * Vector4(m_model->getSize(), 0.0f)).xyz();
		rotationXMatrix.SetRotationX(m_rotation.x);
		rotationYMatrix.SetRotationY(m_rotation.y);
		rotationZMatrix.SetRotationZ(m_rotation.z);
		translationMatrix.SetTranslation(m_position);
		m_center = (translationMatrix*Vector4(m_model->getCenter(), 1.0f)).xyz();
		m_worldMatrix = scaleMatrix * rotationZMatrix * rotationXMatrix * rotationYMatrix * translationMatrix;//OPENGL
		//m_worldMatrix = rotationXMatrix * rotationYMatrix * rotationZMatrix * scaleMatrix * translationMatrix;

	}

#ifdef OPENGL_PROJECT
	void Object::Draw(Camera* cam, OpenGLClass* openGL)
	{
		m_Shader->EnableStates();
		openGL->glUseProgram(m_Shader->m_program);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		openGL->glBindBuffer(GL_ARRAY_BUFFER, m_model->getVertexBuffer());
		
		if (m_Shader->uWVP != -1)
		{
			Matrix wMatrix = m_worldMatrix * cam->getViewMatrix() * cam->getProjectionMatrix();
			openGL->glUniformMatrix4fv(m_Shader->uWVP, 1, GL_FALSE, &wMatrix.m[0][0]);
		}
		if (m_Shader->uworldMatrix != -1)
			openGL->glUniformMatrix4fv(m_Shader->uworldMatrix, 1, GL_FALSE, &m_worldMatrix.m[0][0]);
		if (m_LightCount)
		{
			if (m_Shader->uNumLights != -1)
			{
				openGL->glUniform1i(m_Shader->uNumLights, m_LightCount);
			}
			if (m_Shader->uAmbientColor != -1)
			{
				Vector3 AmbientColor = SceneManager::GetInstance()->GetAmbientColor();
				openGL->glUniform3f(m_Shader->uAmbientColor, AmbientColor.x, AmbientColor.y, AmbientColor.z);
			}
			if (m_Shader->uAmbientWeight != -1)
			{
				openGL->glUniform1f(m_Shader->uAmbientWeight, SceneManager::GetInstance()->GetAmbientWeight());
			}
			if (m_Shader->uSpecularPower != -1)
			{
				openGL->glUniform1f(m_Shader->uSpecularPower, m_SpecularPower);
			}
			for (int j = 0; j < m_LightCount; ++j)
			{
				Light* light = NULL;
				light = SceneManager::GetInstance()->GetLightById(m_Lights[j]);
				if (light != NULL)
				{
					if (m_Shader->uLightTypes[j] != -1)
					{
						openGL->glUniform1i(m_Shader->uLightTypes[j], light->GetType());
					}
					if (m_Shader->uLightColors[j] != -1)
					{
						Vector3 color = light->GetColor();
						openGL->glUniform3f(m_Shader->uLightColors[j], color.x, color.y, color.z);
					}
					if (m_Shader->uPosDirs[j] != -1)
					{
						Vector3 posDir = light->GetPosDir();
						openGL->glUniform3f(m_Shader->uPosDirs[j], posDir.x, posDir.y, posDir.z);
					}
				}

			}
		}
		
		
		unsigned short textureUnit = 0;
		if (m_Shader->uCubeTexture != -1)
		{
			if (m_CubeTexturesCount > 0)
			{
				//make a Texture unit
				openGL->glActiveTexture(GL_TEXTURE0 + textureUnit);
				//bind the texture
				glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTextures[0]->getTextureHandle());
				openGL->glUniform1i(m_Shader->uCubeTexture, textureUnit);
				textureUnit++;
			}
		}
		if (m_Shader->uTexture != -1)
		{
			//make a Texture unit
			openGL->glActiveTexture(GL_TEXTURE0 + textureUnit);
			//bind the texture
			glBindTexture(GL_TEXTURE_2D, m_Textures[0]->getTextureHandle());
			//set the uniform sampler
			openGL->glUniform1i(m_Shader->uTexture, textureUnit);
			textureUnit++;
		}
		if (m_Shader->uTilingFactor != -1)
		{
			openGL->glUniform1f(m_Shader->uTilingFactor, m_tilingFactor);
		}

		if (m_Shader->uTime != -1)
		{
			openGL->glUniform1f(m_Shader->uTime, SceneManager::GetInstance()->GetTime());
		}

		if (m_Shader->ucamPos != -1)
		{
			Vector3 camPos = cam->getPosition();
			openGL->glUniform3f(m_Shader->ucamPos, camPos.x, camPos.y, camPos.z);
		}
		if (m_Shader->ufogColor != -1)
		{
			Vector3 fogColor = cam->getFogColor();
			openGL->glUniform3f(m_Shader->ufogColor, fogColor.x, fogColor.y, fogColor.z);
		}
		if (m_Shader->ufogStart != -1)
		{
			openGL->glUniform1f(m_Shader->ufogStart, cam->getFogStart());
		}
		if (m_Shader->ufogRange != -1)
		{
			openGL->glUniform1f(m_Shader->ufogRange, cam->getFogRange());
		}
		if (m_Shader->uFireAmp != -1)
		{
			openGL->glUniform1f(m_Shader->uFireAmp, Globals::FireAmplitude);
		}
		if (m_Shader->uWaterColor != -1)
		{
			openGL->glUniform3f(m_Shader->uWaterColor, Globals::WaterColor.x, Globals::WaterColor.y, Globals::WaterColor.z);
		}
		if (m_Shader->uFresnelPower != -1)
		{
			openGL->glUniform1f(m_Shader->uFresnelPower, Globals::FresnelPower);
		}
		if (m_Shader->uDepthAdjust != -1)
		{
			float depthAdj = (m_model->getMaxU()) ? 1.0 / m_model->getMaxU() : 0.1;
			openGL->glUniform1f(m_Shader->uDepthAdjust, depthAdj);
		}
		if (m_Shader->uDepthAdjustDisplacement != -1)
		{
			openGL->glUniform1f(m_Shader->uDepthAdjustDisplacement, Globals::WaterDepthAdjustDisplacement);
		}
		if (m_Shader->uMaxReflection != -1)
		{
			openGL->glUniform1f(m_Shader->uMaxReflection, Globals::WaterMaxReflection);
		}
		int n = 0;
		while (n < m_TextureCount)
		{
			if (m_Shader->uTexLoc[n] != -1)
			{
				//make a Texture unit
				openGL->glActiveTexture(GL_TEXTURE0 + textureUnit);
				//bind the texture
				glBindTexture(GL_TEXTURE_2D, m_Textures[n]->getTextureHandle());
				//set the uniform sampler
				openGL->glUniform1i(m_Shader->uTexLoc[n], textureUnit);
				textureUnit++;
			}
			n++;
		}
		openGL->glBindVertexArray(m_VAO);
		openGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->getIndexBuffer());
		glDrawElements(GL_TRIANGLES, m_model->getIndexSize(), GL_UNSIGNED_INT, (void*)0);
		m_Shader->DisableStates();
	}
#else
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
			offset = 3 * sizeof(float);
			devcon->IASetVertexBuffers(m_Shader->colorAttribute, 1, &(m_model->m_vertexBuffer), &stride, &offset);
		}
		if (m_Shader->normalAttribute != -1)
		{
			offset = 7 * sizeof(float);
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
			offset = 16 * sizeof(float);
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
						if (m_Shader->uPosDirs[j].constantbuffer == i)
							Uniform3fv(ms.pData, m_Shader->uPosDirs[j].offset, light->GetPosDir());
					}
				}
			}
			devcon->Unmap(PSConstBuffer, NULL);

		}
		ID3D11ShaderResourceView **textures = NULL;
		ID3D11SamplerState **samplers = NULL;
		ID3D11ShaderResourceView** cubeTextures = NULL;
		ID3D11SamplerState** cubesamplers = NULL;
		if (m_Shader->u2DTexturesCount > 0)
		{
			textures = new ID3D11ShaderResourceView*[m_Shader->u2DTexturesCount];
			samplers = new ID3D11SamplerState*[m_Shader->u2DTexturesCount];
			UINT textureUnit = 0;
			while (textureUnit < m_Shader->u2DTexturesCount)
			{
				textures[textureUnit] = m_Textures[textureUnit]->getTexture();
				samplers[textureUnit] = m_Textures[textureUnit]->getSampler();
				textureUnit++;
			}
			devcon->PSSetSamplers(m_Shader->samplers2DSlot, textureUnit, samplers);
			devcon->PSSetShaderResources(m_Shader->samplers2DSlot, textureUnit, textures);


		}
		if (m_Shader->uCubeTexturesCount > 0)
		{
			cubeTextures = new ID3D11ShaderResourceView*[m_Shader->uCubeTexturesCount];
			cubesamplers = new ID3D11SamplerState*[m_Shader->uCubeTexturesCount];
			UINT textureUnit = 0;
			while (textureUnit < m_Shader->uCubeTexturesCount)
			{
				cubeTextures[textureUnit] = m_CubeTextures[textureUnit]->getTexture();
				cubesamplers[textureUnit] = m_CubeTextures[textureUnit]->getSampler();
				textureUnit++;
			}
			devcon->PSSetSamplers(m_Shader->SamplersCubeSlot, textureUnit, cubesamplers);
			devcon->PSSetShaderResources(m_Shader->SamplersCubeSlot, textureUnit, cubeTextures);

		}

		devcon->IASetIndexBuffer(m_model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		devcon->DrawIndexed(m_model->getIndexSize(), 0, 0);

		if (m_Shader->u2DTexturesCount > 0)
		{
			for (int i = 0; i < m_Shader->u2DTexturesCount; ++i)
			{
				textures[i] = nullptr;
			}
			devcon->PSSetShaderResources(m_Shader->samplers2DSlot, m_Shader->u2DTexturesCount, textures);
			delete[] textures;
			delete[] samplers;
		}
		if (m_Shader->uCubeTexturesCount > 0)
		{
			for (int i = 0; i < m_Shader->uCubeTexturesCount; ++i)
			{
				cubeTextures[i] = nullptr;
			}
			devcon->PSSetShaderResources(m_Shader->SamplersCubeSlot, m_Shader->uCubeTexturesCount, cubeTextures);
			delete[] cubeTextures;
			delete[] cubesamplers;
		}
	}
#endif



	

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

	Vector3 Object::getCenter()
	{
		return m_center;
	}
	Vector3 Object::getSize()
	{
		return m_size;
	}
	void Object::setFrustum(bool frustum)
	{
		m_frustum = frustum;
	}
	bool Object::shouldCalcFrustum()
	{
		return m_frustum;
	}
#ifdef OPENGL_PROJECT
	void Object::InitVAO(OpenGLClass* OpenGL)
	{
		OpenGL->glGenVertexArrays(1, &m_VAO);
		OpenGL->glBindVertexArray(m_VAO);

		if (m_Shader->positionAttribute != -1)
		{
			OpenGL->glEnableVertexAttribArray(m_Shader->positionAttribute);
			OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_model->getVertexBuffer());
			OpenGL->glVertexAttribPointer(m_Shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), 0);
		}
		if (m_Shader->colorAttribute != -1)
		{
			OpenGL->glEnableVertexAttribArray(m_Shader->colorAttribute);
			OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_model->getVertexBuffer());
			OpenGL->glVertexAttribPointer(m_Shader->colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)(3 * sizeof(float)));
		}
		if (m_Shader->normalAttribute != -1)
		{
			OpenGL->glEnableVertexAttribArray(m_Shader->normalAttribute);
			OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_model->getVertexBuffer());
			OpenGL->glVertexAttribPointer(m_Shader->normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)(7 * sizeof(float)));
		}
		if (m_Shader->binormalAttribute != -1)
		{
			OpenGL->glEnableVertexAttribArray(m_Shader->binormalAttribute);
			OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_model->getVertexBuffer());
			OpenGL->glVertexAttribPointer(m_Shader->binormalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)(10 * sizeof(float)));
		}
		if (m_Shader->tangentAttribute != -1)
		{
			OpenGL->glEnableVertexAttribArray(m_Shader->tangentAttribute);
			OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_model->getVertexBuffer());
			OpenGL->glVertexAttribPointer(m_Shader->tangentAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)(13 * sizeof(float)));
		}
		if (m_Shader->uvAttribute != -1)
		{
			OpenGL->glEnableVertexAttribArray(m_Shader->uvAttribute);
			OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_model->getVertexBuffer());
			OpenGL->glVertexAttribPointer(m_Shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)(16 * sizeof(float)));
		}

	}
#endif

}