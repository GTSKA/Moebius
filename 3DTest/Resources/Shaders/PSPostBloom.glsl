Texture2D Texture[2];
SamplerState samplerDD[2];
cbuffer ConstantBuffer
{
	float4 uLimit;
}

struct psIn
{
	float2 uv : TEXCOORD;
	float4 pos : SV_POSITION;
};

float4 PShader(psIn input) : SV_TARGET
{
	float4 color = Texture[0].Sample(samplerDD[0],input.uv);
	float4 blur = Texture[1].Sample(samplerDD[1],input.uv);
	return uLimit*blur + color;
}
