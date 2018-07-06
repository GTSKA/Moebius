struct vIn
{
	float3 aPos : POSITION;
	float2 uv : TEXCOORD;
};
struct vOut
{
	float2 uv : TEXCOORD;
	float4 pos : SV_POSITION;
};

vOut VShader(vIn input)
{
	vOut output;
	output.uv = input.uv;
	output.pos = float4(input.aPos, 1.0f);
	return output;
}