uniform sampler2D Map;
uniform uint PTR;
void main (void)
{
 vec4 color = texture2D(Map,gl_TexCoord[0].xy);
 gl_FragColor = PTR;
}