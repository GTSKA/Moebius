Texture2D Texture[2];
SamplerState samplerDD[2];
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
	float3 bNormW : BINORMAL;
	float3 tanW : TANGENT;
	float4 position : SV_POSITION;
};
float3 diffuseCI(float3 Normal, float3 lightDir, int i)
{
	float3 diffColor = max(dot(Normal, lightDir),0.0)*uLightColors[i];
	return diffColor;
}
float3 specular(float3 lightDir, float3 normal, int i, float3 posW)
{
	float3 reflectVector = normalize(reflect(lightDir,normal));
	float3 toEye = normalize(ucamPos - posW);
	float3 specColor = pow(max(dot(reflectVector,toEye),0),uSpecularPower)*uLightColors[i];
	return specColor;
}
float4 PShader(PS_In input) : SV_TARGET
{
	float3 Normal = Texture[1].Sample(samplerDD[1],input.texcoord).xyz;
	float3x3 TBN=float3x3(normalize(input.tanW), normalize(input.bNormW), normalize(input.normalW)); 
	float3 NormW = normalize(mul(TBN,2.0*Normal - 1.0));
	
	float3 totalDiffuse = float3(0,0,0);
	float3 totalSpecular = float3(0,0,0);
	
	float3 LightDir[10];
	for(int i = 0; i < uNumLights; ++i)
	{
		if(uLightTypes[i] != 0)
			LightDir[i] = normalize(input.wPos - uPosDirs[i]);
		else
			LightDir[i] = uPosDirs[i];
		totalDiffuse += diffuseCI(NormW,LightDir[i],i);
		totalSpecular += specular(LightDir[i],NormW,i,input.wPos);
	}
	float4 ObjColor = Texture[0].Sample(samplerDD[0],input.texcoord);
	float4 ColorFinal = float4((uAmbientColor * uAmbientWeight + totalDiffuse *(1.0-uAmbientWeight))*ObjColor.xyz + totalSpecular, ObjColor.w);
	return ColorFinal;
	//return float4(NormW,1.0f);
	
}