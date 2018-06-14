uniform sampler2DArray atlas;
varying vec3 out_Color;
varying vec3 out_Light;
void main (void)
{
 vec4 color = texture(atlas,out_Color);
 vec3 pix;
 pix[0] = clamp(out_Light[0]*color[0],0.0,color[0]);
 pix[1] = clamp(out_Light[1]*color[1],0.0,color[1]);
 pix[2] = clamp(out_Light[2]*color[2],0.0,color[2]);
 gl_FragColor = vec4(pix[0],pix[1],pix[2],color[3]);
}