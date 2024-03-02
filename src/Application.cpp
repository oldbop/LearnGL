#include "Application.hpp"
#include <iostream>

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

    while(!(glfwWindowShouldClose(m_pWindow))) {

      glClear(GL_COLOR_BUFFER_BIT);

      glBegin(GL_TRIANGLES);
      glVertex2f(0.0f, 0.0f);
      glVertex2f(-0.5f, -0.5f);
      glVertex2f(0.5f, -0.5f);
      glEnd();

      glfwSwapBuffers(m_pWindow);
      glfwPollEvents();
    }

    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
    return;
  }
}
