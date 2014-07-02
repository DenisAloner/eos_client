varying vec4 Light;
uniform sampler2D Map;

void main (void)
{
 vec4 color = texture2D(Map,gl_TexCoord[0].xy);
 gl_FragColor=vec4(Light[0]*Light[3]*color[0],Light[1]*Light[3]*color[1],Light[2]*Light[3]*color[2],color[3]);
}