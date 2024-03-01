#ifdef SYS_GL_HEADERS
  #include <GL/gl.h>
  #include <GLFW/glfw3.h>

  void loadGL() {}
#else
  #include <glad/gl.h>
  #include <GLFW/glfw3.h>

  void loadGL() {
    gladLoadGL(glfwGetProcAddress);
  }
#endif

#include <Cute/Vec3f.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 800;
const float RATIO = WIDTH / (float) HEIGHT;
const char *TITLE = "LearnGL";

int main(int argc, const char **argv) {

  uint32_t texture;

  if(!(glfwInit())) {
    printf("GLFW: failed to initialise.\n");
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  
  GLFWwindow *win = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);

  if(!(win)) {
    printf("GLFW: failed to create window.\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(win);
  loadGL();

  glViewport(0, 0, WIDTH, HEIGHT);
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB,
    GL_UNSIGNED_BYTE, NULL);

  while(!(glfwWindowShouldClose(win))) {

    /*
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();
    */
    
    glfwPollEvents();
  }

  glfwDestroyWindow(win);
  glfwTerminate();
  return EXIT_SUCCESS;
}
