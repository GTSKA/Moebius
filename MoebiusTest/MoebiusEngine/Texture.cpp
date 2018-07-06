#include "Texture.h"
#include "TGA.h"
#include <d3d11.h>

namespace MoebiusEngine
{
	Texture::Texture()
	{
		m_height = 0;
		m_width = 0;
		m_bpp = 0;
#ifndef OPENGL_PROJECT
		m_Texture = 0;
		m_TextureView = 0;
#endif
		m_Id = 0;
	}

	Texture::~Texture()
	{
	}
	void Texture::shutdown()
	{
#ifndef OPENGL_PROJECT
		if (m_TextureView)
		{
			m_TextureView->Release();
			m_TextureView = 0;
		}
		if (m_Texture)
		{
			m_Texture->Release();
			m_Texture = 0;
		}
#endif
			
	}

#ifdef OPENGL_PROJECT
	Texture::Texture(OpenGLClass* OpenGL)
	{
		m_height = 0;
		m_width = 0;
		m_bpp = 0;
		m_OpenGL = OpenGL;
	}
	bool Texture::Init(char* textureName, unsigned int Id)
	{
		m_Id = Id;
		return Init(textureName);
	}
#else
	bool Texture::Init(char* textureName, unsigned int Id, ID3D11Device* dev, ID3D11DeviceContext* devcon)
	{
		m_Id = Id;
		return Init(textureName, dev, devcon);
	}
#endif

#ifdef OPENGL_PROJECT
	bool Texture::Init(char* textureName)
	{
		//Create the OpenGL Texture resource handle
		glGenTextures(1, &m_TextureHandle);

		//Bind to 2D texture type
		glBindTexture(GL_TEXTURE_2D, m_TextureHandle);

		//Create CPU buffer
		char* bufferTGA = LoadTGA(textureName, &m_width, &m_height, &m_bpp);
		if (bufferTGA == NULL)
		{
			return false;
		}
		//Load the Texture into OpenGL Texture resource
		if (m_bpp == 24)//why 24? 8 * 3, so 3 bytes, but the else is not the only case
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, bufferTGA);
		}
		else
		{
#pragma mark EXTEND_OPTIONS //Refer to the table at Texturing documentation
			if (m_bpp > 24)
			{
				//there is only one case RGBA
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferTGA);
			}
			if (m_bpp < 24)
			{
				if (m_bpp == 16)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferTGA);
					//this for _4_4_4_4 or _5_5_5_1 what about _5_6_5? and Luminence_alpha?
				}
				if (m_bpp == 8)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferTGA);
					//the other case is GL_ALPHA
				}
			}
		}
		delete[] bufferTGA;
		bufferTGA = NULL;

		//set filters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//generate mipmap chain
		m_OpenGL->glGenerateMipmap(GL_TEXTURE_2D);

		//set wrapping models
		if (m_tiling == REPEAT)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		if (m_tiling == CLAMP_TO_EDGE)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		if (m_tiling == MIRRORED_REPEAT)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		}
		return true;
	}
	bool Texture::InitCubeTexture(char* textureName)
	{
		//Create the OpenGL Texture resource handle
		glGenTextures(1, &m_TextureHandle);

		//Bind to 2D texture type
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureHandle);

		//Create CPU buffer
		char* bufferTGA = LoadTGA(textureName, &m_width, &m_height, &m_bpp);
		if (bufferTGA == NULL)
		{
			return false;
		}
		if (!ExtractFace(1, 0, bufferTGA))
		{
			return false;
		}
		for (int i = 0; i < 4; ++i)//4 faces
		{
			if (!ExtractFace(i, 1, bufferTGA))
			{
				return false;
			}
		}
		if (!ExtractFace(1, 2, bufferTGA))
		{
			return false;
		}
		//set filters
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//generate mipmap chain
		m_OpenGL->glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, bufferTGA);
		delete[] bufferTGA;
		return true;
	}
#else
	bool Texture::Init(char* textureName, ID3D11Device* dev, ID3D11DeviceContext* devcon)
	{
		HRESULT hr = S_OK;
		//D3D11_SUBRESOURCE_DATA initData;
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		char* bufferTGA = LoadTGA(textureName, &m_width, &m_height, &m_bpp);
		desc.Width = m_width;
		desc.Height = m_height;
		desc.MipLevels = desc.ArraySize = 1;

		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		hr = dev->CreateTexture2D(&desc, NULL, &m_Texture);
		if (FAILED(hr))
			return false;
		if (m_bpp == 24)
		{
			char* bufferTGA2 = new char[m_width*m_height * 4];
			for (int i = 0; i < m_width*m_height; ++i)
			{
				bufferTGA2[i * 4] = bufferTGA[i * 3];
				bufferTGA2[i * 4 + 1] = bufferTGA[i * 3 + 1];
				bufferTGA2[i * 4 + 2] = bufferTGA[i * 3 + 2];
				bufferTGA2[i * 4 + 3] = 255;
			}

			D3D11_MAPPED_SUBRESOURCE ms;
			devcon->Map(m_Texture, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
			memcpy(ms.pData, bufferTGA2, m_width*m_height * 4);                 // copy the data
			devcon->Unmap(m_Texture, NULL);
			delete[] bufferTGA2;
		}
		else
		{
			D3D11_MAPPED_SUBRESOURCE ms;
			devcon->Map(m_Texture, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
			memcpy(ms.pData, bufferTGA, m_width*m_height * 4);                 // copy the data
			devcon->Unmap(m_Texture, NULL);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;


		hr = dev->CreateShaderResourceView(m_Texture, &srvDesc, &m_TextureView);
		if (FAILED(hr))
			return false;

		//devcon->GenerateMips(m_TextureView);
		delete[] bufferTGA;

		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		if (m_tiling == REPEAT)
		{
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		}
		if (m_tiling == CLAMP_TO_EDGE)
		{
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		}
		if (m_tiling == MIRRORED_REPEAT)
		{
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		}
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MaxAnisotropy = 8;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.MipLODBias = 2.0f;
		dev->CreateSamplerState(&samplerDesc, &m_sampler);

		return true;
	}
	bool Texture::InitCubeTexture(char* textureName, ID3D11Device* dev, ID3D11DeviceContext* devcon)
	{
		char* bufferTGA = LoadTGA(textureName, &m_width, &m_height, &m_bpp);
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = m_width / 4;
		desc.Height = m_height / 3;
		desc.MipLevels = 1;
		desc.ArraySize = 6;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		char* faces[6];

		D3D11_SUBRESOURCE_DATA pData[6];
		if (bufferTGA == NULL)
		{
			return false;
		}
		if (!ExtractFace(1, 0, bufferTGA, faces + 0))
		{
			return false;
		}
		for (int i = 0; i < 4; ++i)//4 faces
		{
			if (!ExtractFace(i, 1, bufferTGA, faces + i + 1))
			{
				return false;
			}
		}
		if (!ExtractFace(1, 2, bufferTGA, faces + 5))
		{
			return false;
		}
		for (int i = 0; i < 6; ++i)
		{
			pData[i].SysMemPitch = m_width;
			pData[i].SysMemSlicePitch = 0;
		}
		//according to DirectX accomodation https://msdn.microsoft.com/en-us/library/windows/desktop/bb204881(v=vs.85).aspx
		pData[0].pSysMem = faces[3];
		pData[1].pSysMem = faces[1];
		pData[2].pSysMem = faces[0];
		pData[3].pSysMem = faces[5];
		pData[4].pSysMem = faces[2];
		pData[5].pSysMem = faces[4];

		HRESULT hr = dev->CreateTexture2D(&desc, pData, &m_Texture);
		if (FAILED(hr))
			return false;
		for (int i = 5; i >= 0; --i)
			delete[] faces[i];
		delete[] bufferTGA;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;


		hr = dev->CreateShaderResourceView(m_Texture, &srvDesc, &m_TextureView);
		if (FAILED(hr))
			return false;

		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		if (m_tiling == REPEAT)
		{
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		}
		if (m_tiling == CLAMP_TO_EDGE)
		{
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		}
		if (m_tiling == MIRRORED_REPEAT)
		{
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		}
		samplerDesc.MaxAnisotropy = 8;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.MipLODBias = 2.0f;
		dev->CreateSamplerState(&samplerDesc, &m_sampler);
		return true;
	}
#endif
#ifdef OPENGL_PROJECT
	bool Texture::InitCubeTexture(char* textureName, unsigned int Id)
	{
		m_Id = Id;
		return InitCubeTexture(textureName);
	}
#else
	bool Texture::InitCubeTexture(char* textureName, unsigned int Id, ID3D11Device* dev, ID3D11DeviceContext* devcon)
	{
		m_Id = Id;
		return InitCubeTexture(textureName, dev, devcon);
	}
#endif
#ifdef OPENGL_PROJECT
	bool Texture::ExtractFace(int hOffset, int vOffset, const char* bufferTGA)
	{
		char* buffer2TGA;
		int offsetSize = (m_width / 4);
		int offset = 0;
		int bufferSize = offsetSize * offsetSize * (m_bpp / 8);
		int lineSize = offsetSize * (m_bpp / 8);
		buffer2TGA = new char[bufferSize];
		int face;
		errno_t err;
		if (!vOffset)
		{
			face = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		}
		if (vOffset == 2)
		{
			face = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		}
		if (vOffset == 1)
		{
			switch (hOffset)
			{
			case 0:
				face = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
				break;
			case 1:
				face = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
				break;
			case 2:
				face = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
				break;
			case 3:
				face = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
				break;
			}
		}
		for (int i = 0; i < offsetSize; ++i)
		{
			int bufferOffset = (vOffset * offsetSize + i)* m_width + (hOffset*offsetSize);
			bufferOffset *= (m_bpp / 8);
			rsize_t size = bufferSize - offset;
			err = memcpy_s(buffer2TGA + offset, size, bufferTGA + bufferOffset, lineSize);
			if (err)
			{
				return false;
			}
			offset += lineSize;
		}
		//Load the Texture into OpenGL Texture resource
		if (m_bpp == 24)//why 24? 8 * 3, so 3 bytes, but the else is not the only case
		{
			glTexImage2D(face, 0, GL_RGB, offsetSize, offsetSize, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer2TGA);
		}
		else
		{
			if (m_bpp > 24)
			{
				//there is only one case RGBA
				glTexImage2D(face, 0, GL_RGBA, offsetSize, offsetSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer2TGA);
			}
			if (m_bpp < 24)
			{
				if (m_bpp == 16)
				{
					glTexImage2D(face, 0, GL_RGBA, offsetSize, offsetSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer2TGA);
					//this for _4_4_4_4 or _5_5_5_1 what about _5_6_5? and Luminence_alpha?
				}
				if (m_bpp == 8)
				{
					glTexImage2D(face, 0, GL_LUMINANCE, offsetSize, offsetSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer2TGA);
					//the other case is GL_ALPHA
				}
			}
		}

		delete[] buffer2TGA;
		return true;
	}
#else
	bool Texture::ExtractFace(int hOffset, int vOffset, const char* bufferTGA, char** face)
	{
		char* buffer2TGA;
		int offsetSizeW = (m_width / 4);
		int offsetSizeH = (m_height / 3);
		int offset = 0;
		int bufferSize = offsetSizeW * offsetSizeH * (m_bpp / 8);
		*face = new char[offsetSizeW * offsetSizeH * 4];
		int lineSize = offsetSizeW * (m_bpp / 8);
		buffer2TGA = new char[bufferSize];
		errno_t err;

		for (int i = 0; i < offsetSizeH; ++i)
		{
			int bufferOffset = (vOffset * offsetSizeH + i)* m_width + (hOffset*offsetSizeW);
			bufferOffset *= (m_bpp / 8);
			rsize_t size = bufferSize - offset;
			err = memcpy_s(buffer2TGA + offset, size, bufferTGA + bufferOffset, lineSize);
			if (err)
			{
				return false;
			}
			offset += lineSize;
		}
		if (m_bpp == 24)//why 24? 8 * 3, so 3 bytes, but the else is not the only case
		{
			char* bufferTGA2 = new char[offsetSizeW * offsetSizeH * 4];

			for (int k = 0; k < offsetSizeW * offsetSizeH; ++k)
			{
				bufferTGA2[k * 4] = buffer2TGA[k * 3];
				bufferTGA2[k * 4 + 1] = buffer2TGA[k * 3 + 1];
				bufferTGA2[k * 4 + 2] = buffer2TGA[k * 3 + 2];
				bufferTGA2[k * 4 + 3] = 255;
			}
			memcpy(*face, bufferTGA2, offsetSizeW * offsetSizeH * 4);
			delete[] bufferTGA2;
		}
		else
		{
			if (m_bpp > 24)
			{
				//there is only one case RGBA
				memcpy(*face, buffer2TGA, offsetSizeW * offsetSizeH * 4);
			}
			if (m_bpp < 24)
			{
				//TO DO:
			}
		}
		delete[] buffer2TGA;
		return true;
	}
#endif

	void Texture::setWrapMode(WRAPMODE imode)
	{
		m_tiling = imode;
	}

#ifdef OPENGL_PROJECT
	GLuint Texture::getTextureHandle()
	{
		return m_TextureHandle;
	}
#else
	ID3D11ShaderResourceView* Texture::getTexture()
	{
		return m_TextureView;
	}

	ID3D11SamplerState* Texture::getSampler()
	{
		return m_sampler;
	}
#endif
	unsigned int Texture::getId()
	{
		return m_Id;
	}
}
