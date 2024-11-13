#include "ShaderProgram.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#ifdef SYS_GL_HEADERS
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#else
#include <glad/gl.h>

#endif

void ShaderProgram::CompileShader(uint32_t type, const std::string &path) {

  // Possibly change this function to read the whole file at once, as opposed to line-by-line

  std::ifstream file_stream(path);
  std::string line;
  std::stringstream string_stream;

  while (std::getline(file_stream, line)) {
    string_stream << line << '\n';
  }

  std::string shader_src = string_stream.str();
  const char *c_shader_src = shader_src.c_str();

  uint32_t id = glCreateShader(type);

  glShaderSource(id, 1, &c_shader_src, nullptr);
  glCompileShader(id);

  int32_t result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE) {

    std::cout << "Shader compilation at " << path << " failed.\n"
              << "Compilation error:\n"
              << std::endl;

    char msg[512];
    glGetShaderInfoLog(id, 512, nullptr, msg);

    std::cout << msg << std::endl;

    glDeleteShader(id);

    return;
  }

  m_ShaderIDs.push_back(id);
}

void ShaderProgram::CreateProgram() {

  m_ID = glCreateProgram();

  for (const auto &shaderID : m_ShaderIDs) {
    glAttachShader(m_ID, shaderID);
  }

  glLinkProgram(m_ID);

  int32_t result;
  glGetProgramiv(m_ID, GL_LINK_STATUS, &result);

  if (result == GL_FALSE) {

    std::cout << "Program linking failed.\n"
              << "Linking error:\n"
              << std::endl;

    char msg[512];
    glGetProgramInfoLog(m_ID, 512, nullptr, msg);
    
    std::cout << msg << std::endl;
  }

  for (const auto &shaderID : m_ShaderIDs) {
    glDeleteShader(shaderID);
  }
}

void ShaderProgram::SetBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int32_t) value);
}

void ShaderProgram::SetFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ShaderProgram::SetInt(const std::string &name, int32_t value) const {
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ShaderProgram::Use() const {
  glUseProgram(m_ID);
}
