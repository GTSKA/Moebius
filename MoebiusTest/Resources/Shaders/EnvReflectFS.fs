precision mediump float;
uniform vec3 u_camPos;
varying vec3 v_normW;
varying vec3 v_posW;
uniform samplerCube u_s_CubeTexture;
void main()
{
   vec3 toEye = u_camPos - v_posW;
   vec3 reflectDir = reflect(normalize(-toEye), normalize(v_normW));
   gl_FragColor = textureCube(u_s_CubeTexture, reflectDir);
}