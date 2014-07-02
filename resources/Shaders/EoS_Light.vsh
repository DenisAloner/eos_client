varying vec4 Light;
uniform	vec4 LightPos;
uniform	vec4 LightInfo;

void main (void)
{
  float Power=min(LightInfo[3]/(sqrt((LightPos[2]-LightPos[0])*(LightPos[2]-LightPos[0])+(LightPos[3]-LightPos[1])*(LightPos[3]-LightPos[1]))),1.0);
  Light[0]=LightInfo[0];
  Light[1]=LightInfo[1];
  Light[2]=LightInfo[2];
  Light[3]=Power;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_TexCoord[0]=gl_MultiTexCoord0;
}