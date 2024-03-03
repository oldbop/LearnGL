#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#ifdef SYS_GL_HEADERS
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>

namespace utils {
  inline void loadGL() {}
}
#else
#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace utils {
  inline void loadGL() {
    gladLoadGL(glfwGetProcAddress);
  }
}
#endif

#include <string>

namespace lg {

  const int WIDTH = 600;
  const int HEIGHT = 600;
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
