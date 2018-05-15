cbuffer ConstantBuffer
{
	//float4x4 WMatrix;
	float4x4 WVPMatrix;
}
Texture2D Texture;
SamplerState ss;
struct VOut
{
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float4 position : SV_POSITION;
};
VOut VShader(float4 position:POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD)
{
	VOut output;
	//output.position = position; //Lesson 3
	output.position = mul(WVPMatrix, position);
	output.color = float4(1,1,1,1);
	output.texcoord = texcoord;
	return output;
}

float4 PShader(float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	return color * Texture.Sample(ss, texcoord);
}