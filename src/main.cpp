#include "ShaderProgram.hpp"

#include <cmath>
#include <iostream>
#include <string>

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

  stbi_set_flip_vertically_on_load(true);

  // Vertex data
  float vertices[] = {
    // Positions          // Colours          // Texture Coords
    -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f
  };

  // This data is used by the Element Buffer Object. It specifies how to draw
  // a square using two triangles while reusing some vertices.
  unsigned int indices[] = {
    0, 1, 2,
    1, 2, 3
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *) 0);
  
  glEnableVertexAttribArray(0);

  // Setting and enabling the color vertex attribute.
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *) (3 * sizeof(float)));

  glEnableVertexAttribArray(1);

  // Setting and enabling the texture coordinate vertex attribute.
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *) (6 * sizeof(float)));

  glEnableVertexAttribArray(2);

  // Brick texture
  unsigned int brickT;
  glGenTextures(1, &brickT);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, brickT);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int brickW, brickH, brickChs;

  unsigned char *brickData = stbi_load("../res/textures/brick16.png", &brickW,
                                       &brickH, &brickChs, 0);

  if(brickData) {
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, brickW, brickH, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, brickData);

    glGenerateMipmap(GL_TEXTURE_2D);

  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(brickData);

  // Plate texture
  unsigned int plateT;
  glGenTextures(1, &plateT);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, plateT);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int plateW, plateH, plateChs;

  unsigned char *plateData = stbi_load("../res/textures/plate16.png", &plateW,
                                       &plateH, &plateChs, 0);

  if(plateData) {
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, plateW, plateH, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, plateData);

    glGenerateMipmap(GL_TEXTURE_2D);

  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(plateData);

  // The ShaderPorgram class is instantiated, shaders are compiled, and 
  // the program is created and used.
  ShaderProgram sh1;
  
  sh1.CompileShader(GL_VERTEX_SHADER, "../res/shaders/triangle.vert");
  sh1.CompileShader(GL_FRAGMENT_SHADER, "../res/shaders/triangle.frag");
  sh1.CreateProgram();
  sh1.Use();

  sh1.SetInt("brickT", 0);
  sh1.SetInt("plateT", 1);

  // Wireframe mode (disable with glPolygonMode(GL_FRONT_AND_BACK, GL_FILL))
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while(!(glfwWindowShouldClose(win))) {

    process_input(win);

    glm::mat4 M(1.0f);
    
    M = glm::rotate(M, glm::radians((float) glfwGetTime() * 100),
                    glm::vec3(0.0f, 0.0f, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(sh1.GetID(), "M"), 1, GL_FALSE,
                       glm::value_ptr(M));

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
