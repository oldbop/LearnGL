#version 330 core

out vec4 color;
uniform vec4 cpu_color;

void main() {
  color = cpu_color;
}
