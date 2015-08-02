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
  if(color[3]>0.0)
  {
   vec4 t1 = texture2D(Map,vec2(gl_TexCoord[0].x, gl_TexCoord[0].y - TexStep));
   vec4 t2 = texture2D(Map,vec2(gl_TexCoord[0].x, gl_TexCoord[0].y + TexStep));
   vec4 t3 = texture2D(Map,vec2(gl_TexCoord[0].x - TexStep, gl_TexCoord[0].y));
   vec4 t4 = texture2D(Map,vec2(gl_TexCoord[0].x + TexStep, gl_TexCoord[0].y));
   if( t1[3]==0.0 || t2[3]==0.0 || t3[3]==0.0 || t4[3]==0.0)
   {
    gl_FragColor = vec4(0.7,0.1,0.0,1.0);
   }
  }
 }
}