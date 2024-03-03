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

    float positions[6] = {
      -0.5f, 0.5f,
      -0.5f, -0.5f,
      0.5f, -0.5f
    };

    unsigned int buffer;
    
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while(!(glfwWindowShouldClose(m_pWindow))) {

      glClear(GL_COLOR_BUFFER_BIT);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      
      glfwSwapBuffers(m_pWindow);
      glfwPollEvents();
    }

    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
    return;
  }
}
