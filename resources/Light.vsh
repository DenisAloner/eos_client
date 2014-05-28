#version 130
in vec2 in_vertex;
in vec2 in_coord;
out vec2 coord;
out vec4 Light;
uniform	vec4 LightPos;
uniform	vec4 LightInfo;
uniform mat4 MVMatrix;
uniform mat4 MPMatrix;
void main (void)
{
  float Power=min(LightInfo[3]/(sqrt((LightPos[2]-LightPos[0])*(LightPos[2]-LightPos[0])+(LightPos[3]-LightPos[1])*(LightPos[3]-LightPos[1]))),1);
  Light[0]=LightInfo[0];
  Light[1]=LightInfo[1];
  Light[2]=LightInfo[2];
  Light[3]=Power;
  vec4 v = vec4( in_vertex[0],in_vertex[1], 0.0, 1.0 );
  gl_Position =MPMatrix* v;
  coord=in_coord;
}