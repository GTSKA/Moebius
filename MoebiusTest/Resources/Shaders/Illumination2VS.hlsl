cbuffer ConstantBuffer
{
	float4x4 WVPMatrix;
	float4x4 wMatrix;
}
struct vIn
{
	float3 aPos : POSITION;
	float2 aTexc : TEXCOORD;
	float3 aNorm : NORMAL;
	float3 aBinorm : BINORMAL;
	float3 aTan : TANGENT;
};
struct vOut
{
	float3 wPos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normalW : NORMAL;
	float3 bNormW : BINORMAL;
	float3 tanW : TANGENT;
	float4 position : SV_POSITION;
};

vOut VShader(vIn input)
{
	vOut output;
	output.wPos = mul(wMatrix,float4(input.aPos,1.0f)).xyz;
	output.normalW = normalize(mul(wMatrix,float4(input.aNorm,0.0f))).xyz;
	output.bNormW = normalize(mul(wMatrix,float4(input.aBinorm,0.0f))).xyz;
	output.tanW = normalize(mul(wMatrix,float4(input.aTan,0.0f))).xyz;
	output.texcoord = input.aTexc;
	output.position = mul(WVPMatrix,float4(input.aPos,1.0f));
	return output;
}