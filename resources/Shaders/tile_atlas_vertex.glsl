#version 330 core

uniform mat4 transform;

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec3 vertexTexture;
layout (location = 2) in vec3 vertexLight;

out vec3 fragmentTexture;
out vec3 fragmentLight;

void main (void) { 
  gl_Position = transform * vec4(vertexPosition, 0.0f, 1.0f);
  fragmentTexture = vertexTexture;
  fragmentLight = vertexLight;
}