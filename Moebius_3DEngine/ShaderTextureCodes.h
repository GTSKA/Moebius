#pragma once

#ifndef _MOEBIUS3D_SHADER_TEXTURE_H_
#define	_MOEBIUS3D_SHADER_TEXTURE_H_

const CHAR __TEXTURESHADER_SINGLECOLOR[] =
	"float4 color = {0, 0 ,0 ,0};\n"
	"float4 Main(\n"
	"float2 vTexCoord : POSITION,\n"
	"float2 vTexelSize : PSIZE) : COLOR\n"
	"{\n"
	"return color;\n"
	"};\n";

const CHAR __TEXTURESHADER_RAINBOW4[] =

	"float4 color;\n"
	"float4 Main(\n"
	"float2 vTexCoord : POSITION,\n"
	"float2 vTexelSize : PSIZE) : COLOR\n"
	"{\n"
    "float r,g, b, xSq,ySq, a;\n"
    "xSq = 2.f*vTexCoord.x-1.f; xSq *= xSq;\n"
    "ySq = 2.f*vTexCoord.y-1.f; ySq *= ySq;\n"
    "a = sqrt(xSq+ySq);\n"
	"if (a > 1.0f) {\n"
	"a = 1.0f-(a-1.0f);\n"
	"}\n"
	"else if (a < 0.2f) {\n"
	"a = 0.2f;\n"
	"}\n"
	"r = 1-vTexCoord.x;\n"
	"g = 1-vTexCoord.y;\n"
	"b = vTexCoord.x;\n"
	"return float4(r, g, b, a);\n"
	"};\n";

const CHAR __TEXTURESHADER_NOISETEST[] =

	"float Scale = 0.11f;\n"
	"float CenterX = 0.709f;\n"
	"float CenterY = 0.350f;\n"
	"float Range = 0.05f;\n"
	"float3 InColor = {1,0,0};\n"
	"float3 OutColorA = {0,1,0};\n"
	"float3 OutColorB = {0,0,1};\n"


	"float4 Main(\n"
	"float2 vTexCoord : POSITION,\n"
	"float2 vTexelSize : PSIZE) : COLOR\n"
	"{\n"
	"float2 pos = frac(vTexCoord);\n"
    "float real = ((pos.x - 0.5)*Scale)-CenterX;\n"
    "float imag = ((pos.y - 0.5)*Scale)-CenterY;\n"
    "float Creal = real;\n"
    "float Cimag = imag;\n"
    "float r2 = 0;\n"
    "float i;\n"
	//"for (i=0; (i<25); i++) {\n"
    "for (i=0; (i<25) && (r2<4.0); i++) {\n"
		"float tempreal = real;\n"
		"real = (tempreal*tempreal) - (imag*imag) + Creal;\n"
		"imag = 2*tempreal*imag + Cimag;\n"
		"r2 = (real*real) + (imag*imag);\n"
    "}\n"
    "float3 finalColor;\n"
    "if (r2 < 4) {\n"
       "finalColor = InColor;\n"
    "} else {\n"
    	"finalColor = lerp(OutColorA,OutColorB,frac(i * Range));\n"
    "}\n"
    "return float4(finalColor,1);\n"
	"};\n";

const CHAR __TEXTURESHADER_NOISETEST_01[] =
	"float4 color1;\n"
	"float4 color2;\n"
	"float t = 1.0;\n"
	"float s = 1.0;\n"
	"float4 Main(\n"
	"float2 vTexCoord : POSITION,\n"
	"float2 vTexelSize : PSIZE) : COLOR\n"
	"{\n"
	"float v = (noise((vTexCoord*t) + s)/2);\n"
	"v += ((noise((vTexCoord*2*t) +s))/4);\n"
	"v += ((noise((vTexCoord*4*t) +s))/8);\n"
	"v += ((noise((vTexCoord*8*t) +s))/16);\n"
	"v += ((noise((vTexCoord*16*t)+s))/32);\n"
	"v += ((noise((vTexCoord*32*t)+s))/32);\n"
	"v = (10 * v);\n"
	"return lerp(\n"
		"color1,\n"
		"color2,\n"
		"v);\n"
	"};\n";

 //PNVertexToPixel Output = (PNVertexToPixel)0;    
 //   
 //   float4 perlin = tex2D(TextureSampler, (PSIn.TextureCoords))/2;
 //   perlin += tex2D(TextureSampler, (PSIn.TextureCoords)*2)/4;
 //   perlin += tex2D(TextureSampler, (PSIn.TextureCoords)*4)/8;
 //   perlin += tex2D(TextureSampler, (PSIn.TextureCoords)*8)/16;
 //   perlin += tex2D(TextureSampler, (PSIn.TextureCoords)*16)/32;
 //   perlin += tex2D(TextureSampler, (PSIn.TextureCoords)*32)/32;    

#endif


