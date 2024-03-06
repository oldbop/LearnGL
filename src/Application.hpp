#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <string>

namespace lg {

  const int WIDTH = 600;
  const int HEIGHT = 600;
  const float RATIO = WIDTH / (float) HEIGHT;
  const std::string TITLE = "LearnGL";

  typedef struct {

    std::string VertexSource;
    std::string FragmentSource;

  } ShaderSource;
  
  class Application {
  public:
    bool init();
    void run();
  private:
    static unsigned int createShader(const std::string& vSh, const std::string& fSh);
    static unsigned int compileShader(unsigned int type, const std::string& source);
    static ShaderSource parseShader(const std::string& path);
    GLFWwindow *m_pWindow;
  };
}

#endif
