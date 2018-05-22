Texture2D Texture;
SamplerState samplerDD;
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
	float3 color = Texture.Sample(samplerDD,input.uv).xyz;
	float brightness = 0.3*color.r + 0.59*color.g+0.11*color.b;
	float val = step(uLimit, brightness);
	return float4(color*val,1.0f);
}