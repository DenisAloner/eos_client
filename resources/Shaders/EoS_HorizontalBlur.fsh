uniform sampler2D Map;

const float blurSize = 1.0/1024.0;

void main (void)
{
   vec4 sum = vec4(0.0,0.0,0.0,1.0);
   sum += texture2D(Map, vec2(gl_TexCoord[0].x - 4.0*blurSize, gl_TexCoord[0].y)) * 0.0162162162;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x - 3.0*blurSize, gl_TexCoord[0].y)) * 0.0540540541;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x - 2.0*blurSize, gl_TexCoord[0].y)) * 0.1216216216;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x - blurSize, gl_TexCoord[0].y)) * 0.1945945946;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y)) * 0.2270270270;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x + blurSize, gl_TexCoord[0].y)) * 0.1945945946;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x + 2.0*blurSize, gl_TexCoord[0].y)) * 0.1216216216;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x + 3.0*blurSize, gl_TexCoord[0].y)) * 0.0540540541;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x + 4.0*blurSize, gl_TexCoord[0].y)) * 0.0162162162;
   gl_FragColor = sum;
}