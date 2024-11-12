#ifndef GAME_APP_HPP
#define GAME_APP_HPP

#include <cstdint>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class GameApp {
public:
  GameApp(const std::string &title, int32_t width, int32_t height);
  virtual ~GameApp();
  bool Init();
  void Run();
private:
  void resize_cb(GLFWwindow *win, int32_t width, int32_t height);
  void mouse_cb(GLFWwindow *win, double xpos, double ypos);
  void scroll_cb(GLFWwindow *win, double xoff, double yoff);
  void key_cb(GLFWwindow *win, int32_t key, int32_t scan, int32_t action, int32_t mods);
  std::string m_WindowTitle;
  int32_t m_WindowWidth;
  int32_t m_WindowHeight;
  GLFWwindow *m_pWindow;
  double m_DeltaTime;
};

#endif
