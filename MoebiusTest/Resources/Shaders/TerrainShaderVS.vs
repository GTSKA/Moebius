attribute vec3 a_posL;
attribute vec2 a_uv;
varying vec2 v_uv;
varying vec2 v_uvTiling;
varying vec3 v_Pos;
uniform float u_TilingFactor;
uniform mat4 u_WVP;
uniform mat4 u_WMatrix;
void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	vec3 posW = (u_WMatrix*posL).xyz;
	v_Pos = posW;
	v_uv = a_uv;
	v_uvTiling = v_uv * u_TilingFactor;
	gl_Position = u_WVP*posL;
} 