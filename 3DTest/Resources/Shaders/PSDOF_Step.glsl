Texture2D Texture[3];
SamplerState samplerDD[3];
cbuffer ConstantBuffer
{
	float uNear;
	float uFar;
	float uFade;
	float uClarity;
	float4 uStep;
}

struct psIn
{
	float2 uv : TEXCOORD;
	float4 pos : SV_POSITION;
};

float CalcMixFactor(float2 uv)
{
	float depth = Texture[2].Sample(samplerDD[2],uv).x;
	float z = -uFar*uNear/(depth*(uFar-uNear)-uFar);
	float k = clamp(abs(z-uClarity)/uFade,0.0f,1.0f);
	return k;
}

float4 PShader(psIn input) : SV_TARGET
{
	float2 new_uv[8];
	new_uv[0] = input.uv + float2(uStep.x,0.0);
	new_uv[1] = input.uv + float2(uStep.z,uStep.w);
	new_uv[2] = input.uv + float2(0.0,uStep.y);
	new_uv[3] = input.uv + float2(-uStep.z,uStep.w);
	new_uv[4] = input.uv + float2(-uStep.x,0.0);
	new_uv[5] = input.uv + float2(-uStep.z,-uStep.w);
	new_uv[6] = input.uv + float2(0.0,-uStep.y);
	new_uv[7] = input.uv + float2(uStep.z,-uStep.w);
	
	float3 color[9];
	color[0] = Texture[1].Sample(samplerDD[1],input.uv);
	color[1] = Texture[1].Sample(samplerDD[1],new_uv[0]);
	color[2] = Texture[1].Sample(samplerDD[1],new_uv[1]);
	color[3] = Texture[1].Sample(samplerDD[1],new_uv[2]);
	color[4] = Texture[1].Sample(samplerDD[1],new_uv[3]);
	color[5] = Texture[1].Sample(samplerDD[1],new_uv[4]);
	color[6] = Texture[1].Sample(samplerDD[1],new_uv[5]);
	color[7] = Texture[1].Sample(samplerDD[1],new_uv[6]);
	color[8] = Texture[1].Sample(samplerDD[1],new_uv[7]);
	
	float d[9];
	d[0] = CalcMixFactor(input.uv);
	d[1] = CalcMixFactor(new_uv[0]);
	d[2] = CalcMixFactor(new_uv[1]);
	d[3] = CalcMixFactor(new_uv[2]);
	d[4] = CalcMixFactor(new_uv[3]);
	d[5] = CalcMixFactor(new_uv[4]);
	d[6] = CalcMixFactor(new_uv[5]);
	d[7] = CalcMixFactor(new_uv[6]);
	d[8] = CalcMixFactor(new_uv[7]);
	
	float3 OriginalColor = Texture[0].Sample(samplerDD[0],input.uv);
	float total = 2.0 + d[1] + d[2] + d[3] + d[4] + d[5] + d[6] + d[7] + d[8];
	float3 ColorBlur = (2.0*color[0] + d[1]*color[1] + d[2]*color[2] + d[3]*color[3] + d[4]*color[4] + d[5]*color[5] + d[6]*color[6] + d[7]*color[7] + d[8]*color[8])/total;
	float3 ColorFinal =  lerp(OriginalColor,ColorBlur,d[0]);
	return float4(ColorFinal,1.0f);
}