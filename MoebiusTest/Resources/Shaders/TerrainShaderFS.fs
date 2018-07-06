precision mediump float;
varying vec2 v_uv;
varying vec2 v_uvTiling;
uniform sampler2D u_s_2DTextures[4];
uniform vec3 u_camPos;
uniform float u_fogStart;
uniform float u_fogRange;
uniform vec3 u_fogColor;
varying vec3 v_Pos;

void main()
{
    vec4 Grass = texture2D(u_s_2DTextures[0], v_uvTiling);
    vec4 Dirt = texture2D(u_s_2DTextures[1], v_uvTiling);
    vec4 Rock = texture2D(u_s_2DTextures[2], v_uvTiling);
    vec4 Blend = texture2D(u_s_2DTextures[3], v_uv);
    vec4 colorTerrain = (Dirt*Blend.x + Rock*Blend.y + Grass*Blend.z)/(Blend.x + Blend.y + Blend.z);
    float distanceToCamera = distance(u_camPos,v_Pos);
    vec4 fogColor;
    fogColor.x = u_fogColor.x;
    fogColor.y = u_fogColor.y;
    fogColor.z = u_fogColor.z;
    fogColor.w = 1.0;
    float factor = clamp((distanceToCamera - u_fogStart)/u_fogRange,0.0,1.0);
    vec4 colorFinal = mix(colorTerrain, fogColor, factor);
    gl_FragColor = colorFinal;//colorTerrain;
    //gl_FragColor = vec4(u_camPos/u_camPos.z,1.0);
}