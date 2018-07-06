#pragma once
#ifndef _SHADER_H_
#define _SHADER_H_
#ifdef OPENGL_PROJECT
#include "OpenGLClass.h"
#else
class ID3D11VertexShader;
class ID3D11PixelShader;
class ID3D11Device;
class ID3D11ShaderReflection;
class ID3D11RasterizerState;
class ID3D11Buffer;
class ID3D11InputLayout;
class ID3D11DeviceContext;
class D3D11_RASTERIZER_DESC;
class ID3D11BlendState;
#ifndef ID3DBlob
class ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
#endif // !ID3DBlob
#endif
class Matrix;
class Vector2;
class Vector3;
class Vector4;
#ifndef UINT
typedef unsigned int UINT;
#endif

struct ShaderConstVar
{
	UINT constantbuffer = -1;
	UINT offset = 0;
};
namespace MoebiusEngine
{
	class Shader
	{
	public:
		Shader();
		~Shader();
	
		unsigned int getId();
		
#ifdef OPENGL_PROJECT
		Shader(OpenGLClass* OpenGL);
		int Init(char* fileVertexShader, char* filePixelShader);
		int Init(char* fileVertexShader, char* filePixelShader, unsigned int id);
#else
		int Init(char* fileVertexShader, char* filePixelShader, ID3D11Device* dev);
		int Init(char* fileVertexShader, char* filePixelShader, ID3D11Device* dev, unsigned int id);
		ID3D11VertexShader* getVertexShader();
		ID3D11PixelShader* getPixelShader();
		ID3D11Buffer* getVertexConstBufferbyIndex(unsigned int index);
#endif

		UINT getNumVertexConstBuffer();
#ifdef OPENGL_PROJECT
#else
		ID3D11Buffer* getPixelConstBufferbyIndex(unsigned int index);
#endif
		UINT getNumPixelConstBuffer();
		unsigned int getVSinputsize();
		void Clean();
#ifdef OPENGL_PROJECT
		void InitStates(int stateCount);
		void EnableStates();
		void DisableStates();
		void SetState(int index, GLuint state);
#else
		ID3D11InputLayout* getVertexBufferLayout();
		void EnableStates(ID3D11DeviceContext* devcon);
		void DisableStates(ID3D11DeviceContext* devcon);
		bool InitRasterizerState(D3D11_RASTERIZER_DESC* rasterizerDesc, ID3D11Device* dev);
		void InitBlendState(ID3D11Device* dev);
#endif
		void shutdown();
	protected:
		unsigned int m_Id;
#ifdef OPENGL_PROJECT
		int m_StateCount;
		GLuint* m_States;
		GLuint LoadShader(GLenum type, char * filename);
		GLuint LoadProgram(GLuint vertexShader, GLuint fragmentShader);
#else
		int LoadShader(unsigned int Type, char* filename, ID3DBlob** shaderBlob);
#endif

#ifdef OPENGL_PROJECT
		OpenGLClass* m_OpenGL;
		
#else
		ID3D11VertexShader *m_VertexShader;    // the vertex shader
		ID3D11PixelShader *m_PixelShader;     // the pixel shader
		ID3D11InputLayout *pLayout;
#endif
		UINT numVSinput;
#ifdef OPENGL_PROJECT
		
#else

		ID3D11ShaderReflection* VertexShaderReflector;
		ID3D11ShaderReflection* PixelShaderReflector;
		ID3D11BlendState* m_BlendState;
		ID3D11RasterizerState* m_RasterizerState;
		ID3D11Buffer** m_vertexConstantBuffer;
#endif
		UINT m_numVConstBuffer;
#ifdef OPENGL_PROJECT
#else
		ID3D11Buffer** m_pixelConstantBuffer;
#endif
		UINT m_numPConstBuffer;
	public:
#ifdef OPENGL_PROJECT
		GLuint m_program;
		GLuint m_VertexShader;
		GLuint m_FragmentShader;
		GLuint m_VertexArrayObject;

		GLint positionAttribute;
		GLint colorAttribute;
		GLint uvAttribute;
		GLint normalAttribute;
		GLint binormalAttribute;
		GLint tangentAttribute;
		GLint uWVP;
		GLint uworldMatrix;
		GLint ufogRange;
		GLint ufogStart;
		GLint ufogColor;
		GLint ucamPos;
		GLint uTilingFactor;
		GLint uTime;
		GLint uFireAmp;
		

		GLint uWaterColor;
		GLint uFresnelPower;
		GLint uDepthAdjust;
		GLint uDepthAdjustDisplacement;
		GLint uMaxReflection;

		GLint uStep;
		GLint uLimit;
		GLint uNear;
		GLint uFar;
		GLint uFade;
		GLint uClarity;
		
		GLint* uTexLoc;
		///Light parameters
		GLint uAmbientColor;
		GLint uAmbientWeight;
		GLint uSpecularPower;
		GLint uNumLights;
		GLint* uLightTypes;
		GLint* uPosDirs;
		GLint* uLightColors;
		///
		GLint uCubeTexture;
		GLint uTexture;
		
#else
		UINT positionAttribute;
		UINT colorAttribute;
		UINT uvAttribute;
		UINT normalAttribute;
		UINT binormalAttribute;
		UINT tangentAttribute;
		ShaderConstVar uWVP;
		ShaderConstVar uworldMatrix;

		ShaderConstVar ufogRange;
		ShaderConstVar ufogStart;
		ShaderConstVar ufogColor;
		ShaderConstVar ucamPos;
		ShaderConstVar uTilingFactor;
		ShaderConstVar uTime;
		ShaderConstVar uFireAmp;
		ShaderConstVar uAmbientColor;
		ShaderConstVar uAmbientWeight;
		ShaderConstVar uSpecularPower;
		ShaderConstVar uNumLights;

		ShaderConstVar uWaterColor;
		ShaderConstVar uFresnelPower;
		ShaderConstVar uDepthAdjust;
		ShaderConstVar uDepthAdjustDisplacement;
		ShaderConstVar uMaxReflection;

		ShaderConstVar uStep;
		ShaderConstVar uLimit;
		ShaderConstVar uNear;
		ShaderConstVar uFar;
		ShaderConstVar uFade;
		ShaderConstVar uClarity;

		ShaderConstVar* uLightTypes;
		ShaderConstVar* uPosDirs;
		ShaderConstVar* uLightColors;
#endif
		
		
		
		

		

		UINT u2DTexturesCount;
		UINT samplers2DSlot;
		UINT uCubeTexturesCount;
		UINT SamplersCubeSlot;
	};
#ifndef OPENGL_PROJECT
	void Uniform1f(void* pData, UINT offset, float value);
	void Uniform2f(void* pData, UINT offset, float value1, float value2);
	void Uniform3f(void* pData, UINT offset, float value1, float value2, float value3);
	void Uniform4f(void* pData, UINT offset, float value1, float value2, float value3, float value4);

	void Uniform2fv(void* pData, UINT offset, Vector2 value);
	void Uniform3fv(void* pData, UINT offset, Vector3 value);
	void Uniform4fv(void* pData, UINT offset, Vector4 value);

	void Uniform1i(void* pData, UINT offset, int value);
	void Uniform2i(void* pData, UINT offset, int value1, int value2);
	void Uniform3i(void* pData, UINT offset, int value1, int value2, int value3);
	void Uniform4i(void* pData, UINT offset, int value1, int value2, int value3, int value4);

	void UniformMatrix4f(void* pData, UINT offset, Matrix *m);
#endif
}

#endif