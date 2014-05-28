#version 330
layout(location = 0) out uint SelectBuffer;
in vec2 coord;
uniform uint Ptr;
uniform sampler2D Map;
void main (void)
{
 vec4 alpha = texture2D(Map,coord);
 SelectBuffer= 25U;
}