cbuffer ConstantBuffer
{
	float4x4 WVPMatrix;
}

struct VOut
{
	float4 color : COLOR;
	float4 position : SV_POSITION;
};

struct VIn
{
	float3 position: POSITION;
	float4 color: COLOR;
};

VOut VShader(VIn input)
{
	VOut output;
	
	float4 normalizedPos = float4(input.position,1.0f);

    output.position = mul(WVPMatrix, normalizedPos);

    output.color = input.color;

    return output;
}