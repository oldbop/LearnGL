#include "ShaderProgram.hpp"

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

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

typedef struct {
  float x, y;
} Dimensions;

Dimensions screen = { 1000.0f, 700.0f };
const std::string TITLE = "LearnGL";

void frame_callback(GLFWwindow *win, int width, int height) {

  glViewport(0, 0, width, height);

  screen.x = width;
  screen.y = height;
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

  GLFWwindow *win = glfwCreateWindow(screen.x, screen.y, TITLE.c_str(),
                                     nullptr, nullptr);

  if(!(win)) {
    std::cout << "GLFW: failed to create window" << std::endl;
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(win);
  glfwSwapInterval(1);

  loadGL();

  glViewport(0, 0, screen.x, screen.y);
  glfwSetFramebufferSizeCallback(win, frame_callback);

  stbi_set_flip_vertically_on_load(true);

  // Vertex data: some are redundant with only one texture
  float vertices[] = {
    // Positions           // Texture Coords
    -0.2f, -0.2f, -0.2f,   0.0f, 0.0f,
    -0.2f, -0.2f,  0.2f,   1.0f, 0.0f,
    -0.2f,  0.2f, -0.2f,   0.0f, 1.0f,
    -0.2f,  0.2f,  0.2f,   1.0f, 1.0f,

     0.2f, -0.2f, -0.2f,   1.0f, 0.0f,
     0.2f, -0.2f,  0.2f,   0.0f, 0.0f,
     0.2f,  0.2f, -0.2f,   1.0f, 1.0f,
     0.2f,  0.2f,  0.2f,   0.0f, 1.0f,

    -0.2f, -0.2f, -0.2f,   1.0f, 1.0f,
    -0.2f, -0.2f,  0.2f,   1.0f, 0.0f,
     0.2f, -0.2f, -0.2f,   0.0f, 1.0f,
     0.2f, -0.2f,  0.2f,   0.0f, 0.0f,

    -0.2f,  0.2f, -0.2f,   1.0f, 1.0f,
    -0.2f,  0.2f,  0.2f,   1.0f, 0.0f,
     0.2f,  0.2f, -0.2f,   0.0f, 1.0f,
     0.2f,  0.2f,  0.2f,   0.0f, 0.0f,

    -0.2f, -0.2f, -0.2f,   1.0f, 0.0f,
    -0.2f,  0.2f, -0.2f,   1.0f, 1.0f,
     0.2f, -0.2f, -0.2f,   0.0f, 0.0f,
     0.2f,  0.2f, -0.2f,   0.0f, 1.0f,

    -0.2f, -0.2f,  0.2f,   0.0f, 0.0f,
    -0.2f,  0.2f,  0.2f,   0.0f, 1.0f,
     0.2f, -0.2f,  0.2f,   1.0f, 0.0f,
     0.2f,  0.2f,  0.2f,   1.0f, 1.0f
  };

  unsigned int indices[] = {
     0,  1,  3,  3,  2,  0,
     4,  6,  7,  7,  5,  4,
     8, 10, 11, 11,  9,  8,
    12, 13, 15, 15, 14, 12,
    16, 17, 19, 19, 18, 16,
    20, 22, 23, 23, 21, 20
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

  // A Element Buffer Object is created on the GPU to contain the indices of
  // the vertices used to draw a shape.
  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // Setting and enabling the position vertex attribute.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *) 0);
  
  glEnableVertexAttribArray(0);

  // Setting and enabling the texture coordinate vertex attribute.
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *) (3 * sizeof(float)));

  glEnableVertexAttribArray(1);

  // Brick texture
  unsigned int brickT;
  glGenTextures(1, &brickT);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, brickT);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  int brickW, brickH, brickChs;

  unsigned char *brickData = stbi_load("../res/textures/plate16.png", &brickW,
                                       &brickH, &brickChs, 0);

  if(brickData) {
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, brickW, brickH, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, brickData);

    glGenerateMipmap(GL_TEXTURE_2D);

  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(brickData);

  // The ShaderPorgram class is instantiated, shaders are compiled, and 
  // the program is created and used.
  ShaderProgram sh1;
  
  sh1.CompileShader(GL_VERTEX_SHADER, "../res/shaders/triangle.vert");
  sh1.CompileShader(GL_FRAGMENT_SHADER, "../res/shaders/triangle.frag");
  sh1.CreateProgram();
  sh1.Use();

  sh1.SetInt("brickT", 0);

  // Wireframe mode (disable with glPolygonMode(GL_FRONT_AND_BACK, GL_FILL))
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  
  /*
  // Model matrix
  glm::mat4 M = glm::mat4(1.0f);
  M = glm::rotate(M, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  glUniformMatrix4fv(glGetUniformLocation(sh1.GetID(), "Model"), 1, GL_FALSE,
                                          glm::value_ptr(M));
  */

  /*
  // View matrix
  glm::mat4 V = glm::mat4(1.0f);
  V = glm::translate(V, glm::vec3(0.0f, 0.0f, -3.0f));
  
  glUniformMatrix4fv(glGetUniformLocation(sh1.GetID(), "View"), 1, GL_FALSE,
                                          glm::value_ptr(V));
  */

  // Projection matrix
  glm::mat4 P = glm::mat4(1.0f);
  P = glm::perspective(glm::radians(45.0f), screen.x / screen.y, 0.1f, 100.0f);
  
  glUniformMatrix4fv(glGetUniformLocation(sh1.GetID(), "Proj"), 1, GL_FALSE,
                                          glm::value_ptr(P));

  int nCubes = 5;
  srand(time(0));

  std::vector<glm::vec3> pos;

  for(int i = 0; i < nCubes; ++i) {

    float u = (((float) rand()) / (((float) RAND_MAX) / 2.0f)) - 1.0f;
    float v = (((float) rand()) / (((float) RAND_MAX) / 2.0f)) - 1.0f;
    float w = (((float) rand()) / (((float) RAND_MAX) / 2.0f)) - 1.0f;

    pos.push_back(glm::vec3(u, v, w));
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  while(!(glfwWindowShouldClose(win))) {

    process_input(win);

    float time = (float) glfwGetTime();

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const float radius = 10.0f;
    float camX = sin(time * radius);
    float camZ = cos(time * radius);

    glm::mat4 V = glm::lookAt(glm::vec3(camX, 0.0f, camZ),
                              glm::vec3(0.0f, 0.0f, 0.0f),
                              glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(glGetUniformLocation(sh1.GetID(), "View"), 1, GL_FALSE,
                                            glm::value_ptr(V));

    for(const auto &v : pos) {

      glm::mat4 M = glm::mat4(1.0f);
      M = glm::translate(M, v);
      M = glm::rotate(M, glm::radians(time * 100), glm::vec3(0.5f, 1.0f, 0.0f));

      glUniformMatrix4fv(glGetUniformLocation(sh1.GetID(), "Model"), 1,
                         GL_FALSE, glm::value_ptr(M));

      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
