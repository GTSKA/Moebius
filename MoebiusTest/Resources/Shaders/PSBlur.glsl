Texture2D Texture;
SamplerState samplerDD;
cbuffer ConstantBuffer
{
	float4 uStep;
}

struct psIn
{
	float2 uv : TEXCOORD;
	float4 pos : SV_POSITION;
};

float4 PShader(psIn input) : SV_TARGET
{
	float3 FinalColor = Texture.Sample(samplerDD,input.uv).xyz;
	float3 colors[8];
	colors[0] = Texture.Sample(samplerDD, float2(input.uv.x + uStep.x,input.uv.y)).rgb;
	colors[1] = Texture.Sample(samplerDD, float2(input.uv.x - uStep.x,input.uv.y)).rgb;
	colors[2] = Texture.Sample(samplerDD, float2(input.uv.x,input.uv.y + uStep.y)).rgb;
	colors[3] = Texture.Sample(samplerDD, float2(input.uv.x,input.uv.y - uStep.y)).rgb;
	colors[4] = Texture.Sample(samplerDD, float2(input.uv.x + uStep.z,input.uv.y + uStep.w)).rgb;
	colors[5] = Texture.Sample(samplerDD, float2(input.uv.x - uStep.z,input.uv.y + uStep.w)).rgb;
	colors[6] = Texture.Sample(samplerDD, float2(input.uv.x - uStep.z,input.uv.y - uStep.w)).rgb;
	colors[7] = Texture.Sample(samplerDD, float2(input.uv.x + uStep.z,input.uv.y - uStep.w)).rgb;
	for(int i = 0; i < 8; ++i)
	{
		FinalColor += colors[i];
	}
	return float4(FinalColor*0.1,1.0);
}