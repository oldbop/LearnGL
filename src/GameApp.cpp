#include "GameApp.hpp"

#include <cmath>
#include <iostream>

#ifdef SYS_GL_HEADERS
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#else
#include <glad/gl.h>

#endif

GameApp::GameApp(const std::string &title, int32_t width, int32_t height)
  : m_WindowTitle(title),
    m_WindowWidth(width),
    m_WindowHeight(height),
    m_pWindow(nullptr),
    m_DeltaTime(0.0) {}

GameApp::~GameApp() {
  glfwTerminate();
}

bool GameApp::Init() {

  if (!glfwInit()) {
    std::cout << "GLFW: failed to initialise" << std::endl;
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_pWindow = glfwCreateWindow(m_WindowWidth, m_WindowHeight,
                               m_WindowTitle.c_str(), nullptr, nullptr);

  if (!m_pWindow) {
    std::cout << "GLFW: failed to create window" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(m_pWindow);
  glfwSwapInterval(1);

#ifndef SYS_GL_HEADERS
  gladLoadGL(glfwGetProcAddress);
#endif

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glViewport(0, 0, m_WindowWidth, m_WindowHeight);

  glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetWindowUserPointer(m_pWindow, this);

  auto resize_la = [](GLFWwindow *win, int32_t width, int32_t height) -> void {
    ((GameApp *) glfwGetWindowUserPointer(win))->resize_cb(win, width, height);
  };

  auto mouse_la = [](GLFWwindow *win, double xpos, double ypos) -> void {
    ((GameApp *) glfwGetWindowUserPointer(win))->mouse_cb(win, xpos, ypos);
  };

  auto scroll_la = [](GLFWwindow *win, double xoff, double yoff) -> void {
    ((GameApp *) glfwGetWindowUserPointer(win))->scroll_cb(win, xoff, yoff);
  };

  auto key_la = [](GLFWwindow *win, int32_t key, int32_t scan, int32_t action, int32_t mods) -> void {
    ((GameApp *) glfwGetWindowUserPointer(win))->key_cb(win, key, scan, action, mods);
  };

  glfwSetFramebufferSizeCallback(m_pWindow, resize_la);
  glfwSetCursorPosCallback(m_pWindow, mouse_la);
  glfwSetScrollCallback(m_pWindow, scroll_la);
  glfwSetKeyCallback(m_pWindow, key_la);

  return true;
}

void GameApp::Run() {

  double last_frame = 0.0;

  while (!glfwWindowShouldClose(m_pWindow)) {

    double time = glfwGetTime();

    m_DeltaTime = time - last_frame;
    last_frame  = time;

    double r = (sin(time) + 1.0) / 2.0;

    glClearColor(r, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(m_pWindow);
    glfwPollEvents();
  }
}

void GameApp::resize_cb(GLFWwindow *win, int32_t width, int32_t height) {

  glViewport(0, 0, width, height);

  m_WindowWidth  = width;
  m_WindowHeight = height;
}

void GameApp::mouse_cb(GLFWwindow *win, double xpos, double ypos) {}

void GameApp::scroll_cb(GLFWwindow *win, double xoff, double yoff) {}

void GameApp::key_cb(GLFWwindow *win, int32_t key, int32_t scan, int32_t action, int32_t mods) { 
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(win, 1);
}
