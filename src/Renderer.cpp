#include "Renderer.hpp"

#ifdef SYS_GL_HEADERS
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#else
#include <glad/gl.h>

#endif

#include <iostream>

void gl_clear_error(void) { while(glGetError()); }

bool gl_log_call(void) {

  while(GLenum err = glGetError()) {
    std::cout << "GL Error: " << err << std::endl;
    return false;
  }

  return true;
}
