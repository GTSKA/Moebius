attribute vec3 a_posL;
attribute vec4 a_colorL;
varying vec4 v_colorL;
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
void main()
{
vec4 posL = vec4(a_posL, 1.0);
gl_Position = worldMatrix * posL;
gl_Position = viewMatrix * gl_Position;
gl_Position = projectionMatrix * gl_Position;
v_colorL = a_colorL;

}