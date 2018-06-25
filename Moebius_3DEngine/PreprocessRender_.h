#pragma once

#ifndef _MOEBIUS3D_PREPROCESS_H_
#define	_MOEBIUS3D_PREPROCESS_H_

#include "M3D_CommondInclude.h"
#include "Geometry_.h"
#include "ResourceCtrl.h"
#include "Effect_.h"

class CPreprocessRender_ : public IResourceCtrl
{
public:

	CEffectBase_ m_effect;
	CGeometry_<_MoebiusVertex_puvc>	m_screenPlane;
	

	CPreprocessRender_(void);
	~CPreprocessRender_(void);

	void Render(LPDIRECT3DDEVICE9 pDevice);

	void SetBkColor01(D3DXVECTOR4 color);
	void SetBkColor02(D3DXVECTOR4 color);
	void SetGradientColors(D3DXVECTOR4 color1, D3DXVECTOR4 color2, D3DXVECTOR4 color3, D3DXVECTOR4 color4);

	void SetBkTexture01(LPDIRECT3DTEXTURE9 bkTexture); 
	void SetBkTexture02(LPDIRECT3DTEXTURE9 bkTexture); 

	void SetTextureTilling(float tX, float tY);
	void SetTextureBalance(float balance);

	virtual HRESULT OnInitDeviceObjects(LPDIRECT3DDEVICE9 pDevice);
	virtual HRESULT	OnResetDevice(LPDIRECT3DDEVICE9 pDevice);
	virtual HRESULT	OnLostDevice();
	virtual void OnDisposeDevice();

protected:

	
	D3DXVECTOR4					bkColors[4];
	float						fParameters[4];
};

const CHAR __PREPROCESS_SHADER_[] =

	//Parameters
	//---------------------------------------------------------------------------------------
	"float4 _colors[4] =\n"
	"{\n"
	"float4(1,1,1,1),	//Color & Gradient Color 1\n"
	"float4(1,1,1,1),	//Gradient Color 1\n"
	"float4(1,1,1,1),	//Gradient Color 1\n"
	"float4(1,1,1,1),	//Gradient Color 1\n"
	"};\n"

	"float _parameters[4] =\n"
	"{\n"
	"1,					//Texture Tilling X\n"
	"1,					//Texture Tilling Y\n"
	"0,					//Texture Balance\n"
	"0,					//\n"
	"};\n"

	//Textures
	//---------------------------------------------------------------------------------------
	"texture DiffuseTexture_01;\n"
	"sampler2D DiffuseTexture_01Sampler = sampler_state \n"
	"{\n"
	"	Texture = <DiffuseTexture_01>;\n"
	"	MinFilter = Linear;\n"
	"	MagFilter = Linear;\n"
	"	MipFilter = Linear;\n"
	"   AddressU = Wrap;\n"
	"	AddressV = Wrap;\n"
	"};\n"

	"texture DiffuseTexture_02;\n"
	"sampler2D DiffuseTexture_02Sampler = sampler_state \n"
	"{\n"
	"	Texture = <DiffuseTexture_02>;\n"
	"	MinFilter = Linear;\n"
	"	MagFilter = Linear;\n"
	"	MipFilter = Linear;\n"
	"   AddressU = Wrap;\n"
	"	AddressV = Wrap;\n"
	"};\n"


	//Functions
	//---------------------------------------------------------------------------------------
	"void Generator_Gradient2D(\n"
	"float4 bottom_left,\n"
	"float4 bottom_right,\n"
	"float4 top_left,\n"
	"float4 top_right,\n"
	"float2 texCoord,\n"
	"out float4 result)\n"
	"{\n"
	"	{\n"
	"	float4 hb, ht;\n"
	"	float2 m = abs(texCoord);\n"
	"	hb = (lerp(bottom_right, bottom_left, m.x));\n"
	"	ht = (lerp(top_right, top_left, m.x));\n"
	"	result = (lerp(ht, hb, m.y));\n"
	"	}\n"
	"}\n"
	
	//Vertex Structs
	//---------------------------------------------------------------------------------------
	"struct VS_IN\n"
	"{\n"
	"float4 position 		: POSITION;\n"
	"float2 texCoord0		: TEXCOORD0;\n"
	"};\n"

	//Pixel Structs
	//---------------------------------------------------------------------------------------
	"struct VS_OUT\n"
	"{\n"
	"	float4 ps  : POSITION;\n"
	"	float2 tx  : TEXCOORD0;\n"
	"};\n"

	//Vertex Shaders
	//---------------------------------------------------------------------------------------
	"VS_OUT Screen_VS(VS_IN IN)\n"
	"{\n"
	"VS_OUT vs_out;\n"
	"IN.position.xy = sign(IN.position.xy);\n"

	"vs_out.ps = IN.position;\n"

	"vs_out.tx.x = 0.5 * (1 + IN.position.x);\n"
	"vs_out.tx.y = 0.5 * (1 - IN.position.y);\n"
	"return vs_out;\n"
	"}\n"

	//Pixel Shaders
	//---------------------------------------------------------------------------------------

	//----------------------------------------------------------------------
	//Screen Only
	//----------------------------------------------------------------------
	"float4 ScreenOnly_PS(VS_OUT ps_in) : COLOR\n"
	"{\n"
	"	return _colors[0] * tex2D(DiffuseTexture_01Sampler, ps_in.tx);\n"
	"}\n"

	//----------------------------------------------------------------------
	//Screen Only
	//----------------------------------------------------------------------
	"float4 TextureLerp_PS(VS_OUT ps_in) : COLOR\n"
	"{\n"
	"	return _colors[0] * lerp(tex2D(DiffuseTexture_01Sampler, ps_in.tx), tex2D(DiffuseTexture_02Sampler, ps_in.tx), _parameters[2]);\n"
	"}\n"

	//----------------------------------------------------------------------
	//Tilling
	//----------------------------------------------------------------------
	"float4 TextureTilling_PS(VS_OUT ps_in) : COLOR\n"
	"{\n"
	"	ps_in.tx.x = ps_in.tx.x * _parameters[0];\n"
	"	ps_in.tx.y = ps_in.tx.y * _parameters[1];\n"
	"	return _colors[0] * tex2D(DiffuseTexture_01Sampler, ps_in.tx);\n"
	"}\n"

	//----------------------------------------------------------------------
	//Gradient
	//----------------------------------------------------------------------
	"float4 Gradient_PS(VS_OUT ps_in) : COLOR\n"
	"{\n"
	"float4 result;\n"
	"Generator_Gradient2D(\n"
	"_colors[3],\n"
	"_colors[2],\n"
	"_colors[1],\n"
	"_colors[0],\n"
	"ps_in.tx,\n"
	"result);\n"
	"return result *\n"
	"tex2D(DiffuseTexture_01Sampler, ps_in.tx);\n"
	/*"lerp(tex2D(DiffuseTexture_01Sampler, ps_in.tx), tex2D(DiffuseTexture_02Sampler, ps_in.tx), \n"
	"	TextureLerp);\n"*/
	"}\n"

	//Techniques
	//---------------------------------------------------------------------------------------
	"technique ScreenOnlyPS\n"
	"{\n"
	"pass P0\n"
	"{\n"
	"	VertexShader = compile vs_1_1 Screen_VS();\n"
	"	PixelShader =  compile ps_2_0 ScreenOnly_PS();\n"
	"}\n"
	"}\n"

	"technique TextureLerp\n"
	"{\n"
	"pass P0\n"
	"{\n"
	"	VertexShader = compile vs_1_1 Screen_VS();\n"
	"	PixelShader =  compile ps_2_0 TextureLerp_PS();\n"
	"}\n"
	"}\n"

	"technique TextureTillingPS\n"
	"{\n"
	"pass P0\n"
	"{\n"
	"	VertexShader = compile vs_1_1 Screen_VS();\n"
	"	PixelShader =  compile ps_2_0 TextureTilling_PS();\n"
	"}\n"
	"}\n"

	"technique GradientPS\n"
	"{\n"
	"pass P0\n"
	"{\n"
	"	VertexShader = compile vs_1_1 Screen_VS();\n"
	"	PixelShader =  compile ps_2_0 Gradient_PS();\n"
	"}\n"
	"}\n";

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

#endif
