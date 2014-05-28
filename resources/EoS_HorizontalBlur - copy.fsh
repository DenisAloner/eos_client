uniform sampler2D Map;

const float blurSize = 1.0/1024.0;

void main (void)
{
   vec4 sum = vec4(0.0);   
   sum += texture2D(Map, vec2(gl_TexCoord[0].x - 4.0*blurSize, gl_TexCoord[0].y)) * 0.05;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x - 3.0*blurSize, gl_TexCoord[0].y)) * 0.09;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x - 2.0*blurSize, gl_TexCoord[0].y)) * 0.12;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x - blurSize, gl_TexCoord[0].y)) * 0.15;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y)) * 0.16;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x + blurSize, gl_TexCoord[0].y)) * 0.15;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x + 2.0*blurSize, gl_TexCoord[0].y)) * 0.12;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x + 3.0*blurSize, gl_TexCoord[0].y)) * 0.09;
   sum += texture2D(Map, vec2(gl_TexCoord[0].x + 4.0*blurSize, gl_TexCoord[0].y)) * 0.05;
   gl_FragColor = sum;
}