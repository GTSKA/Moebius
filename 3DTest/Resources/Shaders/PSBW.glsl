Texture2D Texture;
SamplerState samplerDD;
struct psIn
{
	float2 uv : TEXCOORD;
	float4 pos : SV_POSITION;
};

float4 PShader(psIn input) : SV_TARGET
{
	float4 colorp = Texture.Sample(samplerDD,input.uv);
	float colorval = 0.3*colorp.r + 0.59*colorp.g + 0.11*colorp.b;
	float4 colorFinal = float4(colorval, colorval, colorval, 1.0f);
	return colorFinal;
}