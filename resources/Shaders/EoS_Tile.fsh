uniform int layer_num;
uniform vec4 light;
uniform sampler2D Map;
void main (void)
{
 vec4 color = texture2D(Map,gl_TexCoord[0].xy);
 vec3 pix;
 pix[0] = clamp(light[0]*color[0],0.0,color[0]);
 pix[1] = clamp(light[1]*color[1],0.0,color[1]);
 pix[2] = clamp(light[2]*color[2],0.0,color[2]);
 gl_FragData [0] = vec4(pix[0],pix[1],pix[2],color[3]);
 if(layer_num==1)
 {
  gl_FragData [1] = vec4(pix[0],pix[1],pix[2],color[3]);
 }
}