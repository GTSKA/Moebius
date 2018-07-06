precision mediump float;
varying vec2 v_uv;
//varying vec2 v_Newuv;
uniform sampler2D u_s_2DTextures[3];
uniform float u_Time;
uniform float u_FireAmp;
void main()
{
vec2 disp = texture2D(u_s_2DTextures[2],vec2(v_uv.x, v_uv.y + u_Time)).rg;
vec2 offset = (2.0 * disp - 1.0)*u_FireAmp;
vec2 new_uv = v_uv + offset;
vec4 fireColor = texture2D(u_s_2DTextures[0],new_uv);
vec4 AlphaValue = texture2D(u_s_2DTextures[1],v_uv);
vec4 finalColor = fireColor * vec4(1.0,1.0,1.0,AlphaValue.r);
if( finalColor.a < 0.25)
discard;
gl_FragColor = finalColor;
}