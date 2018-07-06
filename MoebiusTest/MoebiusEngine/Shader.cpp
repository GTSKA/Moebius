#include "Shader.h"
#include <D3Dcompiler.h>
#include <d3d11.h>
#include <D3D11Shader.h>
#include <stdio.h>
#include <tchar.h>
#include "MathUtils.h"
#include "Globals.h"
namespace MoebiusEngine
{
	Shader::Shader()
	{
		m_Id = 0;
		u2DTexturesCount = 0;
		uCubeTexturesCount = 0;
		
		
#ifdef OPENGL_PROJECT
		m_States = NULL;
#else
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

		uStep.constantbuffer = -1;
		uLimit.constantbuffer = -1;
		uNear.constantbuffer = -1;
		uFar.constantbuffer = -1;
		uFade.constantbuffer = -1;
		uClarity.constantbuffer = -1;

		uAmbientColor.constantbuffer = -1;
		uAmbientWeight.constantbuffer = -1;
		uSpecularPower.constantbuffer = -1;
		uNumLights.constantbuffer = -1;
		
		m_BlendState = NULL;
		VertexShaderReflector = NULL;
		PixelShaderReflector = NULL;
		m_VertexShader = NULL;
		m_PixelShader = NULL;
		m_RasterizerState = NULL;
		pLayout = NULL;
#endif
		uLightTypes = NULL;
		uPosDirs = NULL;
		uLightColors = NULL;
	}

#ifdef OPENGL_PROJECT
	Shader::Shader(OpenGLClass* OpenGL):Shader()
	{
		m_OpenGL = OpenGL;
	}
#endif

	Shader::~Shader()
	{
		
	}
	void Shader::shutdown()
	{
#ifdef OPENGL_PROJECT
		if (m_States)
		{
			delete[] m_States;
			m_States = NULL;
		}
		if (uTexLoc)
		{
			delete[] uTexLoc;
			uTexLoc = NULL;
		}
		m_OpenGL->glDeleteProgram(m_program);
		m_OpenGL->glDeleteShader(m_VertexShader);
		m_OpenGL->glDeleteShader(m_FragmentShader);
#else
		if (VertexShaderReflector)
		{
			VertexShaderReflector->Release();
			VertexShaderReflector = 0;
		}
		if (PixelShaderReflector)
		{
			PixelShaderReflector->Release();
			PixelShaderReflector = 0;
		}
		if (m_VertexShader)
		{
			m_VertexShader->Release();
			m_VertexShader = 0;
		}
		if (m_PixelShader)
		{
			m_PixelShader->Release();
			m_PixelShader = 0;
		}
		if (m_vertexConstantBuffer)
		{
			for (int i = 0; i < m_numVConstBuffer; ++i)
				m_vertexConstantBuffer[i]->Release();
			delete[] m_vertexConstantBuffer;
			m_vertexConstantBuffer = 0;
		}
		if (m_pixelConstantBuffer)
		{
			for (int i = 0; i < m_numPConstBuffer; ++i)
				m_pixelConstantBuffer[i]->Release();
			delete[] m_pixelConstantBuffer;
			m_pixelConstantBuffer = 0;
		}
		if (m_BlendState)
		{
			m_BlendState->Release();
			m_BlendState = NULL;
		}
		if (m_RasterizerState)
		{
			m_RasterizerState->Release();
			m_RasterizerState = NULL;
		}
		if (pLayout)
		{
			pLayout->Release();
			pLayout = NULL;
		}
#endif
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
		
	}
	void Shader::Clean()
	{

	}
#ifdef OPENGL_PROJECT
	void Shader::EnableStates()
#else
	void Shader::EnableStates(ID3D11DeviceContext* devcon)
#endif
	{
#ifdef OPENGL_PROJECT
		for (int i = 0; i < m_StateCount; ++i)
		{
			glEnable(m_States[i]);
		}
#else
		devcon->RSSetState(m_RasterizerState);
		if (m_BlendState)
			devcon->OMSetBlendState(m_BlendState, 0, 0xffffff);
#endif
	}

#ifdef OPENGL_PROJECT
	void Shader::DisableStates()
#else
	void Shader::DisableStates(ID3D11DeviceContext* devcon)
#endif
	{
#ifdef OPENGL_PROJECT
		for (int i = 0; i < m_StateCount; ++i)
		{
			glDisable(m_States[i]);
		}
#endif
	}
#ifdef OPENGL_PROJECT
	void Shader::SetState(int index, GLuint state)
	{
		m_States[index] = state;
	}
	void Shader::InitStates(int stateCount)
	{
		m_StateCount = stateCount;
		m_States = new GLuint[m_StateCount];
	}
#else
	bool Shader::InitRasterizerState(D3D11_RASTERIZER_DESC* rasterizerDesc, ID3D11Device* dev)
	{
		return !FAILED(dev->CreateRasterizerState(rasterizerDesc, &m_RasterizerState));
	}

	void Shader::InitBlendState(ID3D11Device* dev)
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
			OutputDebugStringA("Falla al crear BlendState");
	}
#endif
#ifdef OPENGL_PROJECT
	int Shader::Init(char* fileVertexShader, char* filePixelShader)
	{
		m_VertexShader = LoadShader(GL_VERTEX_SHADER, fileVertexShader);
		m_States = NULL;
		uLightTypes = NULL;
		uLightColors = NULL;
		uPosDirs = NULL;
		if (m_VertexShader == 0)
			return -1;
		m_FragmentShader = LoadShader(GL_FRAGMENT_SHADER, filePixelShader);
		if (m_FragmentShader == 0)
			return -2;
		m_program = LoadProgram(m_VertexShader, m_FragmentShader);
		int attributeCount = 0;
		positionAttribute = m_OpenGL->glGetAttribLocation(m_program, "a_posL");
		if (positionAttribute != -1)
			attributeCount++;
		colorAttribute = m_OpenGL->glGetAttribLocation(m_program, "a_colorL");
		if (colorAttribute != -1)
			attributeCount++;
		normalAttribute = m_OpenGL->glGetAttribLocation(m_program, "a_normL");
		if (normalAttribute != -1)
			attributeCount++;
		uvAttribute = m_OpenGL->glGetAttribLocation(m_program, "a_uv");
		if (uvAttribute != -1)
			attributeCount++;
		binormalAttribute = m_OpenGL->glGetAttribLocation(m_program, "a_binormL");
		if (binormalAttribute != -1)
			attributeCount++;
		tangentAttribute = m_OpenGL->glGetAttribLocation(m_program, "a_tanL");
		if (tangentAttribute != -1)
			attributeCount++;

		uworldMatrix = m_OpenGL->glGetUniformLocation(m_program, "u_WMatrix");
		uWVP = m_OpenGL->glGetUniformLocation(m_program, "u_WVP");

		uTilingFactor = m_OpenGL->glGetUniformLocation(m_program, "u_TilingFactor");

		uTime = m_OpenGL->glGetUniformLocation(m_program, "u_Time");
		///Light parameters
		uAmbientColor = m_OpenGL->glGetUniformLocation(m_program, "u_ambient_color");
		uAmbientWeight = m_OpenGL->glGetUniformLocation(m_program, "u_ambient_weight");
		uSpecularPower = m_OpenGL->glGetUniformLocation(m_program, "u_specular_power");
		uNumLights = m_OpenGL->glGetUniformLocation(m_program, "u_numLights");
		uLightColors = new GLint[Globals::maxLightCount];
		uLightTypes = new GLint[Globals::maxLightCount];
		uPosDirs = new GLint[Globals::maxLightCount];
		for (int i = 0; i < Globals::maxLightCount; ++i)
		{
			char LightLoc[50];
			sprintf_s(LightLoc, "u_lightColor[%d]", i);
			uLightColors[i] = m_OpenGL->glGetUniformLocation(m_program, LightLoc);
			sprintf_s(LightLoc, "u_LightType[%d]", i);
			uLightTypes[i] = m_OpenGL->glGetUniformLocation(m_program, LightLoc);
			sprintf_s(LightLoc, "u_pos_dir[%d]", i);
			uPosDirs[i] = m_OpenGL->glGetUniformLocation(m_program, LightLoc);
		}
		ucamPos = m_OpenGL->glGetUniformLocation(m_program, "u_camPos");
		ufogColor = m_OpenGL->glGetUniformLocation(m_program, "u_fogColor");
		ufogRange = m_OpenGL->glGetUniformLocation(m_program, "u_fogRange");
		ufogStart = m_OpenGL->glGetUniformLocation(m_program, "u_fogStart");
		uFireAmp = m_OpenGL->glGetUniformLocation(m_program, "u_FireAmp");

		uWaterColor = m_OpenGL->glGetUniformLocation(m_program, "u_WaterColor");
		uFresnelPower = m_OpenGL->glGetUniformLocation(m_program, "u_FresnelPower");
		uDepthAdjust = m_OpenGL->glGetUniformLocation(m_program, "u_depthAdjust");
		uDepthAdjustDisplacement = m_OpenGL->glGetUniformLocation(m_program, "u_depthAdjustDisplacement");
		uMaxReflection = m_OpenGL->glGetUniformLocation(m_program, "u_maxReflection");

		uStep = m_OpenGL->glGetUniformLocation(m_program, "u_step");
		uLimit = m_OpenGL->glGetUniformLocation(m_program, "u_limit");
		uNear = m_OpenGL->glGetUniformLocation(m_program, "u_near");
		uFar = m_OpenGL->glGetUniformLocation(m_program, "u_far");
		uFade = m_OpenGL->glGetUniformLocation(m_program, "u_fade");
		uClarity = m_OpenGL->glGetUniformLocation(m_program, "u_clarity");

		uTexture = m_OpenGL->glGetUniformLocation(m_program, "u_s_texture");
		uCubeTexture = m_OpenGL->glGetUniformLocation(m_program, "u_s_CubeTexture");

		uTexLoc = new GLint[Globals::maxTextureCount];
		char TextureLoc[50];
		for (int i = 0; i < Globals::maxTextureCount; ++i)
		{
			sprintf_s(TextureLoc, "u_s_2DTextures[%d]", i);
			uTexLoc[i] = m_OpenGL->glGetUniformLocation(m_program, TextureLoc);
		}
		return 0;
	}
#else
	int Shader::Init(char* fileVertexShader, char* filePixelShader, ID3D11Device* dev)
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

		if (FAILED(D3DReflect(VertexShaderBlob->GetBufferPointer(), VertexShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&VertexShaderReflector)))
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
			if (strcmp(resourceBindDesc.SemanticName, "POSITION") == 0)
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
				//OutputDebugStringA(variableDesc.Name);
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
				//OutputDebugStringA(variableDesc.Name);
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

				if (strcmp(variableDesc.Name, "uAmbientColor") == 0)
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
				if (strcmp(variableDesc.Name, "uStep") == 0)
				{
					uStep.constantbuffer = i;
					uStep.offset = variableDesc.StartOffset;
				}
				if (strcmp(variableDesc.Name, "uLimit") == 0)
				{
					uLimit.constantbuffer = i;
					uLimit.offset = variableDesc.StartOffset;
				}
				if (strcmp(variableDesc.Name, "uNear") == 0)
				{
					uNear.constantbuffer = i;
					uNear.offset = variableDesc.StartOffset;
				}
				if (strcmp(variableDesc.Name, "uFar") == 0)
				{
					uFar.constantbuffer = i;
					uFar.offset = variableDesc.StartOffset;
				}
				if (strcmp(variableDesc.Name, "uFade") == 0)
				{
					uFade.constantbuffer = i;
					uFade.offset = variableDesc.StartOffset;
				}
				if (strcmp(variableDesc.Name, "uClarity") == 0)
				{
					uClarity.constantbuffer = i;
					uClarity.offset = variableDesc.StartOffset;
				}

			}

		}

		for (int i = 0; i < pixelShaderDesc.BoundResources; ++i)
		{
			D3D11_SHADER_INPUT_BIND_DESC resourceBindDesc;
			PixelShaderReflector->GetResourceBindingDesc(i, &resourceBindDesc);
			//OutputDebugStringA(resourceBindDesc.Name);
			if (strcmp(resourceBindDesc.Name, "Texture") == 0)
			{
				u2DTexturesCount = resourceBindDesc.BindCount;
			}
			if (strcmp(resourceBindDesc.Name, "SkyMap") == 0)
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
	ID3D11InputLayout* Shader::getVertexBufferLayout()
	{
		return pLayout;
	}
	ID3D11Buffer* Shader::getVertexConstBufferbyIndex(unsigned int index)
	{
		if (index >= m_numVConstBuffer)
			return NULL;
		return m_vertexConstantBuffer[index];
	}
	UINT Shader::getNumPixelConstBuffer()
	{
		return m_numPConstBuffer;
	}
	ID3D11Buffer* Shader::getPixelConstBufferbyIndex(unsigned int index)
	{
		if (index >= m_numPConstBuffer)
			return NULL;
		return m_pixelConstantBuffer[index];
	}
#endif
#ifdef OPENGL_PROJECT
	GLuint Shader::LoadShader(GLenum type, char * filename)
	{
		GLuint shader;
		GLint compiled;

		// Create the shader object
		shader = m_OpenGL->glCreateShader(type);

		if (shader == 0)
			return 0;
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
		
		m_OpenGL->glShaderSource(shader, 1, (const char **)&shaderSrc, NULL);
		delete[] shaderSrc;

		// Compile the shader
		m_OpenGL->glCompileShader(shader);

		// Check the compile status
		m_OpenGL->glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			GLint infoLen = 0;

			m_OpenGL->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen > 1)
			{
				char* infoLog = new char[infoLen];


				m_OpenGL->glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
				OutputDebugStringA("Error compiling shader <");
				OutputDebugStringA(filename);
				OutputDebugStringA(">:\n");
				OutputDebugStringA(infoLog);
				OutputDebugStringA("\n");

				delete[] infoLog;
			}

			m_OpenGL->glDeleteShader(shader);
			return 0;
		}

		return shader;
	}
	GLuint Shader::LoadProgram(GLuint vertexShader, GLuint fragmentShader)
	{
		GLuint programObject;
		GLint linked;
		// Create the program object
		programObject = m_OpenGL->glCreateProgram();

		if (programObject == 0)
			return 0;

		m_OpenGL->glAttachShader(programObject, vertexShader);
		m_OpenGL->glAttachShader(programObject, fragmentShader);

		m_OpenGL->glBindAttribLocation(programObject, 0, "a_posL");
		m_OpenGL->glBindAttribLocation(programObject, 1, "a_colorL");

		// Link the program
		m_OpenGL->glLinkProgram(programObject);
		// Check the link status
		m_OpenGL->glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

		if (!linked)
		{
			GLint infoLen = 0;

			m_OpenGL->glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen > 1)
			{
				char* infoLog = new char[sizeof(char) * infoLen];


				m_OpenGL->glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
				OutputDebugStringA("Error linking program:\n");
				OutputDebugStringA(infoLog);
				OutputDebugStringA("\n");
				delete infoLog;
			}

			m_OpenGL->glDeleteProgram(programObject);
			return 0;
		}

		return programObject;

	}
#else
	int Shader::LoadShader(unsigned int Type, char* filename, ID3DBlob** shaderBlob)
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
		if (Type == 2)
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
				OutputDebugStringA(message);
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
				OutputDebugStringA(message);
				errorBlob->Release();
			}
			return -1;
		}
	}
	ID3D11VertexShader* Shader::getVertexShader()
	{
		return m_VertexShader;
	}
	ID3D11PixelShader* Shader::getPixelShader()
	{
		return m_PixelShader;
	}
#endif
#ifdef OPENGL_PROJECT
	int Shader::Init(char* fileVertexShader, char* filePixelShader, unsigned int id)
	{
		m_Id = id;
		return Init(fileVertexShader, filePixelShader);
	}
#else
	int Shader::Init(char* fileVertexShader, char* filePixelShader, ID3D11Device* dev, unsigned int id)
	{
		m_Id = id;
		return Init(fileVertexShader, filePixelShader, dev);
	}
#endif
	unsigned int Shader::getVSinputsize()
	{
		return numVSinput;
	}
	UINT Shader::getNumVertexConstBuffer()
	{
		return m_numVConstBuffer;
	}
	
	
	unsigned int Shader::getId()
	{
		return m_Id;
	}
	

	/*//////////////////////////////////////////////////PASSING VALUES TO CONSTANT BUFFERS/////////////////////////////////////////////////////////////////////*/
#ifndef OPENGL_PROJECT
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
		memcpy(dst, val, 2 * sizeof(float));
	}
	void Uniform3f(void* pData, UINT offset, float value1, float value2, float value3)
	{
		float val[3] = { value1 ,value2, value3 };
		char* dst = (char*)pData;
		dst += offset;
		memcpy(dst, val, 3 * sizeof(float));
	}
	void Uniform4f(void* pData, UINT offset, float value1, float value2, float value3, float value4)
	{
		float val[4] = { value1 ,value2, value3, value4 };
		char* dst = (char*)pData;
		dst += offset;
		memcpy(dst, val, 4 * sizeof(float));
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
#endif
}
