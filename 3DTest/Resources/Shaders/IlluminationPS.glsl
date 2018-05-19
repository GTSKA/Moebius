Texture2D Texture;
SamplerState ss;
cbuffer ConstantBuffer
{
	float3 uAmbientColor;
	float uAmbientWeight;
	float uSpecularPower;
	int uNumLights;
	int uLightTypes[10];
	float3 uPosDirs[10];
	float3 uLightColors[10];
	float3 ucamPos;
}

struct PS_In
{
	float3 wPos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normalW : NORMAL;
	float4 position : SV_POSITION;
};
float3 diffuseCI(float3 Normal, float3 lightDir, int i)
{
	float3 diffColor = max(dot(Normal, -lightDir),0.0)*uLightColors[i];
	return diffColor;
}
float3 specular(float3 lightDir, float3 normal, int i, float posW)
{
	float3 reflectVector = normalize(reflect(lightDir,normal));
	float3 toEye = normalize(ucamPos - posW);
	float3 specColor = pow(max(dot(reflectVector,toEye),0),uSpecularPower)*uLightColors[i];
	return specColor;
}
float4 PShader(PS_In input) : SV_TARGET
{
	float3 Normal = normalize(input.normalW);
	float3 totalDiffuse = float3(0,0,0);
	float3 totalSpecular = float3(0,0,0);
	
	float3 LightDir[10];
	for(int i = 0; i < uNumLights; ++i)
	{
		if(uLightTypes[i] != 0)
			LightDir[i] = normalize(input.wPos - uPosDirs[i]);
		else
			LightDir[i] = uPosDirs[i];
		totalDiffuse += diffuseCI(Normal,LightDir[i],i);
		totalSpecular += specular(LightDir[i],Normal,i,input.wPos);
	}
	float4 ObjColor = Texture.Sample(ss,input.texcoord);
	float4 ColorFinal = float4((uAmbientColor * uAmbientWeight + totalDiffuse *(1.0-uAmbientWeight))*ObjColor.xyz + totalSpecular, ObjColor.w);
	return ColorFinal;
	//return float4(totalSpecular,1.0f);
	
}