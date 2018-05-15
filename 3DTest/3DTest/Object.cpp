#include "Object.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Model.h"
#include "Shaders.h"
#include "Camera.h"
#include "Vertex.h"
#include "Globals.h"
#include <d3d11.h>

Object::Object()
{
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
	//to be implemented later
	//for (int i = 0; i < m_CubeTextureCount; ++i)
	//{
	//	m_CubeTextures[i] = ResourceManager::GetInstance()->GetCubicTextureById(cubicTextures->m_textureIds[i]);
	//}
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
	ResourceManager::GetInstance()->EnableStates();
	devcon->VSSetShader(m_Shader->getVertexShader(), 0, 0);
	devcon->PSSetShader(m_Shader->getPixelShader(), 0, 0);
	if (m_Shader->uWVP != -1)
	{
		Matrix wMatrix = m_worldMatrix * cam->getViewMatrix()*cam->getProjectionMatrix();
		D3D11_MAPPED_SUBRESOURCE ms;
		devcon->Map(m_Shader->getVertexConstBufferbyIndex(0), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
		memcpy(ms.pData, &wMatrix.m[0][0], 16 * sizeof(float));                 // copy the data
		devcon->Unmap(m_Shader->getVertexConstBufferbyIndex(0), NULL);
	}
	UINT stride = sizeof(VERTEX);
	UINT offset;
	if (m_Shader->positionAttribute != -1)
	{
		offset = 0;
		devcon->IASetVertexBuffers(m_Shader->positionAttribute, 1, &(m_model->m_vertexBuffer), &stride, &offset);
	}
	if (m_Shader->colorAttribute != -1)
	{
		offset = 12;
		devcon->IASetVertexBuffers(m_Shader->colorAttribute, 1, &(m_model->m_vertexBuffer), &stride, &offset);
	}
	if (m_Shader->uvAttribute != -1)
	{
		offset = 64;
		devcon->IASetVertexBuffers(m_Shader->uvAttribute, 1, &(m_model->m_vertexBuffer), &stride, &offset);
	}
/*	if(m_Shader->uTexture != -1)
	{
		unsigned short textureUnit = 0;
		while (textureUnit < m_TextureCount)
		{
			devcon->PSSetShaderResources(textureUnit, 1, &(m_Textures[textureUnit]->m_TextureView));
			textureUnit++;
		}
	}*/

	devcon->IASetIndexBuffer(m_model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(m_model->getIndexSize(), 0, 0);
	ResourceManager::GetInstance()->DisableStates();
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
}

