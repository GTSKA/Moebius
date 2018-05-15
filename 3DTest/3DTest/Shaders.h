#pragma once
#include <d3d11.h>
#include <D3D11Shader.h>
struct ShaderVariable
{
	char Name[50];
	D3D11_SHADER_VARIABLE_DESC Desc;
};
class Shaders
{
public:
	Shaders();
	~Shaders();
	unsigned int getId();
	int Init(char* fileVertexShader, char* filePixelShader, ID3D11Device* dev, ID3D11DeviceContext* devcon);
	int Init(char* fileVertexShader, char* filePixelShader, ID3D11Device* dev, ID3D11DeviceContext* devcon, unsigned int id);
	ID3D11VertexShader* getVertexShader();
	ID3D11PixelShader* getPixelShader();
	ID3D11Buffer* getVertexConstBufferbyIndex(unsigned int index);
	unsigned int getVSinputsize();
	void Clean();
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
	UINT uWVP;
	UINT uworldMatrix;
	UINT ufogRange;
	UINT ufogStart;
	UINT ufogColor;
	UINT u_camPos;
	UINT uTilingFactor;

	UINT* u_s2DTextures;
};

