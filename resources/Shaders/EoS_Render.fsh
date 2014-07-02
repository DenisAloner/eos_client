uniform sampler2D Map;
void main (void)
{
 vec4 color = texture2D(Map,gl_TexCoord[0].xy);
 gl_FragColor = vec4(0,0,0,color[3]);
}