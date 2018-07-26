Texture2D Texture[3];
SamplerState samplerDD[3];

TextureCube SkyMap;
SamplerState samplerCube;
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
	float uFresnelPower;
	float3 uWaterColor;
	float uDepthAdjust;
	float uTime;

	float uDepthAdjustDisplacement;
	float uMaxReflection;
	
}
float fresnelTerm(float3 Normal, float3 toEye)
{
	float lFresnetTerm;
	lFresnetTerm = pow(1.0f - abs(dot(Normal, normalize(toEye))), uFresnelPower);
	return lFresnetTerm;
}
float3 diffuseCI(float3 Normal, float3 lightDir, int i)
{
	float3 diffColor = max(dot(Normal, -lightDir),0.0)*uLightColors[i];
	return diffColor;
}
float3 specular(float3 lightDir, float3 normal, int i, float3 toEye)
{
	float3 reflectVector = normalize(reflect(lightDir,normal));
	float3 specColor = pow(max(dot(reflectVector,toEye),0),uSpecularPower)*uLightColors[i];
	return specColor;
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
float4 PShader(PS_In input) : SV_TARGET
{
	float3 Normal = Texture[1].Sample(samplerDD[1],input.texcoord).xyz;
	float3x3 TBN = float3x3(normalize(input.tanW), normalize(input.bNormW), normalize(input.normalW)); 
	float3 NormW = normalize(mul(TBN,2.0f*Normal - 1.0f));
	float3 toEye = normalize(ucamPos - input.wPos);
	
	float depthFactor = clamp(uDepthAdjust*input.texcoord.x,0.0f,1.0f);
	
	float2 disp = Texture[2].Sample(samplerDD[2],float2(input.texcoord.x + uTime, input.texcoord.y)).xy;
	float dMaxRefraction = input.texcoord.x * uDepthAdjustDisplacement;
	
	float2 offsetRefraction = (2.0f * disp - 1.0)*dMaxRefraction;
	float2 newtexcoord = input.texcoord + offsetRefraction;
	float3 bottomColor = Texture[0].Sample(samplerDD[0],newtexcoord).xyz;
	
	float3 refractionColor = lerp(bottomColor, uWaterColor, depthFactor);
	
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
		totalSpecular += specular(LightDir[i],NormW,i,toEye);
	}
	float3 NrefractionColor = (uAmbientColor * uAmbientWeight + totalDiffuse * (1.0 - uAmbientWeight)) * refractionColor;
	
	float2 offsetReflection = (2.0f*disp -1.0f)*uMaxReflection;
	newtexcoord = input.texcoord + offsetReflection;
	float3 reflectionNormal = Texture[1].Sample(samplerDD[1],newtexcoord).xyz;
	float3 Norm2W = normalize(mul(TBN,2.0f*reflectionNormal-1.0f));
	
	float3 reflectDir = reflect(-toEye, Norm2W);
	float3 reflectColor = SkyMap.Sample(samplerCube, reflectDir).xyz;
	float FT = fresnelTerm(NormW, toEye);
	float4 ColorFinal = float4(lerp(NrefractionColor,reflectColor,FT),1.0);
	return ColorFinal;
	//return float4(disp,0.0f,1.0f);
}