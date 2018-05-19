#include "FrameBufferObject.h"
#include "Quad.h"
#include "Globals.h"
#include <D3D11.h>


FrameBufferObject::FrameBufferObject()
{
}


FrameBufferObject::~FrameBufferObject()
{
	//m_DepthTexture->Release();
	//m_Framebuffer->Release();
	//m_ColorTexture->Release();
	//m_ColorTexture->;
	//m_ColorRenderTargetView;
	//m_colorShaderResView;
	if (m_ShaderResView)
	{
		m_ShaderResView->Release();
		m_colorShaderResView = 0;
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
	if(isDepth)
		textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	else
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	if(isDepth)
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	else
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (isDepth)
		result = dev->CreateTexture2D(&textureDesc, NULL, &m_DepthTexture);
	else
		result = dev->CreateTexture2D(&textureDesc, NULL, &m_ColorTexture);
	if (FAILED(result))
		return (isDepth)? FBO_DEPTHTEX_ERROR:FBO_COLORTEX_ERROR;
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
	}
	else
	{
		// Setup the description of the render target view.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		result = dev->CreateRenderTargetView(m_ColorTexture, &renderTargetViewDesc, &m_ColorRenderTargetView);
		if (FAILED(result))
			return FBO_INCOMPLETE;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	// Setup the description of the shader resource view.
	if (isDepth)
		shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	else
		shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	if (isDepth)
		result = dev->CreateShaderResourceView(m_DepthTexture, &shaderResourceViewDesc, &m_ShaderResView);
	else
		result = dev->CreateShaderResourceView(m_ColorTexture, &shaderResourceViewDesc, &m_ShaderResView);

	return FBO_SUCCESS;
}
ID3D11RenderTargetView* FramebufferObject::GetColorTexture()
{
	return m_ColorRenderTargetView;
}
ID3D11DepthStencilView* FramebufferObject::GetDepthTexture()
{
	return m_DepthStencilView;
}