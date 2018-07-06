attribute vec3 a_posL;
attribute vec2 a_uv;
varying vec2 v_uv;
//varying vec2 v_Newuv;
//uniform sampler2D u_s_2DTextures[3];
//uniform float u_Time;
uniform mat4 u_WVP;
void main()
{
vec4 posL = vec4(a_posL, 1.0);

v_uv = a_uv;
//v_Newuv = new_uv;
gl_Position = u_WVP*posL;
}