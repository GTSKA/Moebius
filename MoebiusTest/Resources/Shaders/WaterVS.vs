attribute vec3 a_posL;
attribute vec2 a_uv;
attribute vec3 a_normL;
attribute vec3 a_binormL;
attribute vec3 a_tanL;
varying vec3 v_normW;
varying vec3 v_posW;
varying vec2 v_uv;
varying vec3 v_binormW;
varying vec3 v_tanW;
uniform mat4 u_WVP;
uniform mat4 u_WMatrix;
void main()
{
vec4 posL = vec4(a_posL, 1.0);
vec3 posW = (u_WMatrix*posL).xyz;
vec4 normL = vec4(a_normL, 0.0);
vec4 binormL = vec4(a_binormL, 0.0);
vec4 tanL = vec4(a_tanL, 0.0);

v_normW = normalize((u_WMatrix*normL).xyz);
v_binormW = normalize((u_WMatrix*binormL).xyz);
v_tanW = normalize((u_WMatrix*tanL).xyz);
v_posW = posW;
v_uv = a_uv;
gl_Position = u_WVP*posL;
}
   