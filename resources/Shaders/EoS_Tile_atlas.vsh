attribute vec4 in_Position;
attribute vec3 in_Color;
attribute vec3 in_Light;
varying vec3 out_Color;
varying vec3 out_Light;
void main (void)
{ 
  gl_Position = gl_ModelViewProjectionMatrix * in_Position;
  out_Color = in_Color;
  out_Light = in_Light;
}