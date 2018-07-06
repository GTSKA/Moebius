cbuffer ConstantBuffer
{
	float4x4 WVPMatrix;
	float4x4 wMatrix;
}
struct vIn
{
	float3 aPos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};
struct vOut
{
	float3 wPos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normalW : NORMAL;
	float4 position : SV_POSITION;
};

vOut VShader(vIn input)
{
	vOut output;
	output.wPos = mul(wMatrix,float4(input.aPos,1.0f)).xyz;
	output.normalW = mul(wMatrix,float4(input.normal,0.0f)).xyz;
	output.texcoord = input.texcoord;
	output.position = mul(WVPMatrix,float4(input.aPos,1.0f));
	return output;
}