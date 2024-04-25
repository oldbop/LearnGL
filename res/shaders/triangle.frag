#version 330 core

out vec4 rCol;
in vec3 fCol;

void main() {
  rCol = vec4(fCol, 1.0);
}
