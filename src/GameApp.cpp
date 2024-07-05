#include "GameApp.hpp"

#include <iostream>

#ifdef SYS_GL_HEADERS
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#else
#include <glad/gl.h>

#endif

bool GameApp::Init() {

  if(!(glfwInit())) {
    std::cout << "GLFW: failed to initialise" << std::endl;
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_pWindow = glfwCreateWindow(m_WindowSize.x, m_WindowSize.y, 
                               m_WindowTitle.c_str(), nullptr, nullptr);

  if(!(m_pWindow)) {
    std::cout << "GLFW: failed to create window" << std::endl;
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(m_pWindow);
  glfwSwapInterval(1);

#ifndef SYS_GL_HEADERS
  gladLoadGL(glfwGetProcAddress);
#endif

  glViewport(0, 0, m_WindowSize.x, m_WindowSize.y);
  glfwSetFramebufferSizeCallback(m_pWindow, frame_resize_callback);

  glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(m_pWindow, mouse_callback);

  glfwSetScrollCallback(m_pWindow, scroll_callback);

  return true;
}
