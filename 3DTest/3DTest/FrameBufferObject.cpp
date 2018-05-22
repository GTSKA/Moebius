#include "FrameBufferObject.h"
#include "Quad.h"
#include "Globals.h"
#include <d3d11.h>


FramebufferObject::FramebufferObject()
{
	m_ShaderColorResView = NULL;
	m_ShaderDepthResView = NULL;
	m_DepthTexture = NULL;
	m_DepthStencilView = NULL;
	m_ColorRenderTargetView = NULL;
	m_ColorTexture = NULL;
	m_ColorSampler = NULL;
	m_DepthSampler = NULL;
}


FramebufferObject::~FramebufferObject()
{
	//m_DepthTexture->Release();
	//m_Framebuffer->Release();
	//m_ColorTexture->Release();
	//m_ColorTexture->;
	//m_ColorRenderTargetView;
	//m_colorShaderResView;
	if (m_ShaderColorResView)
	{
		m_ShaderColorResView->Release();
		m_ShaderColorResView = 0;
	}
	if (m_ShaderDepthResView)
	{
		m_ShaderDepthResView->Release();
		m_ShaderDepthResView = 0;
	}
	
	if (m_DepthTexture)
	{
		m_DepthTexture->Release();
		m_DepthTexture = 0;
	}
	if (m_DepthStencilView)
	{
		m_DepthStencilView->Release();
		m_DepthStencilView = 0;
	}
	if (m_ColorRenderTargetView)
	{
		m_ColorRenderTargetView->Release();
		m_ColorRenderTargetView = 0;
	}

	if (m_ColorTexture)
	{
		m_ColorTexture->Release();
		m_ColorTexture = 0;
	}

	if (m_ColorSampler)
	{
		m_ColorSampler->Release();
		m_ColorSampler = 0;
	}
	if (m_DepthSampler)
	{
		m_DepthSampler->Release();
		m_DepthSampler = 0;
	}
}

FBOERROR FramebufferObject::Init(ID3D11Device* dev, UINT textureWidth, UINT textureHeight, bool isDepth)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	
	HRESULT result;
	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	// Setup the render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;		
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	if (isDepth)
	{
		textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		result = dev->CreateTexture2D(&textureDesc, NULL, &m_DepthTexture);
		if (FAILED(result))
			return FBO_DEPTHTEX_ERROR;
	}
	
	{
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		result = dev->CreateTexture2D(&textureDesc, NULL, &m_ColorTexture);
		if (FAILED(result))
			return FBO_COLORTEX_ERROR;
	}
		
	
	if (isDepth)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		result = dev->CreateDepthStencilView(m_DepthTexture, &depthStencilViewDesc, &m_DepthStencilView);
		if (FAILED(result))
			return FBO_INCOMPLETE;
		if (!CreateTexture(dev, isDepth))
		{
			return FBO_DEPTHTEX_ERROR;
		}
	}
	
	{
		// Setup the description of the render target view.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		result = dev->CreateRenderTargetView(m_ColorTexture, &renderTargetViewDesc, &m_ColorRenderTargetView);
		if (FAILED(result))
			return FBO_INCOMPLETE;
		if (!CreateTexture(dev))
		{
			return FBO_COLORTEX_ERROR;
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	// Setup the description of the shader resource view.
	if (isDepth)
	{
		shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		result = dev->CreateShaderResourceView(m_DepthTexture, &shaderResourceViewDesc, &m_ShaderDepthResView);
		if (FAILED(result))
			return FBO_INCOMPLETE;
	}
		
	{
		shaderResourceViewDesc.Format = textureDesc.Format;
		result = dev->CreateShaderResourceView(m_ColorTexture, &shaderResourceViewDesc, &m_ShaderColorResView);
		if (FAILED(result))
			return FBO_INCOMPLETE;
	}
	
	
	
		

	return FBO_SUCCESS;
}
bool FramebufferObject::CreateTexture(ID3D11Device* dev, bool isDepth)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxAnisotropy = 8;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MipLODBias = 2.0f;
	HRESULT hr;
	if(isDepth)
		hr = dev->CreateSamplerState(&samplerDesc, &m_DepthSampler);
	else
		hr = dev->CreateSamplerState(&samplerDesc, &m_ColorSampler);
	if (FAILED(hr))
		return false;
	return true;
}
ID3D11ShaderResourceView* FramebufferObject::GetColorTexture()
{
	return m_ShaderColorResView;
}
ID3D11ShaderResourceView* FramebufferObject::GetDepthTexture()
{
	return m_ShaderDepthResView;
}
ID3D11SamplerState* FramebufferObject::GetColorSampler()
{
	return m_ColorSampler;
}
ID3D11SamplerState* FramebufferObject::GetDepthSampler()
{
	return m_DepthSampler;
}
ID3D11RenderTargetView* FramebufferObject::GetRenderTarget()
{
	return m_ColorRenderTargetView;
}
ID3D11DepthStencilView* FramebufferObject::GetDepthStencil()
{
	return m_DepthStencilView;
}