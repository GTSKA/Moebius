Texture2D Texture;
SamplerState ss;

float4 PShader(float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	return color * Texture.Sample(ss, texcoord);
	//return float4(texcoord.x, texcoord.y, 0,1);
}

/*float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}*/