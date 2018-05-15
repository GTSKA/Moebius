#include "Texture.h"
#include "TGA.h"
#include <float.h>
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
	samplerDesc.MaxAnisotropy = 8;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MipLODBias = 2.0f;
	dev->CreateSamplerState(&samplerDesc, &m_sampler);

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

