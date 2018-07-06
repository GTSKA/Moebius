attribute vec3 a_posL;
varying vec3 v_Pos;
uniform mat4 u_WVP;
uniform mat4 u_WMatrix;
void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	v_Pos = a_posL;
	gl_Position = u_WVP*posL;
} 