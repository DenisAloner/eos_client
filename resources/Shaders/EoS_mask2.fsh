uniform sampler2D Map1;
uniform sampler2D Unit;
uniform sampler2D Map2;
const float TexStep = 1.0/1024.0;

void main (void)
{
 vec4 color1 = texture2D(Map1,gl_TexCoord[0].xy);
 vec4 color2 = texture2D(Map2,gl_TexCoord[0].xy);
 vec4 unitv = texture2D(Unit,gl_TexCoord[1].xy);
 vec4 res;
 if(color2[3]==0.0)
 {
 res = vec4(color1[0],color1[1],color1[2],color1[3]);
 }
 else
 {
 res = vec4(0.0,0,0,0.0);
 }
 if(unitv[3]>0.1)
 {
  gl_FragColor = vec4(res[0],res[1],res[2],res[3]*0.5);
 }
}