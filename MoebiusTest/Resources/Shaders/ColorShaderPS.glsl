struct PIn
{
	float4 color : COLOR;
	float4 position : SV_POSITION;
};

float4 PShader(PIn input) : SV_TARGET
{
	return input.color;
}