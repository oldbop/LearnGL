#include <fstream>
#include <iostream>
#include <sstream>
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

std::string parse_shader(const std::string& path) {

  std::ifstream stream(path);
  std::string line;
  std::stringstream sstream;

  while(std::getline(stream, line)) {
    sstream << line << '\n';
  }

  return sstream.str();
}

unsigned int compile_shader(unsigned int type, const std::string& source) {

  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  char msg[512];
  int result;

  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if(result == GL_FALSE) {

    glGetShaderInfoLog(id, 512, nullptr, msg);
    std::cout << msg << std::endl;

    glDeleteShader(id);

    return 0;
  }

  return id;
}

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

  // Parsing and compiling the vertex shader
  std::string vert_str = parse_shader("../res/shaders/triangle.vert");
  unsigned int vert_sha = compile_shader(GL_VERTEX_SHADER, vert_str);

  // Parsing and compiling the fragment shader
  std::string frag_str = parse_shader("../res/shaders/triangle.frag");
  unsigned int frag_sha = compile_shader(GL_FRAGMENT_SHADER, frag_str);

  // Creating, linking, and using the shader program
  unsigned int shader_prog = glCreateProgram();
  
  glAttachShader(shader_prog, vert_sha);
  glAttachShader(shader_prog, frag_sha);
  
  glLinkProgram(shader_prog);
  glValidateProgram(shader_prog);

  glDeleteShader(vert_sha);
  glDeleteShader(frag_sha);

  glUseProgram(shader_prog);

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
