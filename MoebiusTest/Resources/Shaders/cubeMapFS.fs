precision mediump float;
varying vec3 v_Pos;
uniform samplerCube u_s_CubeTexture;
void main()
{
    gl_FragColor = textureCube(u_s_CubeTexture, v_Pos);
}