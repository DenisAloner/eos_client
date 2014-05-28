#version 330
layout(location = 0) out vec4 ColorBuffer;
in vec2 coord;
in vec4 Light;
uniform sampler2D Map;

void main (void)
{
 vec4 color = texture2D(Map,coord);
 ColorBuffer=vec4(Light[0]*Light[3]*color[0],Light[1]*Light[3]*color[1],Light[2]*Light[3]*color[2],color[3]);
}