#include "Texture.h"
#include "TGA.h"
#include <d3d11.h>


Texture::Texture()
{
	m_height = 0;
	m_width = 0;
	m_bpp = 0;
	m_Texture = 0;
	m_TextureView = 0;
	m_Id = 0;
}


Texture::~Texture()
{
	if(m_Texture)
		m_Texture->Release();
	if(m_TextureView)
		m_TextureView->Release();
}

bool Texture::Init(char* textureName, unsigned int Id, ID3D11Device* dev, ID3D11DeviceContext* devcon)
{
	m_Id = Id;
	return Init(textureName,dev, devcon);
}
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
			bufferTGA2[i * 4  + 1] = bufferTGA[i * 3 + 1];
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
	desc.Width = m_width/4;
	desc.Height = m_height/3;
	desc.MipLevels  = 1;
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
	for (int i = 5; i >=0;--i)
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
bool Texture::InitCubeTexture(char* textureName, unsigned int Id, ID3D11Device* dev, ID3D11DeviceContext* devcon)
{
	m_Id = Id;
	return InitCubeTexture(textureName, dev, devcon);
}
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

void Texture::setWrapMode(WRAPMODE imode)
{
	m_tiling = imode;
}


ID3D11ShaderResourceView* Texture::getTexture()
{
	return m_TextureView;
}

ID3D11SamplerState* Texture::getSampler()
{
	return m_sampler;
}
unsigned int Texture::getId()
{
	return m_Id;
}
/*ID3D11ShaderResourceView* Texture::getTextureView() 
{
	return m_TextureView;
}*/

