attribute vec3 a_posL;
attribute vec2 a_uv;
attribute vec3 a_normL;
varying vec3 v_normW;
varying vec3 v_posW;
varying vec2 v_uv;
uniform mat4 u_WVP;
uniform mat4 u_WMatrix;
void main()
{
vec4 posL = vec4(a_posL, 1.0);
vec3 posW = (u_WMatrix*posL).xyz;
vec4 normL = vec4(a_normL, 0.0);
vec3 normW = (u_WMatrix*normL).xyz;

v_posW = posW;
v_normW = normW;
v_uv = a_uv;
gl_Position = u_WVP*posL;
}