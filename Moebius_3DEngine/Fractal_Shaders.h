#pragma once

#ifndef _MOEBIUS3D_FRACTAL_SHADERS
#define	_MOEBIUS3D_FRACTAL_SHADERS

class CFractalShaders__ : public CEffectBase_
{
public:
	CBasicShaders_();
	~CBasicShaders_();
};

const CHAR __FRACTAL_SHADERS[] =

	"float MaxIterations = 16.0;\n"
	"float MaxRadio =  4.0;\n"
	"float Zoom = 1.0;\n"
	"float CenterX = 0.0;\n"
	"float CenterY = 0.0;\n"
	"float outColorDefinition = 0.05;\n"
	"float JuliaSeedX = 0.0;\n"
	"float JuliaSeedY = 0.0;\n"
	"float4  inColor = {1.0f, 0.0f, 0.0f, 1.0f};\n"
	"float4  outColor1 = {0.0f, 1.0f, 0.0f, 1.0f};\n"
	"float4  outColor2 = {0.0f, 0.0f, 1.0f, 1.0f};\n"

	"struct VS_OUTPUT {\n"
	"float4 Pos: POSITION;\n"
	"float2 texCoord: TEXCOORD;\n"
	"};\n"

	"VS_OUTPUT VS_MAIN(float4 Pos: POSITION){\n"
	"	  VS_OUTPUT Out;\n"

	// Clean up inaccuracies
   "Pos.xy = sign(Pos.xy);\n"
   "Out.Pos = float4(Pos.xy, 0, 1);\n"
   "Out.texCoord = Pos.xy;\n"

   //Out.Pos = sign(Pos);
   //Out.texCoord.x = 0.5 * (1 + Pos.x);
   //Out.texCoord.y = 0.5 * (1 - Pos.y);

   "return Out;\n"
   "}\n"

   "float4 PS_JULIA(VS_OUTPUT IN): COLOR\n"
   "{\n"
   "float real  = (IN.texCoord.x * Zoom) + CenterX;\n"
   "float imag  = (IN.texCoord.y * Zoom) + CenterY;\n"
   //float Creal = JuliaSeedX;
   //float Cimag = JuliaSeedY;

   "float r2 = 0.0;\n"
   "float iter = 0;\n"
   "for(iter = 0.0; iter < MaxIterations && r2 < MaxRadio; ++iter)\n"
   "{\n"
   "        float t = real;\n"
   "        real = (t*t) - (imag * imag) + JuliaSeedX;\n"
   "        imag = 2.0*(t*imag) + JuliaSeedY;\n"
   "        r2   = (real * real) + (imag * imag);\n"
   "}\n"

   "float3 color;\n"
   "if (r2 < MaxRadio)\n"
   "{\n"
   "        color = inColor.rgb;\n"
   "}\n"
   "else\n"
   "{\n"
   "    	color = lerp(outColor1.rgb, outColor2.rgb, frac(iter * outColorDefinition));\n"
   "}\n"
   "return float4(clamp(color, 0.0, 1.0), 1.0);\n"
   "}\n"

   "float4 PS_MANDELBROT(VS_OUTPUT IN): COLOR\n"
   "{\n"
   "float   real  = (IN.texCoord.x * Zoom) + CenterX;\n"
   "float   imag  = (IN.texCoord.y * Zoom) + CenterY;\n"
   "float   cReal = real;\n"
   "float   cImag = imag;\n"
   "float   r2    = 0.0;\n"
   "float   iter;\n"

   "for (iter = 0.0; iter < MaxIterations && r2 < 4.0; ++iter)\n"
   "{\n"
   "	float tempreal = real;\n"
   "	real = (tempreal * tempreal) - (imag * imag) + cReal;\n"
   "	imag = 2.0 * tempreal * imag + cImag;\n"
   "    r2   = (real * real) + (imag * imag);\n"
   "}\n"

   "float3 color;\n"
   "if (r2 < 4.0)\n"
   "{\n"
   "   	color = inColor.rgb;\n"
   "}\n"
   "else\n"
   "{\n"
   "    color = lerp(outColor1.rgb, outColor2.rgb, frac(iter * outColorDefinition));\n"
   "}\n"
   "return float4(clamp(color, 0.0, 1.0), 1.0);\n"
   "}\n"

	//-----------------------------------
	"technique Julia\n"
	"{\n"
	"pass p0 \n"
	"{\n"
	"	VertexShader = compile vs_1_1 VS_MAIN();\n"
	"	PixelShader  = compile ps_3_0 PS_JULIA();\n"
	"}\n"
	"}\n"

	//-----------------------------------
	"technique Mandelbrot\n"
	"{\n"
	"pass p0 \n"
	"{\n"
	"	VertexShader = compile vs_1_1 VS_MAIN();\n"
	"	PixelShader  = compile ps_3_0 PS_MANDELBROT();\n"
	"}\n"
	"}\n";

#endif