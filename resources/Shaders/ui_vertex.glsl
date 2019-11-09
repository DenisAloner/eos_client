#version 330 core

uniform mat4 transform;

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec3 vertexTexture;
layout (location = 2) in vec4 vertexColor;
layout (location = 3) in vec4 clipRect;

out vec3 fragmentTexture;
out vec4 fragmentColor;

void main (void) {
  gl_ClipDistance[0] = vertexPosition.x - clipRect[0];
  gl_ClipDistance[1] = clipRect[2] - vertexPosition.x;
  gl_ClipDistance[2] = vertexPosition.y - clipRect[1];
  gl_ClipDistance[3] = clipRect[3] - vertexPosition.y;
  gl_Position = transform * vec4(vertexPosition, 0.0f, 1.0f);
  fragmentTexture = vertexTexture;
  fragmentColor = vertexColor;
}