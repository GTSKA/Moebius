cbuffer ConstantBuffer
{
	float4x4 WVPMatrix;
}

struct vOut
{
	float3 tex : TEXCOORD;
	float4 position : SV_POSITION;
};
struct vIn
{
	float3 vPos : POSITION;
};

vOut VShader(vIn input)
{
	vOut output;
	output.tex = input.vPos;
	output.position = mul(WVPMatrix, float4(input.vPos,1.0f));
	return output;
}


