#pragma once

#ifndef _MOEBIUS3D_EFFECT_H_
#define	_MOEBIUS3D_EFFECT_H_

#include "M3D_CommondInclude.h"
#include "strsafe.h"
#include "ResourceCtrl.h"

class CEffectCtrl_ : public IResourceCtrl 
{
public:

	LPD3DXEFFECT m_Effect;

	CEffectCtrl_();
	~CEffectCtrl_();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pDevice, LPCSTR effectCode);
	HRESULT ValidateTechnique(D3DXHANDLE handleTechnique);

	HRESULT OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	HRESULT OnLostDevice();
	void OnDisposeDevice();
};

class CEffectBase_ : public CEffectCtrl_ 
{
public:

	
	virtual HRESULT Init(LPDIRECT3DDEVICE9 pDevice);

	STRSAFE_LPSTR GetEffectCode();
	HRESULT SetEffectCode(LPDIRECT3DDEVICE9 pDevice, STRSAFE_LPCSTR effectCode);

protected:

	STRSAFE_LPSTR m_effectCode;
};

class CBasicShaders_ : public CEffectBase_
{
public:
	CBasicShaders_();
	~CBasicShaders_();
};

static const CHAR __SHADER_BASIC[] =
	//--------------------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------------------

	//Matrix 
	//---------------------------------------------------------------------------------------
	"float4x4 WorldViewProjection_Matrix;\n"

	//Parameters
	//---------------------------------------------------------------------------------------
	"float4 diffuseColor = {1.0f, 1.0f, 1.0f, 1.0f};\n"

	"float textureBalance = 0.0f;\n"

	//Textures
	//---------------------------------------------------------------------------------------
	"texture diffuseTexture_01;\n"
	"sampler2D diffuseTexture_01Sampler = sampler_state\n"
	"{\n"
	"Texture = <diffuseTexture_01>;\n"
	"MinFilter = Linear;\n"
	"MagFilter = Linear;\n"
	"MipFilter = Linear;\n"
	"AddressU = Clamp;\n"
	"AddressV = Clamp;\n"
	"};\n"

	"texture diffuseTexture_02;\n"
	"sampler2D diffuseTexture_02Sampler = sampler_state\n"
	"{\n"
	"Texture = <diffuseTexture_02>;\n"
	"MinFilter = Linear;\n"
	"MagFilter = Linear;\n"
	"MipFilter = Linear;\n"
	"AddressU = Clamp;\n"
	"AddressV = Clamp;\n"
	"};\n"

	//Vertex Structs
	//---------------------------------------------------------------------------------------
	"struct VertexInput\n"
	"{\n"
	"float4 position : POSITION;\n"
	"float2 texCoord0: TEXCOORD0;\n"
	"float4 color: COLOR;\n" 
	"};\n"

	//Pixel Structs
	//---------------------------------------------------------------------------------------
	"struct PixelInput\n"
	"{\n"
	"float4 hpos      : POSITION;\n"
	"float2 texCoord0 : TEXCOORD0;\n"
	"};\n"
	
	"struct PixelColorInput\n"
	"{\n"
	"float4 hpos      : POSITION;\n"
	"float2 texCoord0 : TEXCOORD0;\n"
	"float4 color     : COLOR0;\n"
	"};\n"

	//Vertex Shaders
	//---------------------------------------------------------------------------------------
	"PixelInput VertexShader_Main(VertexInput vs_in)\n"
	"{\n"
	"    PixelInput vs_out;\n"
	"    vs_out.hpos = mul(vs_in.position, WorldViewProjection_Matrix);\n"
	"    vs_out.texCoord0 = vs_in.texCoord0;\n"
	"    return vs_out;\n"
	"}\n"

	"PixelColorInput VertexShaderColor_Main(VertexInput vs_in)\n"
	"{\n"
	"    PixelColorInput vs_out;\n"
	"    vs_out.hpos = mul(vs_in.position, WorldViewProjection_Matrix);\n"
	"    vs_out.texCoord0 = vs_in.texCoord0;\n"
	"    vs_out.color = vs_in.color;\n"
	"    return vs_out;\n"
	"}\n"

	"PixelInput VS_SCREEN(float4 Pos: POSITION)\n"
	"{\n"
	"    PixelInput vs_out;\n"
	"    Pos.xy = sign(Pos.xy);\n"
	"    vs_out.hpos = float4(Pos.xy, 0 ,1);\n"
	"    vs_out.texCoord0.x = 0.5 * (1 + Pos.x);\n"
	"    vs_out.texCoord0.y = 0.5 * (1 - Pos.y);\n"
	"    return vs_out;\n"
	"}\n"

	//Pixel Shaders
	//---------------------------------------------------------------------------------------
	"float4 ColorOnly_PS(PixelInput ps_in) : COLOR\n"
	"{\n"
	"    return diffuseColor;\n"
	"}\n"

	"float4 VertexColor_PS(PixelColorInput ps_in) : COLOR\n"
	"{\n"
	"    return ps_in.color;\n"
	"}\n"

	"float4 VertexColorTextured_PS(PixelColorInput ps_in) : COLOR\n"
	"{\n"
	"    return ps_in.color * tex2D(diffuseTexture_01Sampler, ps_in.texCoord0);\n"
	"}\n"

	"float4 TextureOnly_PS(PixelInput ps_in) : COLOR\n"
	"{\n"
	"	return tex2D(diffuseTexture_01Sampler, ps_in.texCoord0);\n"
	"}\n"

	"float4 ColorTexturedPS(PixelInput ps_in) : COLOR\n"
	"{\n"
	"	return diffuseColor * tex2D(diffuseTexture_01Sampler, ps_in.texCoord0);\n"
	"}\n"

	
	"float4 TexturedLerp_PS(PixelInput ps_in) : COLOR\n"
	"{\n"
	"	return lerp(tex2D(diffuseTexture_01Sampler, ps_in.texCoord0), tex2D(diffuseTexture_02Sampler, ps_in.texCoord0), textureBalance);\n"
	"}\n"

	"technique Color \n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"			CullMode = NONE;\n"
	"		    VertexShader = compile vs_2_0 VertexShader_Main();\n"
	"		    PixelShader = compile ps_2_0 ColorOnly_PS();\n"
	"    }\n"
	"}\n"

	//Techniques
	//---------------------------------------------------------------------------------------
	"technique ColorWireframe\n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"		CullMode = NONE;\n"
	"	    VertexShader = compile vs_2_0 VertexShader_Main();\n"
	"		PixelShader = compile ps_2_0 ColorOnly_PS();\n"
	"       FillMode = WIREFRAME;\n"
	"	}\n"
	"}\n"

	"technique Textured\n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"		CullMode = NONE;\n"
	//"       FillMode = WIREFRAME;\n"
	"		    VertexShader = compile vs_2_0 VertexShader_Main();\n"
	"		    PixelShader = compile ps_2_0 TextureOnly_PS();\n"
	
	"    }\n"
	"}\n"

	"technique TexturedAlpha\n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"		CullMode = NONE;\n"
	//"       FillMode = WIREFRAME;\n"
	"		AlphaBlendEnable = true;\n"
	"		SrcBlend = SrcAlpha;\n"
	"		DestBlend = InvSrcAlpha;\n"
	"		    VertexShader = compile vs_2_0 VertexShader_Main();\n"
	"		    PixelShader = compile ps_2_0 ColorTexturedPS();\n"
	
	"    }\n"
	"}\n"

	"technique TexturedLerp\n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"		CullMode = NONE;\n"
	//"       FillMode = WIREFRAME;\n"
	"		AlphaBlendEnable = true;\n"
	"		SrcBlend = SrcAlpha;\n"
	"		DestBlend = InvSrcAlpha;\n"
	"		    VertexShader = compile vs_2_0 VertexShader_Main();\n"
	"		    PixelShader = compile ps_2_0 TexturedLerp_PS();\n"
	
	"    }\n"
	"}\n"

		"technique TexturedLerp_NoAlpha\n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"		CullMode = NONE;\n"
	//"       FillMode = WIREFRAME;\n"
	//"		AlphaBlendEnable = true;\n"
	//"		SrcBlend = SrcAlpha;\n"
	//"		DestBlend = InvSrcAlpha;\n"
	"		    VertexShader = compile vs_2_0 VertexShader_Main();\n"
	"		    PixelShader = compile ps_2_0 TexturedLerp_PS();\n"
	
	"    }\n"
	"}\n"

	"technique TexturedWireframe\n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"		CullMode = NONE;\n"
	"	    VertexShader = compile vs_2_0 VertexShader_Main();\n"
	"	    PixelShader = compile ps_2_0 TextureOnly_PS();\n"
	"       FillMode = WIREFRAME;\n"
	"	}\n"
	"}\n"

	"technique VertexColor\n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"		CullMode = NONE;\n"
	"		    VertexShader = compile vs_2_0 VertexShaderColor_Main();\n"
	"		    PixelShader = compile ps_2_0 VertexColor_PS();\n"
	"    }\n"
	"}\n"

	"technique VertexColorAlpha\n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"		CullMode = NONE;\n"
	"		AlphaBlendEnable = true;\n"
	"		SrcBlend = SrcAlpha;\n"
	"		DestBlend = InvSrcAlpha;\n"
	"		    VertexShader = compile vs_2_0 VertexShaderColor_Main();\n"
	"		    PixelShader = compile ps_2_0 VertexColor_PS();\n"
	"    }\n"
	"}\n"

	"technique VertexColorTextured\n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"		CullMode = NONE;\n"
	"		    VertexShader = compile vs_2_0 VertexShaderColor_Main();\n"
	"		    PixelShader = compile ps_2_0 VertexColorTextured_PS();\n"
	"    }\n"
	"}\n";

//---------------------------------------------------------------------------------------------------------------------------
//Node Shader
//---------------------------------------------------------------------------------------------------------------------------


static const CHAR __NODE_SHADER[] =

"float4x4 WorldViewProjection_Matrix;\n"

"float4  NodeFillColor = {1.0f, 1.0f, 1.0f, 1.0f};\n"
"float4  NodeBorderColor = {1.0f, 1.0f, 1.0f, 1.0f};\n"
"float4  SelectionColor = {1.0f, 1.0f, 1.0f, 1.0f};\n"

"float NodeAlpha = 1.0;\n"
"float SelectionAlpha = 1.0;\n"

"texture selectionMask;\n"
"sampler selectionMask_Sampler = sampler_state \n"
"{\n"
"    texture = <selectionMask>;\n"
"    MinFilter = Linear;\n"
"	MagFilter = Linear;\n"
"	MipFilter = Linear;\n"
"};\n"

"texture NodeFill;\n"
"sampler NodeFill_Sampler = sampler_state \n"
"{\n"
"   texture = <NodeFill>;\n"
"   MinFilter = Linear;\n"
"	MagFilter = Linear;\n"
"	MipFilter = Linear;\n"
"};\n"

"texture NodeTextureFill;\n"
"sampler NodeTextureFill_Sampler = sampler_state \n"
"{\n"
"   texture = <NodeTextureFill>;\n"
"   MinFilter = Linear;\n"
"	MagFilter = Linear;\n"
"	MipFilter = Linear;\n"
"};\n"

"texture NodeBorder;\n"
"sampler NodeBorder_Sampler = sampler_state \n"
"{\n"
"   texture = <NodeBorder>;\n"
"   MinFilter = Linear;\n"
"	MagFilter = Linear;\n"
"	MipFilter = Linear;\n"
"};\n"

//------------------------------------
"struct vertexInput {\n"
"	float4 Position	: POSITION;\n"
"	float2 texCoord : TEXCOORD;\n"
"	float4 color: COLOR;\n" 
"};\n"

"struct VS_OUTPUT {\n"
"  float4 Pos: POSITION;\n"
"  float2 texCoord: TEXCOORD;\n"
"  float4 color: COLOR;\n" 
"};\n"

//------------------------------------
"VS_OUTPUT VS_TransformDiffuse(vertexInput IN) \n"
"{\n"
"    VS_OUTPUT OUT;\n"
//"    OUT.Pos = mul(float4(IN.Position.xyz , 1.0), matWVP);\n"
"    OUT.Pos = mul(IN.Position, WorldViewProjection_Matrix);\n"
"    OUT.texCoord = IN.texCoord;\n"
"    OUT.color = IN.color;\n"
"    return OUT;\n"
"}\n"

"float4 PS_NODE_01(VS_OUTPUT IN): COLOR\n"
"{\n"
"	float4 nodeFillDiffuse =  tex2D(NodeFill_Sampler, IN.texCoord.xy);\n"
"	float4 nodeBorderDiffuse =  tex2D(NodeBorder_Sampler, IN.texCoord.xy);\n"
"	float4 nodeTextureFillDiffuse = tex2D(NodeTextureFill_Sampler, IN.texCoord.xy);\n"
"	float4 selectionDiffuse =  tex2D(selectionMask_Sampler, IN.texCoord.xy);\n"

"	float4 nodeB = float4(float3(NodeFillColor.xyz * nodeFillDiffuse.xyz * nodeTextureFillDiffuse.xyz), nodeFillDiffuse.w);\n"

//"	float4 nodeC = float4(lerp(nodeB.xyz, \n"
//"	NodeBorderColor * nodeBorderDiffuse.xyz, \n"
//"	lerp(nodeB.w, nodeBorderDiffuse.w, nodeBorderDiffuse.w)),\n"
//"	nodeFillDiffuse.w * NodeAlpha);\n"

"	float4 nodeC =  float4(lerp(nodeB.xyz, NodeBorderColor * nodeBorderDiffuse.xyz,  nodeBorderDiffuse.w), \n"
"	nodeFillDiffuse.w * NodeAlpha);\n"

"	return lerp(float4(SelectionColor * selectionDiffuse.xyz, SelectionAlpha * selectionDiffuse.w * IN.color.w),\n"
"	nodeC,\n"
"	nodeFillDiffuse.w);\n"

"}\n"

//-----------------------------------
"technique Main\n"
"{\n"
"    pass p0 \n"
"    {\n"
"    	AlphaBlendEnable = true;\n"
"		SrcBlend = SrcAlpha;\n"
"		DestBlend = InvSrcAlpha;\n"
"		VertexShader = compile vs_1_1 VS_TransformDiffuse();\n"
"		PixelShader = compile ps_2_0 PS_NODE_01();\n"
"    }\n"
"}\n";

//---------------------------------------------------------------------------------------------------------------------------
//Function Shader
//---------------------------------------------------------------------------------------------------------------------------


static const CHAR __FUNCTION_SHADER[] =
	//--------------------------------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------------------------------

	//Matrix 
	//---------------------------------------------------------------------------------------
	"float4x4 WorldViewProjection_Matrix;\n"

	//Parameters
	//---------------------------------------------------------------------------------------
	"float4 diffuseColor = {1.0f, 1.0f, 1.0f, 1.0f};\n"

	"float textureBalance = 0.0f;\n"

	"float t = 0.0f;\n"

	//Textures
	//---------------------------------------------------------------------------------------
	"texture diffuseTexture_01;\n"
	"sampler2D diffuseTexture_01Sampler = sampler_state\n"
	"{\n"
	"Texture = <diffuseTexture_01>;\n"
	"MinFilter = ANISOTROPIC;\n"
	"MagFilter = ANISOTROPIC;\n"
	"MipFilter = Linear;\n"
	"AddressU = Clamp;\n"
	"AddressV = Clamp;\n"
	"};\n"

	"texture diffuseTexture_02;\n"
	"sampler2D diffuseTexture_02Sampler = sampler_state\n"
	"{\n"
	"Texture = <diffuseTexture_02>;\n"
	"MinFilter = ANISOTROPIC;\n"
	"MagFilter = ANISOTROPIC;\n"
	"MipFilter = Linear;\n"
	"AddressU = Clamp;\n"
	"AddressV = Clamp;\n"
	"};\n"

	//Textures
	//---------------------------------------------------------------------------------------
	"texture noiseTexture;\n"
	"sampler2D noiseTexture_Sampler = sampler_state\n"
	"{\n"
	"Texture = <diffuseTexture_01>;\n"
	"MinFilter = Linear;\n"
	"MagFilter = Linear;\n"
	"MipFilter = Linear;\n"
	"AddressU = Clamp;\n"
	"AddressV = Clamp;\n"
	"};\n"

	//Vertex Structs
	//---------------------------------------------------------------------------------------
	"struct VertexInput\n"
	"{\n"
	"float4 position : POSITION;\n"
	"float2 texCoord0: TEXCOORD0;\n"
	"};\n"

	//Pixel Structs
	//---------------------------------------------------------------------------------------
	"struct PixelInput\n"
	"{\n"
	"float4 hpos      : POSITION;\n"
	"float2 texCoord0 : TEXCOORD0;\n"
	"};\n"

	//((Math.Sin(Math.Sqrt((x * x) + (z * z)))) / (Math.Sqrt((x * x) + (z * z))));

	//Vertex Shaders
	//---------------------------------------------------------------------------------------
	"PixelInput VertexShader_Main(VertexInput vs_in)\n"
	"{\n"
	"   PixelInput vs_out;\n"
	//"	float4 v = tex2D(noiseTexture_Sampler, vs_in.texCoord0);\n"
	//"	vs_in.position.z = t * v.x;\n"
	//"	vs_in.position.z = sin( t * (vs_in.position.x * vs_in.position.y));\n"
	"	vs_in.position.z = t * ((sin(sqrt((vs_in.position.x * vs_in.position.x) + (vs_in.position.y * vs_in.position.y)))) / (sqrt((vs_in.position.x * vs_in.position.x) + (vs_in.position.y * vs_in.position.y))));\n"
	//" vs_in.position.z = (((vs_in.position.x * vs_in.position.x) + (vs_in.position.y * vs_in.position.y)) * sin(vs_in.position.x)) / (1 * vs_in.position.x);\n"
	"   vs_out.hpos = mul(vs_in.position, WorldViewProjection_Matrix);\n"
	"   vs_out.texCoord0 = vs_in.texCoord0;\n"
	"   return vs_out;\n"
	"}\n"

		//Pixel Shaders
	//---------------------------------------------------------------------------------------
	"float4 Textured_PS(PixelInput ps_in) : COLOR\n"
	"{\n"
	"    return lerp(tex2D(diffuseTexture_01Sampler, ps_in.texCoord0), tex2D(diffuseTexture_02Sampler, ps_in.texCoord0), textureBalance);\n"
	"}\n"

		//Techniques
	//---------------------------------------------------------------------------------------
	"technique Main\n"
	"{\n"
	"    pass p0 \n"
	"    {\n"
	"		CullMode = NONE;\n"
	"	    VertexShader = compile vs_3_0 VertexShader_Main();\n"
	"		PixelShader = compile ps_3_0 Textured_PS();\n"
	//"       FillMode = WIREFRAME;\n"
	"	}\n"
	"}\n";

//End Shaders Code
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------



	

#endif
