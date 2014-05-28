varying vec2 blurCoordinates[5];
 
void main()
{
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        gl_TexCoord[0]=gl_MultiTexCoord0;
	vec2 singleStepOffset = vec2(1.0/1024.0, 1.0/1024.0);
	blurCoordinates[0] =  gl_TexCoord[0].xy;
	blurCoordinates[1] =  gl_TexCoord[0].xy + singleStepOffset * 1.407333;
	blurCoordinates[2] =  gl_TexCoord[0].xy - singleStepOffset * 1.407333;
	blurCoordinates[3] =  gl_TexCoord[0].xy + singleStepOffset * 3.294215;
	blurCoordinates[4] =  gl_TexCoord[0].xy - singleStepOffset * 3.294215;
}