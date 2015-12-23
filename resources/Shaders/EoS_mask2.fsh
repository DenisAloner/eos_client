uniform sampler2D Map;
uniform sampler2D Unit;

const float TexStep = 1.0/1024.0;

void main (void)
{
 vec4 color = texture2D(Map,gl_TexCoord[0].xy);
 vec4 unitv = texture2D(Unit,gl_TexCoord[1].xy);
 if(unitv[3]>0.1)
 {
  gl_FragColor = vec4(color[0],color[1],color[2],color[3]*0.5);
 }
}