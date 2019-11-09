#version 330 core

uniform sampler2DArray atlas;

in vec3 fragmentTexture;
in vec4 fragmentColor;

out vec4 fragColor;

void main (void) {
 fragColor = texture(atlas,fragmentTexture)*fragmentColor;
}