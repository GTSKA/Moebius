Texture2D Texture[3];
SamplerState samplerDD[3];
cbuffer ConstantBuffer
{
	float uTime;
	float uFireAmp;
}
struct PS_In
{
	float2 texcoord : TEXCOORD0;
	float4 position : SV_POSITION;
};

float4 PShader(PS_In input) : SV_TARGET
{
	float2 disp = Texture[2].Sample(samplerDD[2],float2(input.texcoord.x,input.texcoord.y+uTime)).xy;
	float dMax = uFireAmp;
	float2 offset = float2((disp.x*2.0f-1.0f)*dMax,(disp.y*2.0f-1.0f)*dMax);
	float2 newTexcoord = input.texcoord + offset;
	float4 fireColor = Texture[0].Sample(samplerDD[0],newTexcoord);
	float AlphaValue = Texture[1].Sample(samplerDD[1],input.texcoord).x;
	float4 ColorFinal = fireColor * float4(1.0f,1.0f,1.0f,AlphaValue);
	return ColorFinal;
}