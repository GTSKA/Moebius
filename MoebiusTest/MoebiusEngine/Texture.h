#pragma once
#ifdef OPENGL_PROJECT
#include "OpenGLClass.h"
#else
class ID3D11Texture2D;
class ID3D11SamplerState;
class ID3D11ShaderResourceView;
class ID3D11Device;
class ID3D11DeviceContext;
#endif
namespace MoebiusEngine
{
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
#ifdef OPENGL_PROJECT
		Texture(OpenGLClass* OpenGL);
		bool Init(char* textureName);
		bool Init(char* textureName, unsigned int Id);
		bool InitCubeTexture(char* textureName);
		bool InitCubeTexture(char* textureName, unsigned int Id);
		GLuint getTextureHandle();
		
#else
		bool Init(char* textureName, ID3D11Device* dev, ID3D11DeviceContext* devcon);
		bool Init(char* textureName, unsigned int Id, ID3D11Device* dev, ID3D11DeviceContext* devcon);
		bool InitCubeTexture(char* textureName, ID3D11Device* dev, ID3D11DeviceContext* devcon);
		bool InitCubeTexture(char* textureName, unsigned int Id, ID3D11Device* dev, ID3D11DeviceContext* devcon);
		ID3D11ShaderResourceView* getTexture();
		ID3D11SamplerState* getSampler();
#endif
		void setWrapMode(WRAPMODE imode);
		void shutdown();
		
		unsigned int getId();

	protected:
		int m_width;
		int m_height;
		int m_bpp;
		WRAPMODE m_tiling;
#ifdef OPENGL_PROJECT
		OpenGLClass* m_OpenGL;
		GLuint m_TextureHandle;
		bool ExtractFace(int hOffset, int vOffset, const char* bufferTGA);
#else
		ID3D11Texture2D *m_Texture;
		ID3D11SamplerState* m_sampler;
		ID3D11ShaderResourceView* m_TextureView;
		bool ExtractFace(int hOffset, int vOffset, const char* bufferTGA, char** face);
#endif
		unsigned int m_Id;
		
	};
}

