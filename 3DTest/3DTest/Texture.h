#pragma once
class ID3D11Texture2D;
class ID3D11SamplerState;
class ID3D11ShaderResourceView;
class ID3D11Device;
class ID3D11DeviceContext;
static enum WRAPMODE
{
	REPEAT = 1,
	CLAMP_TO_EDGE = 2,
	MIRRORED_REPEAT = 3
};
class Texture
{
public:
	Texture();
	~Texture();
	bool Init(char* textureName, ID3D11Device* dev, ID3D11DeviceContext* devcon);
	bool Init(char* textureName, unsigned int Id, ID3D11Device* dev, ID3D11DeviceContext* devcon);
	void setWrapMode(WRAPMODE imode);
	ID3D11ShaderResourceView* getTexture();
	ID3D11SamplerState* getSampler();
	//ID3D11ShaderResourceView* getTextureView();
	
	unsigned int getId();
protected:
	int m_width;
	int m_height;
	int m_bpp;
	WRAPMODE m_tiling;
	ID3D11Texture2D *m_Texture;
	unsigned int m_Id;
	ID3D11SamplerState* m_sampler;
	ID3D11ShaderResourceView* m_TextureView;
};

