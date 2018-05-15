#include "Shaders.h"
#include <D3Dcompiler.h>

#include <stdio.h>
#include <malloc.h>
#include <tchar.h>


Shaders::Shaders()
{
	m_Id = 0;
	positionAttribute = -1;
	colorAttribute = -1;
	uvAttribute = -1;
	uWVP = -1;
	uworldMatrix = -1;
	uTilingFactor = -1;
	ufogRange = -1;
	ufogStart = -1;
	ufogColor = -1;
	u_camPos = -1;
	//u_s2DTextures = -1;
}


Shaders::~Shaders()
{
	delete[] m_vertexConstantBuffer;
	delete[] m_pixelConstantBuffer;
	VertexShaderReflector->Release();
	PixelShaderReflector->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}
void Shaders::Clean()
{
	delete[] u_s2DTextures;
}
int Shaders::Init(char* fileVertexShader, char* filePixelShader, ID3D11Device* dev, ID3D11DeviceContext* devcon)
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
	devcon->IASetInputLayout(pLayout);
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
		devcon->VSSetConstantBuffers(0, 1, &m_vertexConstantBuffer[i]);
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
				uWVP = j;
			}
			if (strcmp(variableDesc.Name, "wMatrix") == 0)
			{
				uworldMatrix = j;
			}
			if (strcmp(variableDesc.Name, "TilingFactor") == 0)
			{
				uTilingFactor = j;
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
				ufogColor = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "ufogStart") == 0)
			{
				ufogStart = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "ufogRange") == 0)
			{
				ufogRange = variableDesc.StartOffset;
			}
			if (strcmp(variableDesc.Name, "ucamPos") == 0)
			{
				u_camPos = variableDesc.StartOffset;
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
			u_s2DTextures = new UINT[resourceBindDesc.BindCount];
			//uTexture = i;
		}
		//p.m_pixelBindings[resourceBindDesc.Name] = resourceBindDesc;
	}

	
	return 0;
}
unsigned int Shaders::getVSinputsize()
{
	return numVSinput;
}
ID3D11Buffer* Shaders::getVertexConstBufferbyIndex(unsigned int index)
{
	if (index >= m_numVConstBuffer)
		return NULL;
	return m_vertexConstantBuffer[index];
}
int Shaders::Init(char* fileVertexShader, char* filePixelShader, ID3D11Device* dev, ID3D11DeviceContext* devcon, unsigned int id)
{
	m_Id = id;
	return Init(fileVertexShader, filePixelShader, dev, devcon);
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