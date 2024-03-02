#ifndef APPLICATION_H
#define APPLICATION_H

#ifdef SYS_GL_HEADERS
  #include <GL/gl.h>
  #include <GLFW/glfw3.h>

  namespace lg {
    inline void loadGL() {}
  }
#else
  #include <glad/gl.h>
  #include <GLFW/glfw3.h>

  namespace lg {
    inline void loadGL() {
      gladLoadGL(glfwGetProcAddress);
    }
  }
#endif

#include <string>

namespace lg {

  const unsigned int WIDTH = 600;
  const unsigned int HEIGHT = 600;
  const float RATIO = WIDTH / (float) HEIGHT;
  const std::string TITLE = "LearnGL";

  class Application {
  public:
    bool init();
    void run();
  private:
    GLFWwindow *m_pWindow;
  };
}

#endif
