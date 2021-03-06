Texture2D Texture[4];
SamplerState samplerDD[4];
cbuffer ConstantBuffer
{
float3 ufogColor;
float ufogStart;
float ufogRange;
float3 ucamPos : POSITION;
}
struct PS_In
{
	float2 uv : TEXCOORD0;
	float2 uvTiling : TEXCOORD1;
	float3 pos : POSITION1;
	float4 position : SV_POSITION;
};

float4 PShader(PS_In input) : SV_TARGET
{
	float4 Grass = Texture[0].Sample(samplerDD[0], input.uvTiling);
	float4 Dirt = Texture[1].Sample(samplerDD[1], input.uvTiling);
	float4 Rock = Texture[2].Sample(samplerDD[2], input.uvTiling);
	float4 Blend = Texture[3].Sample(samplerDD[3], input.uv);
	float4 ColorTerrain = (Dirt*Blend.x + Rock*Blend.y + Grass*Blend.z)/(Blend.x+Blend.y+Blend.z);
	
	float distanceToCamera = distance(ucamPos, float4(input.pos,1.0f));
	float4 fogColor;
	fogColor.x = ufogColor.x;
	fogColor.y = ufogColor.y;
	fogColor.z = ufogColor.z;
	fogColor.w = 1.0;
	float factor = clamp((distanceToCamera - ufogStart)/ufogRange, 0.0f, 1.0f);
	float4 colorFinal = lerp(ColorTerrain, fogColor, factor);
	return colorFinal;
}