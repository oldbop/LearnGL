#include <cassert>
#include <fstream>
#include <iostream>
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

#define GL_CALL(x) gl_clear_error(); x; assert(gl_log_call())

const int WIDTH = 800;
const int HEIGHT = 600;
const float RATIO = WIDTH / (float) HEIGHT;
const std::string TITLE = "LearnGL";

typedef struct {
  std::string VertexSource;
  std::string FragmentSource;
} ShaderSource;

void gl_clear_error(void) { while(glGetError()); }

bool gl_log_call(void) {
  
  while(GLenum err = glGetError()) {
    std::cout << "GL Error: " << err << std::endl;
    return false;
  }

  return true;
}

ShaderSource parse_shader(const std::string& path) {

  enum ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  std::ifstream stream(path);
  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;

  while(std::getline(stream, line)) {

    if(line.find("#shader") != std::string::npos) {

      if(line.find("vertex") != std::string::npos) {
        type = ShaderType::VERTEX;
      } else if(line.find("fragment") != std::string::npos) {
        type = ShaderType::FRAGMENT;
      }

    } else {
      ss[(int) type] << line << '\n';
    }
  }

  return { ss[0].str(), ss[1].str() };
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

    char *msg = new char[length];

    glGetShaderInfoLog(id, length, &length, msg);
    std::cout << msg << std::endl;

    glDeleteShader(id);

    delete[] msg;
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

  // look into this (try unbinding everything then just bind this vao)
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float),
               positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  unsigned int ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int),
               indices, GL_STATIC_DRAW);

  ShaderSource source = parse_shader("../res/shaders/basic.shader");

  unsigned int program = create_program(source.VertexSource,
                                        source.FragmentSource);

  glUseProgram(program);

  int location = glGetUniformLocation(program, "u_Color");

  float r = 0.0f;
  float inc = 0.05f;

  while(!(glfwWindowShouldClose(win))) {

    glClear(GL_COLOR_BUFFER_BIT);

    glUniform4f(location, r, 0.0f, 0.0f, 1.0f);
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

  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}
