cbuffer ConstantBuffer
{
	float4x4 WVPMatrix;
}

struct VOut
{
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float4 position : SV_POSITION;
};

VOut VShader(float3 position:POSITION, float2 texcoord : TEXCOORD)
{
	VOut output;
	output.position = mul(WVPMatrix, float4(position,1.0f));
	output.color = float4(1,1,1,1);
	output.texcoord = texcoord;
	return output;
}
/*struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut VShader(float3 position : POSITION, float4 color : COLOR)
{
    VOut output;
	
	float4 normalizedPos = float4(position,1.0f);

    output.position = mul(WVPMatrix, normalizedPos);

    output.color = color;

    return output;
}*/