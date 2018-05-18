cbuffer ConstantBuffer
{
	float4x4 WVPMatrix;
}

struct vOut
{
	float2 texcoord : TEXCOORD;
	float4 position : SV_POSITION;
};
struct vIn
{
	float3 aPos : POSITION;
	float2 texcoord : TEXCOORD;
};
vOut VShader(vIn input)
{
	vOut output;
	output.texcoord = input.texcoord;
	output.position = mul(WVPMatrix,float4(input.aPos,1.0f));
	return output;
}