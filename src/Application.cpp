#include "Application.hpp"
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifdef SYS_GL_HEADERS
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

namespace utils {
  inline void loadGL() {}
}
#else
#include <glad/gl.h>

namespace utils {
  inline void loadGL() {
    gladLoadGL(glfwGetProcAddress);
  }
}
#endif

namespace lg {

  bool Application::init() {

    if(!(glfwInit())) {
      std::cout << "GLFW: failed to initialise" << std::endl;
      return false;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_pWindow = glfwCreateWindow(WIDTH, HEIGHT, TITLE.c_str(), NULL, NULL);

    if(!(m_pWindow)) {
      std::cout << "GLFW: failed to create window" << std::endl;
      glfwTerminate();
      return false;
    }

    glfwMakeContextCurrent(m_pWindow);
    utils::loadGL();

    return true;
  }

  void Application::run() {

    float positions[6] = {
      -0.5f,  0.5f,
      -0.5f, -0.5f,
       0.5f, -0.5f
    };

    unsigned int buffer;

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    ShaderSource source = parseShader("../res/shaders/basic.shader");
    unsigned int shader = createShader(source.VertexSource, source.FragmentSource);

    glUseProgram(shader);

    while(!(glfwWindowShouldClose(m_pWindow))) {

      glClear(GL_COLOR_BUFFER_BIT);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      glfwSwapBuffers(m_pWindow);
      glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
    return;
  }

  ShaderSource Application::parseShader(const std::string& path) {

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

  unsigned int Application::compileShader(unsigned int type, const std::string& source) {

    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    int result;

    glShaderSource(id, 1, &src, NULL);
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

  unsigned int Application::createShader(const std::string& vSh, const std::string& fSh) {

    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vSh);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fSh);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
  }
}
