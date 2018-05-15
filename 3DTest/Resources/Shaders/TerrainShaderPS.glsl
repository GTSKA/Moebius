Texture2D Texture[4];
SamplerState ss;
float3 ufogColor;
float ufogStart;
float ufogRange;
float ucamPos : POSITION;
struct PS_In
{
	float2 uv : TEXCOORD0;
	float2 uvTiling : TEXCOORD1;
	float4 pos : POSITION1;
	float4 position : SV_POSITION;
};

float4 PShader(PS_In input) : SV_TARGET
{
	float4 Grass = Texture[0].Sample(ss, input.uvTiling);
	float4 Dirt = Texture[1].Sample(ss, input.uvTiling);
	float4 Rock = Texture[2].Sample(ss, input.uvTiling);
	float4 Blend = Texture[3].Sample(ss, input.uv);
	float4 ColorTerrain = (Dirt*Blend.x + Rock*Blend.y + Grass*Blend.z)/(Blend.x+Blend.y+Blend.z);
	
	float distanceToCamera = distance(ucamPos, input.pos);
	float4 fogColor;
	fogColor.x = ufogColor.x;
	fogColor.y = ufogColor.y;
	fogColor.z = ufogColor.z;
	fogColor.w = 1.0;
	float factor = clamp((distanceToCamera - ufogStart)/ufogRange, 0.0f, 1.0f);
	float4 colorFinal = lerp(ColorTerrain, fogColor, factor);
	return colorFinal;
}