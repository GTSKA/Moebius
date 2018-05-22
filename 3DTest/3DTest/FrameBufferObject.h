#pragma once
class ID3D11Texture2D;
class ID3D11RenderTargetView;
class ID3D11ShaderResourceView;
class ID3D11DepthStencilView;
class ID3D11Device;
class ID3D11SamplerState;
#ifndef UINT
typedef unsigned int UINT;
#endif
static enum FBOERROR
{
	FBO_SUCCESS = 0,
	FBO_SIZE_ERROR = 1,
	FBO_INCOMPLETE = 2,
	FBO_DEPTHTEX_ERROR = 3,
	FBO_COLORTEX_ERROR
};

class FramebufferObject
{
public:
	FramebufferObject();
	~FramebufferObject();
	FBOERROR Init(ID3D11Device* dev, UINT textureWidth, UINT textureHeight, bool isDepth = false);
	//GLenum BindFBO();
	//bool UnbindFBO();
	//GLuint GetFrameBuffer();
	ID3D11ShaderResourceView* GetColorTexture();
	ID3D11ShaderResourceView* GetDepthTexture();
	ID3D11SamplerState* GetColorSampler();
	ID3D11SamplerState* GetDepthSampler();
	ID3D11RenderTargetView* GetRenderTarget();
	ID3D11DepthStencilView* GetDepthStencil();
	bool CreateTexture(ID3D11Device* dev, bool isDepth = false);
protected:
	//UINT* m_Framebuffer;
	ID3D11Texture2D* m_ColorTexture;
	ID3D11RenderTargetView* m_ColorRenderTargetView;
	ID3D11Texture2D* m_DepthTexture;
	ID3D11DepthStencilView* m_DepthStencilView;
	ID3D11ShaderResourceView* m_ShaderColorResView;
	ID3D11ShaderResourceView* m_ShaderDepthResView;
	ID3D11SamplerState* m_ColorSampler;
	ID3D11SamplerState* m_DepthSampler;
};


