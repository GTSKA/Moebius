TextureCube SkyMap;
SamplerState ss;
float3 ucamPos : POSITION;
struct PS_In
{
	float3 vNormW : NORMAL;
	float3 vposW : POSITION;
	float4 position : SV_POSITION;
};

float4 PShader(PS_In input): SV_TARGET
{
	float3 toEye = ucamPos - input.vposW;
	float3 reflectDir = reflect(normalize(-toEye), normalize(input.vNormW));
	reflectDir.y*=-1.0f;
	float4 colorFinal = SkyMap.Sample(ss,reflectDir);
	return colorFinal;
}