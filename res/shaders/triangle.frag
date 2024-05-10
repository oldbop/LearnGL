#version 330 core

in vec3 Col;
in vec2 TexCoord;

out vec4 rCol;

uniform sampler2D brickT;
uniform sampler2D plateT;

void main() {
  rCol = texture(brickT, TexCoord);
}
