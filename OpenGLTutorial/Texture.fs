precision mediump float;
varying vec2 v_uv;
uniform sampler2D shaderTexture;
void main()
{
	gl_FragColor = texture2D(shaderTexture,v_uv);
}