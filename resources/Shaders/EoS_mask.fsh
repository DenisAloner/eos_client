uniform sampler2D Map;
void main (void)
{
 vec4 color = texture2D(Map,gl_TexCoord[0].xy);
 gl_FragData [0] = vec4(color[0],color[1],color[2],color[3]*0.5);
}