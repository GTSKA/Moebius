#pragma once

class ID3D11VertexShader;
class ID3D11PixelShader;
class ID3D11Buffer;
class ID3D11Device;
class ID3D11ShaderReflection;
class ID3D11InputLayout;
class ID3D11RasterizerState;
class ID3D11DeviceContext;
class D3D11_MAPPED_SUBRESOURCE;
class Matrix;
class Vector2;
class Vector3;
class Vector4;


struct ShaderConstVar
{
	UINT constantbuffer = -1;
	UINT offset = 0;
};

class Shaders
{
public:
	Shaders();
	~Shaders();
	unsigned int getId();
	int Init(char* fileVertexShader, char* filePixelShader, ID3D11Device* dev);
	int Init(char* fileVertexShader, char* filePixelShader, ID3D11Device* dev, unsigned int id);
	ID3D11VertexShader* getVertexShader();
	ID3D11PixelShader* getPixelShader();
	ID3D11Buffer* getVertexConstBufferbyIndex(unsigned int index);
	UINT getNumVertexConstBuffer();
	ID3D11Buffer* getPixelConstBufferbyIndex(unsigned int index);
	UINT getNumPixelConstBuffer();
	unsigned int getVSinputsize();
	void Clean();
	ID3D11InputLayout* getVertexBufferLayout();
	void EnableStates(ID3D11DeviceContext* devcon);
	void DisableStates(ID3D11DeviceContext* devcon);
	bool InitRasterizerState(D3D11_RASTERIZER_DESC* rasterizerDesc, ID3D11Device* dev);
	void InitBlendState(ID3D11Device* dev);
protected:
	unsigned int m_Id;
	int LoadShader(unsigned int Type, char* filename, ID3DBlob** shaderBlob);
	
	ID3D11VertexShader *m_VertexShader;    // the vertex shader
	ID3D11PixelShader *m_PixelShader;     // the pixel shader
	ID3D11InputLayout *pLayout;
	UINT numVSinput;
	ID3D11ShaderReflection* VertexShaderReflector;
	ID3D11ShaderReflection* PixelShaderReflector;
	ID3D11BlendState* m_BlendState;
	ID3D11RasterizerState* m_RasterizerState;
	ID3D11Buffer** m_vertexConstantBuffer;
	UINT m_numVConstBuffer;
	ID3D11Buffer** m_pixelConstantBuffer;
	UINT m_numPConstBuffer;
	//D3D11_SHADER_VARIABLE_DESC m_variableDesc;
	//ShaderVariable* m_vertexVariables;
	//ShaderVariable* m_pixelVariables;
public:
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
	ShaderConstVar* uLightTypes;
	ShaderConstVar* uPosDirs;
	ShaderConstVar* uLightColors;

	ShaderConstVar uWaterColor;
	ShaderConstVar uFresnelPower;
	ShaderConstVar uDepthAdjust;
	ShaderConstVar uDepthAdjustDisplacement;
	ShaderConstVar uMaxReflection;

	UINT u2DTexturesCount;
	UINT samplers2DSlot;
	UINT uCubeTexturesCount;
	UINT SamplersCubeSlot;
};

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


