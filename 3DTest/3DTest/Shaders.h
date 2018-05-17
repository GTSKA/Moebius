#pragma once

class ID3D11VertexShader;
class ID3D11PixelShader;
class ID3D11Buffer;
class ID3D11Device;
class ID3D11ShaderReflection;
class ID3D11InputLayout;

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
protected:
	unsigned int m_Id;
	int LoadShader(unsigned int Type, char* filename, ID3DBlob** shaderBlob);
	
	ID3D11VertexShader *m_VertexShader;    // the vertex shader
	ID3D11PixelShader *m_PixelShader;     // the pixel shader
	ID3D11InputLayout *pLayout;
	unsigned int numVSinput;
	ID3D11ShaderReflection* VertexShaderReflector;
	ID3D11ShaderReflection* PixelShaderReflector;
	ID3D11Buffer** m_vertexConstantBuffer;
	unsigned int m_numVConstBuffer;
	ID3D11Buffer** m_pixelConstantBuffer;
	unsigned int m_numPConstBuffer;
	//D3D11_SHADER_VARIABLE_DESC m_variableDesc;
	//ShaderVariable* m_vertexVariables;
	//ShaderVariable* m_pixelVariables;
public:
	UINT positionAttribute;
	UINT colorAttribute;
	UINT uvAttribute;
	ShaderConstVar uWVP;
	ShaderConstVar uworldMatrix;
	ShaderConstVar ufogRange;
	ShaderConstVar ufogStart;
	ShaderConstVar ufogColor;
	ShaderConstVar ucamPos;
	ShaderConstVar uTilingFactor;

	UINT u2DTexturesCount;
	UINT uCubeTexturesCount;
};

