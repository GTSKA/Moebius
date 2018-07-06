precision mediump float;

varying vec3 v_normW;
varying vec3 v_posW;
varying vec2 v_uv;
varying vec3 v_binormW;
varying vec3 v_tanW;

uniform vec3 u_ambient_color;
uniform float u_ambient_weight;
uniform float u_specular_power;
uniform int u_numLights;
uniform int u_LightType[10];
uniform vec3 u_pos_dir[10];
uniform vec3 u_lightColor[10];

uniform vec3 u_camPos;
uniform sampler2D u_s_2DTextures[4];
uniform samplerCube u_s_CubeTexture;
uniform float u_FresnelPower;
uniform vec3 u_WaterColor;
uniform float u_depthAdjust;
uniform float u_Time;

uniform float u_depthAdjustDisplacement;
uniform float u_maxReflection;



float fresnelTerm(vec3 Normal, vec3 toEye)
{
    float lFresnelTerm;
    lFresnelTerm = pow(1.0 - abs(dot(Normal,normalize(toEye))), u_FresnelPower);
    return lFresnelTerm;
}

vec3 LightDir(int i)
{
vec3 LDir = vec3(0.0,0.0,0.0);
if(u_LightType[i] != 0)
{
LDir = normalize(v_posW - u_pos_dir[i]);
}
else
{
LDir = u_pos_dir[i];
}
return LDir;
}

vec3 diffuseCI(vec3 Normal, vec3 lightDirection, int i)
{
vec3 diffColor = max(dot(Normal, -lightDirection), 0.0)* u_lightColor[i];
return diffColor;
}

vec3 specular(vec3 lightDirection, vec3 Normal, int i, vec3 toEye)
{
vec3 reflectVector = normalize(reflect(lightDirection, Normal));
vec3 specColor = pow( max(dot(reflectVector,toEye),0.0) , u_specular_power) * u_lightColor[i];
return specColor;
}

void main()
{
vec3 Normal = texture2D(u_s_2DTextures[1], v_uv).xyz;
mat3 TBN = mat3(normalize(v_tanW),normalize(v_binormW),normalize(v_normW));
vec3 NormW = normalize(TBN*(2.0*Normal - 1.0));
vec3 toEye = normalize(u_camPos - v_posW);


float depthFactor = clamp(u_depthAdjust * v_uv.x,0.0,1.0);
//vec3 bottomColor = texture2D(u_s_2DTextures[0], v_uv).xyz;

vec2 disp = texture2D(u_s_2DTextures[2],vec2(v_uv.x + u_Time, v_uv.y)).rg;
float dMaxRefraction = v_uv.x * u_depthAdjustDisplacement;
vec2 offsetRefraction = (2.0 * disp - 1.0)*dMaxRefraction;
vec2 new_uv = v_uv + offsetRefraction;
vec3 bottomColor = texture2D(u_s_2DTextures[0], new_uv).xyz;

vec3 refractionColor = mix(bottomColor, u_WaterColor, depthFactor);

vec3 totalDiffuse = vec3(0.0,0.0,0.0);
vec3 totalSpecular = vec3(0.0,0.0,0.0);
vec3 lightDirection[10];
for(int i = 0; i < u_numLights; ++i)
{
lightDirection[i] = LightDir(i);
totalDiffuse += diffuseCI(NormW,lightDirection[i],i);
totalSpecular += specular(lightDirection[i],NormW,i,toEye);
}
vec3 NrefractionColor = (u_ambient_color * u_ambient_weight + totalDiffuse * (1.0 - u_ambient_weight)) * refractionColor;// + totalSpecular;

vec2 offsetReflection = (2.0 * disp - 1.0)*u_maxReflection;
new_uv = v_uv + offsetReflection;
vec3 reflectionNormal = texture2D(u_s_2DTextures[1], new_uv).xyz;
vec3 Norm2W = normalize(TBN*(2.0*reflectionNormal - 1.0));

vec3 reflectDir = reflect(-toEye,Norm2W);
vec3 reflectionColor = textureCube(u_s_CubeTexture, reflectDir).xyz;
float FT = fresnelTerm(Norm2W, toEye);
gl_FragColor = vec4(mix(NrefractionColor,reflectionColor,FT),1.0);
}
