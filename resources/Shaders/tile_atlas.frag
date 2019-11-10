#version 330 core

uniform sampler2DArray atlas;

in vec3 fragmentTexture;
in vec3 fragmentLight;

out vec4 fragColor;

void main(void) {
  vec4 color = texture(atlas, fragmentTexture);
  fragColor[0] = clamp(fragmentLight[0] * color[0], 0.0f, color[0]);
  fragColor[1] = clamp(fragmentLight[1] * color[1], 0.0f, color[1]);
  fragColor[2] = clamp(fragmentLight[2] * color[2], 0.0f, color[2]);
  fragColor[3] = color[3];
}