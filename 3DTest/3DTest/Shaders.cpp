#include <D3Dcompiler.h>
#include <d3d11.h>
#include <D3D11Shader.h>
#include <stdio.h>
#include <tchar.h>
#include "Shaders.h"
#include "MathUtil.h"
#include "Globals.h"

Shaders::Shaders()
{
	m_Id = 0;
	positionAttribute = -1;
	colorAttribute = -1;
	uvAttribute = -1;
	normalAttribute = -1;
	binormalAttribute = -1;
	tangentAttribute = -1;
	uWVP.constantbuffer = -1;
	uworldMatrix.constantbuffer = -1;
	uTilingFactor.constantbuffer = -1;
	ufogRange.constantbuffer = -1;
	ufogStart.constantbuffer = -1;
	ufogColor.constantbuffer = -1;
	ucamPos.constantbuffer = -1;
	uTime.constantbuffer = -1;
	uFireAmp.constantbuffer = -1;
	uWaterColor.constantbuffer = -1;
	uFresnelPower.constantbuffer = -1;
	uDepthAdjust.constantbuffer = -1;
	uDepthAdjustDisplacement.constantbuffer = -1;
	uMaxReflection.constantbuffer = -1;

	uAmbientColor.constantbuffer = -1;
	uAmbientWeight.constantbuffer = -1;
	uSpecularPower.constantbuffer = -1;
	uNumLights.constantbuffer = -1;
	u2DTexturesCount = 0;
	uCubeTexturesCount = 0;
	m_BlendState = NULL;
	uLightTypes = NULL;
	uPosDirs = NULL;
	uLightColors = NULL;
}


Shaders::~Shaders()
{
	delete[] m_vertexConstantBuffer;
	delete[] m_pixelConstantBuffer;
	VertexShaderReflector->Release();
	PixelShaderReflector->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	for (int i = 0; i < m_numVConstBuffer; ++i)
		m_vertexConstantBuffer[i]->Release();
	delete[] m_vertexConstantBuffer;
	for (int i = 0; i < m_numPConstBuffer; ++i)
		m_pixelConstantBuffer[i]->Release();
	delete[] m_pixelConstantBuffer;
	if (uLightTypes)
	{
		delete[] uLightTypes;
		uLightTypes = NULL;
	}
	if (uPosDirs)
	{
		delete[] uPosDirs;
		uPosDirs = NULL;
	}
	if (uLightColors)
	{
		delete[] uLightColors;
		uLightColors = NULL;
	}
	if (m_BlendState)
		m_BlendState->Release();
	m_RasterizerState->Release();
	pLayout->Release();
}
void Shaders::Clean()
{
	
}

void Shaders::EnableStates(ID3D11DeviceContext* devcon)
{	
	devcon->RSSetState(m_RasterizerState);
	if (m_BlendState)
		devcon->OMSetBlendState(m_BlendState, 0, 0xffffff);
}

void Shaders::DisableStates(ID3D11DeviceContext* devcon)
{
	
}

bool Shaders::InitRasterizerState(D3D11_RASTERIZER_DESC* rasterizerDesc, ID3D11Device* dev)
{
	return !FAILED(dev->CreateRasterizerState(rasterizerDesc, &m_RasterizerState));
}
void Shaders::InitBlendState(ID3D11Device* dev)
{
	D3D11_BLEND_DESC blendDesc;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.AlphaToCoverageEnable = FALSE;
	HRESULT hr = dev->CreateBlendState(&blendDesc, &m_BlendState);
	if (FAILED(hr))
		OutputDebugString("Falla al crear BlendState");
}
int Shaders::Init(char* fileVertexShader, char* filePixelShader, ID3D11Device* dev)
{
	if (fileVertexShader == NULL)
		return -1;
	if (filePixelShader == NULL)
		return -2;
	ID3DBlob* VertexShaderBlob;
	ID3DBlob* PixelShaderBlob;

	if (LoadShader(1, fileVertexShader, &VertexShaderBlob) < 0)
		return -3;
	if (LoadShader(2, filePixelShader, &PixelShaderBlob) < 0)
		return -4;
	dev->CreateVertexShader(VertexShaderBlob->GetBufferPointer(), VertexShaderBlob->GetBufferSize(), NULL, &m_VertexShader);
	dev->CreatePixelShader(PixelShaderBlob->GetBufferPointer(), PixelShaderBlob->GetBufferSize(), NULL, &m_PixelShader);

	//devcon->VSSetShader(m_VertexShader, 0, 0);//moved to Object class draw function
	//devcon->PSSetShader(m_PixelShader, 0, 0);//moved to Object class draw function

	if(FAILED(D3DReflect(VertexShaderBlob->GetBufferPointer(), VertexShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&VertexShaderReflector)))
		return -5;
	if (FAILED(D3DReflect(PixelShaderBlob->GetBufferPointer(), PixelShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&PixelShaderReflector)))
		return -6;

	D3D11_INPUT_ELEMENT_DESC *layout;

	D3D11_SHADER_DESC vertexShaderDesc;
	VertexShaderReflector->GetDesc(&vertexShaderDesc);
	numVSinput = vertexShaderDesc.InputParameters;
	layout = new D3D11_INPUT_ELEMENT_DESC[numVSinput];
	//int offset = 0;
	for (int i = 0; i < numVSinput; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC  resourceBindDesc;
		VertexShaderReflector->GetInputParameterDesc(i, &resourceBindDesc);
		//OutputDebugString(resourceBindDesc.SemanticName);
		layout[i].SemanticName = resourceBindDesc.SemanticName;
		layout[i].SemanticIndex = resourceBindDesc.SemanticIndex;
		layout[i].InputSlot = i;
		layout[i].AlignedByteOffset = 0;// offset;
		layout[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		layout[i].InstanceDataStepRate = 0;
		if(strcmp(resourceBindDesc.SemanticName, "POSITION") == 0)
		{
			//offset += 12;
			positionAttribute = i;
		}
		if (strcmp(resourceBindDesc.SemanticName, "COLOR") == 0)
		{
			//offset += 16;
			colorAttribute = i;
		}
		if (strcmp(resourceBindDesc.SemanticName, "TEXCOORD") == 0)
		{
			//offset += 8;
			uvAttribute = i;
		}
		if (strcmp(resourceBindDesc.SemanticName, "NORMAL") == 0)
		{
			normalAttribute = i;
		}
		if (strcmp(resourceBindDesc.SemanticName, "BINORMAL") == 0)
		{
			binormalAttribute = i;
		}
		if (strcmp(resourceBindDesc.SemanticName, "TANGENT") == 0)
		{
			tangentAttribute = i;
		}

		
		if (resourceBindDesc.Mask == 1)
		{
			if (resourceBindDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) layout[i].Format = DXGI_FORMAT_R32_UINT;
			else if (resourceBindDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) layout[i].Format = DXGI_FORMAT_R32_SINT;
			else if (resourceBindDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) layout[i].Format = DXGI_FORMAT_R32_FLOAT;
			//offset += 4;
		}
		else if (resourceBindDesc.Mask <= 3)
		{
			if (resourceBindDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) layout[i].Format = DXGI_FORMAT_R32G32_UINT;
			else if (resourceBindDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) layout[i].Format = DXGI_FORMAT_R32G32_SINT;
			else if (resourceBindDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) layout[i].Format = DXGI_FORMAT_R32G32_FLOAT;
			//offset += 8;
		}
		else if (resourceBindDesc.Mask <= 7)
		{
			if (resourceBindDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) layout[i].Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (resourceBindDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) layout[i].Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (resourceBindDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) layout[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			//offset += 12;
		}
		else if (resourceBindDesc.Mask <= 15)
		{
			if (resourceBindDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) layout[i].Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (resourceBindDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) layout[i].Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (resourceBindDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) layout[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			//offset += 16;
		}
	}
	
	dev->CreateInputLayout(layout, numVSinput, VertexShaderBlob->GetBufferPointer(), VertexShaderBlob->GetBufferSize(), &pLayout);
	
	delete[] layout;

	m_numVConstBuffer = vertexShaderDesc.ConstantBuffers;
	m_vertexConstantBuffer = new ID3D11Buffer*[vertexShaderDesc.ConstantBuffers];
	HRESULT hr = S_OK;
	for (int i = 0; i < vertexShaderDesc.ConstantBuffers; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* vertexConstantBuffer = VertexShaderReflector->GetConstantBufferByIndex(i);

		D3D11_SHADER_BUFFER_DESC vertexShaderBufferDesc;
		vertexConstantBuffer->GetDesc(&vertexShaderBufferDesc);

		D3D11_BUFFER_DESC vertexConstantBufferDesc;
		ZeroMemory(&vertexConstantBufferDesc, sizeof(vertexConstantBufferDesc));

		vertexConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexConstantBufferDesc.ByteWidth = vertexShaderBufferDesc.Size;
		vertexConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		vertexConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		hr = dev->CreateBuffer(&vertexConstantBufferDesc, NULL, &m_vertexConstantBuffer[i]);
		
		if (FAILED(hr))
			return -4;
		// Not needed right now
		for (int j = 0; j < vertexShaderBufferDesc.Variables; ++j)
		{
			ID3D11ShaderReflectionVariable* Variable = vertexConstantBuffer->GetVariableByIndex(j);
			D3D11_SHADER_VARIABLE_DESC variableDesc;
			Variable->GetDesc(&variableDesc);
			OutputDebugString(variableDesc.Name);
			if (strcmp(variableDesc.Name, "WVPMatrix") == 0)
			{
				uWVP.constantbuffer = i;
				uWVP.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "wMatrix") == 0)
			{
				uworldMatrix.constantbuffer = i;
				uworldMatrix.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "TilingFactor") == 0)
			{
				uTilingFactor.constantbuffer = i;
				uTilingFactor.offset = variableDesc.StartOffset;
			}
		}

	}

	D3D11_SHADER_DESC pixelShaderDesc;
	PixelShaderReflector->GetDesc(&pixelShaderDesc);
	m_numPConstBuffer = pixelShaderDesc.ConstantBuffers;
	m_pixelConstantBuffer = new ID3D11Buffer*[m_numPConstBuffer];
	for (int i = 0; i < pixelShaderDesc.ConstantBuffers; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* pixelConstantBuffer = PixelShaderReflector->GetConstantBufferByIndex(i);

		D3D11_SHADER_BUFFER_DESC pixelShaderBufferDesc;
		pixelConstantBuffer->GetDesc(&pixelShaderBufferDesc);

		D3D11_BUFFER_DESC pixelConstantBufferDesc;
		ZeroMemory(&pixelConstantBufferDesc, sizeof(pixelConstantBufferDesc));
		pixelConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		pixelConstantBufferDesc.ByteWidth = pixelShaderBufferDesc.Size;
		pixelConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		pixelConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		dev->CreateBuffer(&pixelConstantBufferDesc, NULL, &m_pixelConstantBuffer[i]);

		for (int j = 0; j < pixelShaderBufferDesc.Variables; ++j)
		{
			ID3D11ShaderReflectionVariable* variable = pixelConstantBuffer->GetVariableByIndex(j);

			D3D11_SHADER_VARIABLE_DESC variableDesc;
			variable->GetDesc(&variableDesc);

			//logMsg("%s\n", variableDesc.Name);
			OutputDebugString(variableDesc.Name);
			if (strcmp(variableDesc.Name, "ufogColor") == 0)
			{
				ufogColor.constantbuffer = i;
				ufogColor.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "ufogStart") == 0)
			{
				ufogStart.constantbuffer = i;
				ufogStart.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "ufogRange") == 0)
			{
				ufogRange.constantbuffer = i;
				ufogRange.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "ucamPos") == 0)
			{
				ucamPos.constantbuffer = i;
				ucamPos.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "uTime") == 0)
			{
				uTime.constantbuffer = i;
				uTime.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "uFireAmp") == 0)
			{
				uFireAmp.constantbuffer = i;
				uFireAmp.offset = variableDesc.StartOffset;
			}

			if (strcmp(variableDesc.Name, "uWaterColor") == 0)
			{
				uWaterColor.constantbuffer = i;
				uWaterColor.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "uFresnelPower") == 0)
			{
				uFresnelPower.constantbuffer = i;
				uFresnelPower.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "uDepthAdjust") == 0)
			{
				uDepthAdjust.constantbuffer = i;
				uDepthAdjust.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "uDepthAdjustDisplacement") == 0)
			{
				uDepthAdjustDisplacement.constantbuffer = i;
				uDepthAdjustDisplacement.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "uMaxReflection") == 0)
			{
				uMaxReflection.constantbuffer = i;
				uMaxReflection.offset = variableDesc.StartOffset;
			}

			if(strcmp(variableDesc.Name, "uAmbientColor") == 0)
			{
				uAmbientColor.constantbuffer = i;
				uAmbientColor.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "uAmbientWeight") == 0)
			{
				uAmbientWeight.constantbuffer = i;
				uAmbientWeight.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "uSpecularPower") == 0)
			{
				uSpecularPower.constantbuffer = i;
				uSpecularPower.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "uNumLights") == 0)
			{
				uNumLights.constantbuffer = i;
				uNumLights.offset = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "uLightTypes") == 0)
			{
				uLightTypes = new ShaderConstVar[Globals::maxLightCount];
				for (int j = 0; j < Globals::maxLightCount; ++j)
				{
					uLightTypes[j].constantbuffer = i;
					uLightTypes[j].offset = variableDesc.StartOffset + j * sizeof(int);
				}
			}
			if (strcmp(variableDesc.Name, "uPosDirs") == 0)
			{
				uPosDirs = new ShaderConstVar[Globals::maxLightCount];
				for (int j = 0; j < Globals::maxLightCount; ++j)
				{
					uPosDirs[j].constantbuffer = i;
					uPosDirs[j].offset = variableDesc.StartOffset + j * 3 * sizeof(float);
				}
			}
			if (strcmp(variableDesc.Name, "uLightColors") == 0)
			{
				uLightColors = new ShaderConstVar[Globals::maxLightCount];
				for (int j = 0; j < Globals::maxLightCount; ++j)
				{
					uLightColors[j].constantbuffer = i;
					uLightColors[j].offset = variableDesc.StartOffset + j * 3 * sizeof(float);
				}
			}
			
			
		}

	}

	for (int i = 0; i < pixelShaderDesc.BoundResources; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC resourceBindDesc;
		PixelShaderReflector->GetResourceBindingDesc(i, &resourceBindDesc);
		OutputDebugString(resourceBindDesc.Name);
		if (strcmp(resourceBindDesc.Name, "Texture") == 0)
		{
			u2DTexturesCount = resourceBindDesc.BindCount;
		}
		if(strcmp(resourceBindDesc.Name, "SkyMap") == 0)
		{
			uCubeTexturesCount = resourceBindDesc.BindCount;
		}
		if (strcmp(resourceBindDesc.Name, "samplerDD") == 0)
		{
			samplers2DSlot = resourceBindDesc.BindPoint;
		}
		if (strcmp(resourceBindDesc.Name, "samplerCube") == 0)
		{
			SamplersCubeSlot = resourceBindDesc.BindPoint;
		}
	}

	
	return 0;
}
ID3D11InputLayout* Shaders::getVertexBufferLayout()
{
	return pLayout;
}
unsigned int Shaders::getVSinputsize()
{
	return numVSinput;
}
UINT Shaders::getNumVertexConstBuffer()
{
	return m_numVConstBuffer;
}
ID3D11Buffer* Shaders::getVertexConstBufferbyIndex(unsigned int index)
{
	if (index >= m_numVConstBuffer)
		return NULL;
	return m_vertexConstantBuffer[index];
}
UINT Shaders::getNumPixelConstBuffer()
{
	return m_numPConstBuffer;
}
ID3D11Buffer* Shaders::getPixelConstBufferbyIndex(unsigned int index)
{
	if (index >= m_numPConstBuffer)
		return NULL;
	return m_pixelConstantBuffer[index];
}
int Shaders::Init(char* fileVertexShader, char* filePixelShader, ID3D11Device* dev, unsigned int id)
{
	m_Id = id;
	return Init(fileVertexShader, filePixelShader, dev);
}
unsigned int Shaders::getId()
{
	return m_Id;
}

int Shaders::LoadShader(unsigned int Type, char* filename, ID3DBlob** shaderBlob)
{
	*shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// Load the shader source
	FILE * pf;
	if (fopen_s(&pf, filename, "rb") != 0)
		return NULL;
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char * shaderSrc = new char[size + 1];
	fread(shaderSrc, sizeof(char), size, pf);
	shaderSrc[size] = 0;
	fclose(pf);
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif
	HRESULT hr;
	if (Type == 1)
		hr = D3DCompile(shaderSrc, size, NULL, NULL, NULL, "VShader", "vs_4_0", flags, 0, shaderBlob, &errorBlob);
	if(Type == 2)
		hr = D3DCompile(shaderSrc, size, NULL, NULL, NULL, "PShader", "ps_4_0", flags, 0, shaderBlob, &errorBlob);
	if (Type > 2)
		return -1;//TO DO:
	if (SUCCEEDED(hr))
	{
		if (errorBlob)
		{
			// The message is a 'warning' from the HLSL compiler.
			char const* message =
				static_cast<char const*>(errorBlob->GetBufferPointer());
			// Write the warning to the output window when the program is
			// executing through the Microsoft Visual Studio IDE.
			OutputDebugString(message);
			errorBlob->Release();
		}
		return 0;
	}
	else
	{
		if (errorBlob)
		{
			// The message is an 'error' from the HLSL compiler.
			char const* message =
				static_cast<char const*>(errorBlob->GetBufferPointer());
			// Write the warning to the output window when the program is
			// executing through the Microsoft Visual Studio IDE.
			OutputDebugString(message);
			errorBlob->Release();
		}
		return -1;
	}
}

ID3D11VertexShader* Shaders::getVertexShader()
{
	return m_VertexShader;
}
ID3D11PixelShader* Shaders::getPixelShader() 
{
	return m_PixelShader;
}


/*//////////////////////////////////////////////////PASSING VALUES TO CONSTANT BUFFERS/////////////////////////////////////////////////////////////////////*/

void Uniform1f(void* pData, UINT offset, float value)
{
	char* dst = (char*)pData;
	dst += offset;
	memcpy(dst, &value, sizeof(float));
}
void Uniform2f(void* pData, UINT offset, float value1, float value2)
{
	float val[2] = { value1 ,value2 };
	char* dst = (char*)pData;
	dst += offset;
	memcpy(dst, val, 2*sizeof(float));
}
void Uniform3f(void* pData, UINT offset, float value1, float value2, float value3)
{
	float val[3] = { value1 ,value2, value3};
	char* dst = (char*)pData;
	dst += offset;
	memcpy(dst, val, 3*sizeof(float));
}
void Uniform4f(void* pData, UINT offset, float value1, float value2, float value3, float value4)
{
	float val[4] = { value1 ,value2, value3, value4 };
	char* dst = (char*)pData;
	dst += offset;
	memcpy(dst, val, 4*sizeof(float));
}

void Uniform1i(void* pData, UINT offset, int value)
{
	char* dst = (char*)pData;
	dst += offset;
	memcpy(dst, &value, sizeof(int));
}
void Uniform2i(void* pData, UINT offset, int value1, int value2)
{
	int val[2] = { value1 ,value2 };
	char* dst = (char*)pData;
	dst += offset;
	memcpy(dst, val, 2 * sizeof(int));
}
void Uniform3i(void* pData, UINT offset, int value1, int value2, int value3)
{
	int val[3] = { value1 ,value2, value3 };
	char* dst = (char*)pData;
	dst += offset;
	memcpy(dst, val, 3 * sizeof(int));
}
void Uniform4i(void* pData, UINT offset, int value1, int value2, int value3, int value4)
{
	int val[4] = { value1 ,value2, value3, value4 };
	char* dst = (char*)pData;
	dst += offset;
	memcpy(dst, val, 4 * sizeof(int));
}

void UniformMatrix4f(void* pData, UINT offset, Matrix *m)
{
	char* dst = (char*)pData;
	dst += offset;
	memcpy(dst, &(m->m[0][0]), 16 * sizeof(float));
}

void Uniform2fv(void* pData, UINT offset, Vector2 value)
{
	char* dst = (char*)pData;
	dst += offset;
	memcpy(dst, &value, 2 * sizeof(int));
}
void Uniform3fv(void* pData, UINT offset, Vector3 value)
{
	char* dst = (char*)pData;
	dst += offset;
	memcpy(dst, &value, 3 * sizeof(int));
}
void Uniform4fv(void* pData, UINT offset, Vector4 value)
{
	char* dst = (char*)pData;
	dst += offset;
	memcpy(dst, &value, 4 * sizeof(int));
}