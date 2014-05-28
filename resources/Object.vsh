#version 130
in vec2 in_vertex;
in vec2 in_coord;
out vec2 coord;
uniform mat4 MVMatrix;
uniform mat4 MPMatrix;
void main (void)
{ 
  vec4 v = vec4( in_vertex[0],in_vertex[1], 0.0, 1.0 );
  gl_Position = MPMatrix*MVMatrix * v;
  coord=in_coord;
}