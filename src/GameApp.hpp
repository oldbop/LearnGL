#ifndef GAME_APP_HPP
#define GAME_APP_HPP

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

typedef struct {
  float x, y;
} Dimensions;

class GameApp {
public:
  bool Init();
private:
  Dimensions m_WindowSize = { 1000.0f, 700.0f };
  std::string m_WindowTitle = "LearnGL";
  GLFWwindow *m_pWindow;
};

#endif
