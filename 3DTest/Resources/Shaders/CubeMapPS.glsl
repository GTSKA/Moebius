TextureCube SkyMap;
SamplerState ss;
struct PS_In
{
	float3 tex : TEXCOORD;
	float4 position : SV_POSITION;
};
float4 PShader(PS_In input) : SV_TARGET
{
	float3 texcoord = input.tex;
	texcoord.y *= -1.0f; //necesito arreglar el espacio en directX
	float4 colorFinal = SkyMap.Sample(ss,texcoord);
	return colorFinal;
	//return float4(0.0f,0.0f,1.0f,1.0f);
}