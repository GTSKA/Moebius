cbuffer ConstantBuffer
{
	float4x4 WVPMatrix;
	float4x4 wMatrix;
	float TilingFactor;
}
struct VS_In
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};
struct PS_In
{
	float2 uv : TEXCOORD0;
	float2 uvTiling :TEXCOORD1;
	float3 pos : POSITION1;
	float4 position : SV_POSITION;
};

PS_In VShader(VS_In input)
{
	PS_In output;
	output.pos = mul(wMatrix, float4(input.position,1.0f));
	output.uv = input.uv;
	output.uvTiling = input.uv*TilingFactor;
	output.position = mul(WVPMatrix, float4(input.position,1.0f));
	return output;
}