#pragma once
class Shaders;
class FramebufferObject;
class Effect;
class Quad;
class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11RenderTargetView;
class ID3D11DepthStencilView;
class EffectManager
{
protected:
	EffectManager();
	~EffectManager();
	EffectManager(const EffectManager &);
	EffectManager& operator =(const EffectManager &);
public:
	static EffectManager* GetInstance();
	static void DestroyInstance();
	void Clean();
	bool Init(char* FileName, ID3D11Device* dev, ID3D11DeviceContext* devcon);
	void Draw(unsigned int iEffect, ID3D11DeviceContext* devcon);
	void PreDraw(bool activate, ID3D11DeviceContext* devcon);
	bool LoadShaders();
	void DeleteShaders();
	int Get_EffectCount();
	void setDefaultRenderTarget(ID3D11RenderTargetView* renderTarget);
	void setDefaultDepthStencil(ID3D11DepthStencilView* depthStencil);
	FramebufferObject* GetFBO(int index);
	Effect* GetEffects();
	Quad* GetQuads();
	Shaders* GetShader(unsigned int ShaderId);
	ID3D11RenderTargetView* getDefaultBuffer();
	ID3D11DepthStencilView* getDefaultStencil();
protected:
	static EffectManager* m_pInstance;

	int m_ShaderCount;
	Shaders* m_Shaders;

	int m_FBOCount;
	FramebufferObject* m_FBOs;

	int m_EffectCount;
	Effect* m_Effects;

	/*GLuint m_vertexBuffer;
	GLuint m_IndexBuffer;
	unsigned int m_numVertex;
	unsigned int m_numIndices;*/

	Quad* m_Quads;
	ID3D11RenderTargetView* m_defaultBuffer;	//pointer to the back buffer
	ID3D11DepthStencilView* m_defaultStencil;
};

