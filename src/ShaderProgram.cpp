#include "ShaderProgram.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifdef SYS_GL_HEADERS
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#else
#include <glad/gl.h>

#endif

void ShaderProgram::CompileShader(uint32_t type, const std::string &path) {

  std::ifstream fileStream(path);
  std::string line, strSource;
  std::stringstream strStream;
  
  uint32_t id;
  char msg[512];
  int32_t result;

  while(std::getline(fileStream, line)) {
    strStream << line << '\n';
  }

  strSource = strStream.str();
  const char *cSource = strSource.c_str();

  id = glCreateShader(type);

  glShaderSource(id, 1, &cSource, nullptr);
  glCompileShader(id);

  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if(result == GL_FALSE) {

    std::cout << "Shader compilation at " << path << " failed.\n"
              << "Compilation error:\n"
              << std::endl;

    glGetShaderInfoLog(id, 512, nullptr, msg);
    std::cout << msg << std::endl;

    glDeleteShader(id);

    return;
  }

  m_ShaderIDs.push_back(id);
}

void ShaderProgram::CreateProgram() {

  m_ID = glCreateProgram();
  char msg[512];
  int32_t result;

  for(const auto &shaderID : m_ShaderIDs) {
    glAttachShader(m_ID, shaderID);
  }

  glLinkProgram(m_ID);
  glGetProgramiv(m_ID, GL_LINK_STATUS, &result);

  if(result == GL_FALSE) {

    std::cout << "Program linking failed.\n"
              << "Linking error:\n"
              << std::endl;

    glGetProgramInfoLog(m_ID, 512, nullptr, msg);
    std::cout << msg << std::endl;
  }

  for(const auto &shaderID : m_ShaderIDs) {
    glDeleteShader(shaderID);
  }
}

void ShaderProgram::SetBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int) value);
}

void ShaderProgram::SetFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ShaderProgram::SetInt(const std::string &name, int32_t value) const {
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ShaderProgram::Use() {
  glUseProgram(m_ID);
}
