uniform vec4 light;
uniform sampler2D Map;
void main (void)
{
 vec4 color = texture2D(Map,gl_TexCoord[0].xy);
 vec3 pix;
 float scale=(color[0]*0.21+color[1]*0.72+color[3]*0.07);
 scale = clamp(scale,0.0,scale);
 pix[0] = scale;
 pix[1] = scale;
 pix[2] = scale;
 gl_FragColor = vec4(pix[0],pix[1],pix[2],color[3]);
}