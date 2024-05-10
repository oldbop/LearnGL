#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 2) in vec2 aTexCoord;

out vec3 Col;
out vec2 TexCoord;

uniform mat4 M;

void main() {
  gl_Position = M * vec4(aPos, 1.0);
  Col = aCol;
  TexCoord = aTexCoord;
}
