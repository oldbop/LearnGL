#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef SYS_GL_HEADERS
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

inline void loadGL() {}
#else
#include <glad/gl.h>

inline void loadGL() {
  gladLoadGL(glfwGetProcAddress);
}
#endif

const int WIDTH = 800;
const int HEIGHT = 600;
const float RATIO = WIDTH / (float) HEIGHT;
const std::string TITLE = "LearnGL";

std::string parse_shader(const std::string& path) {

  std::ifstream stream(path);
  std::string line;
  std::stringstream sstream;

  while(std::getline(stream, line)) {
    sstream << line << '\n';
  }

  return sstream.str();
}

unsigned int compile_shader(unsigned int type, const std::string& source) {

  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  int result;

  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if(result == GL_FALSE) {

    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

    std::unique_ptr<char[]> msg(new char[length]);

    glGetShaderInfoLog(id, length, &length, msg.get());
    std::cout << msg.get() << std::endl;

    glDeleteShader(id);

    return 0;
  }

  return id;
}

unsigned int create_program(const std::string& vSh, const std::string& fSh) {

  unsigned int program = glCreateProgram();
  unsigned int vs = compile_shader(GL_VERTEX_SHADER, vSh);
  unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fSh);

  glAttachShader(program, vs);
  glAttachShader(program, fs);

  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main(int argc, const char **argv) {

  if(!(glfwInit())) {
    std::cout << "GLFW: failed to initialise" << std::endl;
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow *win = glfwCreateWindow(WIDTH, HEIGHT, TITLE.c_str(),
                                     nullptr, nullptr);

  if(!(win)) {
    std::cout << "GLFW: failed to create window" << std::endl;
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(win);
  glfwSwapInterval(1);

  loadGL();

  float positions[] = {
    -0.9f, -0.9f,
     0.9f, -0.9f,
     0.9f,  0.9f,
    -0.9f,  0.9f
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
  };

  // the Vertex Array Object stores all of the information related to the
  // buffers bound and configured after it
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  auto vb = std::make_unique<VertexBuffer>(positions, 4 * 2 * sizeof(float));

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  auto ib = std::make_unique<IndexBuffer>(indices, 6);

  std::string vert = parse_shader("../res/shaders/basic.vert");
  std::string frag = parse_shader("../res/shaders/basic.frag");
  
  unsigned int program = create_program(vert, frag);

  glUseProgram(program);

  int location = glGetUniformLocation(program, "u_Color");

  glBindVertexArray(0);
  glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  float r = 0.0f;
  float inc = 0.05f;

  while(!(glfwWindowShouldClose(win))) {

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glUniform4f(location, r, 0.0f, 0.0f, 1.0f);
    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    if(r > 1.0f) {
      inc = -0.05f;
    } else if(r < 0.0f) {
      inc = 0.05f;
    }

    r += inc;

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glDeleteProgram(program);

  ib.reset();
  vb.reset();

  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}
