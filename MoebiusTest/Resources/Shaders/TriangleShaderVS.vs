attribute vec3 a_posL;
attribute vec4 a_colorL;
varying vec4 v_colorL;
uniform mat4 u_WMatrix;
uniform mat4 u_WVP;
void main()
{
vec4 posL = vec4(a_posL, 1.0);
gl_Position = u_WVP*posL;
v_colorL = a_colorL;
}
   