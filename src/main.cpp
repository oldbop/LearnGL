#include "ShaderProgram.hpp"

#include <iostream>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef SYS_GL_HEADERS
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

inline void loadGL() {}
#else
#include <glad/gl.h>

inline void loadGL() {
  gladLoadGL(glfwGetProcAddress);
}
#endif

const int WIDTH = 600;
const int HEIGHT = 600;
const float RATIO = WIDTH / (float) HEIGHT;
const std::string TITLE = "LearnGL";

void frame_callback(GLFWwindow *win, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *win) {
  if(glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(win, true);
  }
}

int main(int argc, const char **argv) {

  if(!(glfwInit())) {
    std::cout << "GLFW: failed to initialise" << std::endl;
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *win = glfwCreateWindow(WIDTH, HEIGHT, TITLE.c_str(),
    nullptr, nullptr);

  if(!(win)) {
    std::cout << "GLFW: failed to create window" << std::endl;
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(win);
  glfwSwapInterval(1);

  loadGL();

  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(win, frame_callback);

  float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  // A Vertex Array Object is created and bound. All of the VBO's and EBO's
  // that are bound after this are associated with this object.
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // A Vertex Buffer Object is created on the GPU, then our vertex 
  // data is sent to it
  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Setting and enabling the position vertex attribute.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
    (void *) 0);
  
  glEnableVertexAttribArray(0);

  // Setting and enabling the color vertex attribute.
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
    (void *) (3 * sizeof(float)));

  glEnableVertexAttribArray(1);

  ShaderProgram sh1;
  
  sh1.CompileShader(GL_VERTEX_SHADER, "../res/shaders/triangle.vert");
  sh1.CompileShader(GL_FRAGMENT_SHADER, "../res/shaders/triangle.frag");
  sh1.CreateProgram();
  sh1.Use();

  // Wireframe mode (disable with glPolygonMode(GL_FRONT_AND_BACK, GL_FILL))
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while(!(glfwWindowShouldClose(win))) {

    process_input(win);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
