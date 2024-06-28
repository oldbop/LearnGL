#include "ShaderProgram.hpp"

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

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

const std::string TITLE = "LearnGL";

Dimensions screen = { 1000.0f, 700.0f };

bool fMouse = true;

float sensitivity = 0.1f;
float deltaTime   = 0.0f;
float lastFrame   = 0.0f;

float lastX = screen.x / 2;
float lastY = screen.y / 2;

float yaw   = -90.0f;
float pitch =   0.0f;
float fov   =  45.0f;

float     camSpeed = 8.0f;
glm::vec3 camPos   = glm::vec3(0.0f, 0.0f,  5.0f);
glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camUp    = glm::vec3(0.0f, 1.0f,  0.0f);

void mouse_callback(GLFWwindow *win, double xpos, double ypos) {

  if(fMouse) {
    lastX  = xpos;
    lastY  = ypos;
    fMouse = false;
  }

  float xoff = xpos - lastX;
  float yoff = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  xoff *= sensitivity;
  yoff *= sensitivity;

  yaw   += xoff;
  pitch += yoff;

  if(pitch > 89.0f) {
    pitch = 89.0f;
  } else if(pitch < -89.0f) {
    pitch = -89.0f;
  }

  glm::vec3 direction = glm::vec3(
    cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
    sin(glm::radians(pitch)),
    cos(glm::radians(pitch)) * sin(glm::radians(yaw))
  );

  camFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow *win, double xoff, double yoff) {

  fov -= yoff;

  if(fov < 1.0f) {
    fov = 1.0f;
  } else if(fov > 45.0f) {
    fov = 45.0f;
  }
}

void frame_resize_callback(GLFWwindow *win, int width, int height) {

  glViewport(0, 0, width, height);

  screen.x = width;
  screen.y = height;
}

void process_input(GLFWwindow *win, float time) {
    
  deltaTime = time - lastFrame;
  lastFrame = time;

  float camSpeedS = camSpeed * deltaTime;

  if(glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(win, true);
  }

  if(glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
    camPos += camSpeedS * camFront;
  }

  if(glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
    camPos -= camSpeedS * camFront;
  }

  if(glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
    camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpeedS;
  }

  if(glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
    camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpeedS;
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
  glfwSetFramebufferSizeCallback(win, frame_resize_callback);

  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(win, mouse_callback);

  glfwSetScrollCallback(win, scroll_callback);

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

    float time = (float) glfwGetTime();

    process_input(win, time);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // View matrix
    glm::mat4 V = glm::lookAt(camPos, camPos + camFront, camUp);

    glUniformMatrix4fv(glGetUniformLocation(sh1.GetID(), "View"), 1, GL_FALSE,
                                            glm::value_ptr(V));

    // Perspective matrix
    glm::mat4 P = glm::mat4(1.0f);
    P = glm::perspective(glm::radians(fov), screen.x / screen.y, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(sh1.GetID(), "Proj"), 1, GL_FALSE,
                                            glm::value_ptr(P));

    for(const auto &v : pos) {

      // Model matrix
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
