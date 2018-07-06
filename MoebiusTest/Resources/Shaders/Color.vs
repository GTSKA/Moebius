attribute vec3 a_posL;
attribute vec4 a_colorL;
uniform mat4 u_WVP;
varying vec4 v_colorL;
void main()
{
	vec4 posL = vec4(a_posL, 1.0);	
	
	v_colorL = a_colorL;
	//gl_Position = u_WVP*posL;
	gl_Position = posL;
} 