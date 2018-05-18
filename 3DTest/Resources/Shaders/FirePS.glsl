Texture2D Texture[3];
SamplerState ss[4];
cbuffer ConstantBuffer
{
	float uTime;
}
struct PS_In
{
	float2 texcoord : TEXCOORD0;
	float4 position : SV_POSITION;
};

float4 PShader(PS_In input) : SV_TARGET
{
	float2 disp = Texture[2].Sample(ss[2],float2(input.texcoord.x,input.texcoord.y+uTime)).xy;
	float dMax = 0.125;
	float2 offset = float2((disp.x*2.0f-1.0f)*dMax,(disp.y*2.0f-1.0f)*dMax);
	float2 newTexcoord = input.texcoord + offset;
	float4 fireColor = Texture[0].Sample(ss[0],newTexcoord);
	float AlphaValue = Texture[1].Sample(ss[1],input.texcoord).x;
	float4 ColorFinal = fireColor * float4(1.0f,13.0f,1.0f,AlphaValue);
	return ColorFinal;
}