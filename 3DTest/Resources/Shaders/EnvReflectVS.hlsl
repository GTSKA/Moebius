cbuffer ConstantBuffer
{
	float4x4 wMatrix;
	float4x4 WVPMatrix;
}

struct vIn
{
	float3 aPos : POSITION;
	float3 aNorm : NORMAL;
};
struct vOut
{
	float3 vNormW : NORMAL;
	float3 vposW : POSITION;
	float4 position : SV_POSITION;
};

vOut VShader(vIn input)
{
	vOut output;
	output.vposW = mul(wMatrix,float4(input.aPos,1.0f)).xyz;
	output.vNormW = mul(wMatrix,float4(input.aNorm,0.0f)).xyz;
	output.position = mul(WVPMatrix, float4(input.aPos,1.0f));
	return output;
}