#version 330
layout(location = 0) out vec4 ColorBuffer;
in vec2 coord;
uniform sampler2D Map;
void main (void)
{
 vec4 alpha = texture2D(Map,coord);
 ColorBuffer = vec4(0,0,0,alpha[3]);
}